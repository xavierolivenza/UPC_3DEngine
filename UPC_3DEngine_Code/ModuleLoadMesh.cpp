#include "Globals.h"
#include "Application.h"
#include "ModuleLoadMesh.h"
#include "p2Log.h"
#include "Primitive.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleLoadMesh::ModuleLoadMesh(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "loadmesh";
}

// Destructor
ModuleLoadMesh::~ModuleLoadMesh()
{}

// Called before render is available
bool ModuleLoadMesh::Init()
{
	LOGP("LoadMesh Init");
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

bool ModuleLoadMesh::Start()
{
	LOGP("LoadMesh Start");
	bool ret = true;
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleLoadMesh::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleLoadMesh::Update(float dt)
{
	Load(App->input->GetDroppedFile(), geomData);
	if (geomLoaded)
		App->renderer3D->Draw(&geomData);
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleLoadMesh::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleLoadMesh::CleanUp()
{
	LOGP("LoadMesh CleanUp");
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}

bool ModuleLoadMesh::Load(std::string* file, std::vector<GeometryData>& meshDataOutput)
{
	bool ret = true;
	if (file == nullptr)
		return false;

	if (geomLoaded)
	{
		//clean vector
		for (std::vector<GeometryData>::iterator item = meshDataOutput.begin(); item != meshDataOutput.cend(); ++item)
		{
			RELEASE_ARRAY(item._Ptr->vertices);
			RELEASE_ARRAY(item._Ptr->indices);
			glDeleteBuffers(1, &item._Ptr->id_vertices);
			glDeleteBuffers(1, &item._Ptr->id_indices);
		}
		meshDataOutput.clear();
	}

	const aiScene* scene = aiImportFile(file->c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			const aiMesh* new_mesh = scene->mMeshes[i];

			GeometryData geomData;

			// copy vertices
			geomData.num_vertices = new_mesh->mNumVertices;
			geomData.vertices = new float[geomData.num_vertices * 3];
			memcpy(geomData.vertices, new_mesh->mVertices, sizeof(float) * geomData.num_vertices * 3);
			LOGP("New mesh with %d vertices", geomData.num_vertices);

			// copy faces
			if (new_mesh->HasFaces())
			{
				geomData.num_indices = new_mesh->mNumFaces * 3;
				geomData.indices = new uint[geomData.num_indices]; // assume each face is a triangle
				for (uint i = 0; i < new_mesh->mNumFaces; ++i)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3)
						LOGP("WARNING, geometry face with != 3 indices!");
					else
						memcpy(&geomData.indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}
			}

			// normals
			if (new_mesh->HasNormals())
			{
				geomData.normals = new float[geomData.num_vertices * 3];
				memcpy(geomData.normals, new_mesh->mNormals, sizeof(float) * geomData.num_vertices * 3);
			}

			// colors
			if (new_mesh->HasVertexColors(0))
			{
				geomData.colors = new float[geomData.num_vertices * 3];
				memcpy(geomData.colors, new_mesh->mColors, sizeof(float) * geomData.num_vertices * 3);
			}

			// texture coords (only one texture for now)
			if (new_mesh->HasTextureCoords(0))
			{
				geomData.texture_coords = new float[geomData.num_vertices * 2];
				memcpy(geomData.texture_coords, new_mesh->mTextureCoords[0], sizeof(float) * geomData.num_vertices * 2);
			}

			// Generate AABB
			geomData.BoundBox.SetNegativeInfinity();
			geomData.BoundBox.Enclose((float3*)geomData.vertices, geomData.num_vertices);

			// Buffer for vertices
			glGenBuffers(1, (GLuint*) &(geomData.id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, geomData.id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * geomData.num_vertices * 3, geomData.vertices, GL_STATIC_DRAW);

			// Buffer for indices
			glGenBuffers(1, (GLuint*) &(geomData.id_indices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geomData.id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * geomData.num_indices, geomData.indices, GL_STATIC_DRAW);

			// Buffer for normals
			if (geomData.normals != nullptr)
			{
				glGenBuffers(1, (GLuint*) &(geomData.id_normals));
				glBindBuffer(GL_ARRAY_BUFFER, geomData.id_normals);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * geomData.num_vertices * 3, geomData.normals, GL_STATIC_DRAW);
			}

			// Buffer for vertex colors
			if (geomData.colors != nullptr)
			{
				glGenBuffers(1, (GLuint*) &(geomData.id_colors));
				glBindBuffer(GL_ARRAY_BUFFER, geomData.id_colors);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * geomData.num_vertices * 3, geomData.colors, GL_STATIC_DRAW);
			}

			// Buffer for texture coords
			if (geomData.texture_coords != nullptr)
			{
				glGenBuffers(1, (GLuint*) &(geomData.id_texture_coords));
				glBindBuffer(GL_ARRAY_BUFFER, geomData.id_texture_coords);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * geomData.num_vertices * 2, geomData.texture_coords, GL_STATIC_DRAW);
			}

			meshDataOutput.push_back(geomData);
		}
		aiReleaseImport(scene);
		geomLoaded = true;
	}
	else
	{
		LOGP("Error loading scene %s", file->c_str());
		LOGP("Error: %s", aiGetErrorString());
	}

	return ret;
}

bool ModuleLoadMesh::SaveConf(JSON_Object* conf) const
{
	return true;
}

bool ModuleLoadMesh::LoadConf(JSON_Object* conf)
{
	return true;
}

void ModuleLoadMesh::ImGuiModuleVariables()
{

}