#include "Globals.h"
#include "Application.h"
#include "ModuleLoadMesh.h"
#include "p2Log.h"
#include "Primitive.h"

#include "DevIL\include\il.h"
#include "DevIL\include\ilu.h"
#include "DevIL\include\ilut.h"
#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "Assimp/include/Logger.hpp"
#include "Assimp/include/LogStream.hpp"
//#include "Assimp/include/NullLogger.hpp"
#include "Assimp/include/DefaultLogger.hpp"

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
	/*
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	*/

	// Create a logger instance 
	Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
	// Now I am ready for logging my stuff
	Assimp::DefaultLogger::get()->info("this is my info-call");
	// Select the kinds of messages you want to receive on this log stream
	const unsigned int severity = Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Err | Assimp::Logger::Warn;
	// Attaching it to the default logger
	Assimp::DefaultLogger::get()->attachStream(new Assimp::myStream(), severity);

	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilClearColour(255, 255, 255, 000);
	ILenum ilError = ilGetError();
	if (ilError != IL_NO_ERROR)
		ret = false;

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
	std::string* DroppedFile = App->input->GetDroppedFile();
	if (DroppedFile != nullptr)
	{
		bool loaded = Load(DroppedFile, &geomData);
		//If you load an fbx with more than one mesh, this center the last one
		if (loaded)
		{
			//Compute SceneAABB
			SceneAABB.SetNegativeInfinity();
			for (std::vector<GeometryData>::iterator item = geomData.begin(); item != geomData.cend(); ++item)
				SceneAABB.Enclose((float3*)item->vertices, item->num_vertices);
			App->camera->CenterCameraToGeometry(&SceneAABB);
		}
		else
		{
			LOGP("The file you dropped is being processed as a texture... Press Scroll Down");
			//Load texture?
			if (geomData.size() > 0)
			{
				uint tex_w = 0;
				uint tex_h = 0;
				uint tex_d = 0;
				int tex_id = LoadImageFromFile(DroppedFile->c_str(), tex_w, tex_h, tex_d);
				if (tex_id < 0)
				{
					LOGP("Error loading texture with path: %s", DroppedFile->c_str());
					LOGP("The file you dropped is not a texture / supported extension.");
				}
				else
				{
					LOGP("Texture loaded with path: %s", DroppedFile->c_str());
					for (std::vector<GeometryData>::iterator item = geomData.begin(); item != geomData.cend(); ++item)
					{
						if(item._Ptr->texture_name != "")
							glDeleteTextures(1, &item._Ptr->id_texture);
						item._Ptr->id_texture = tex_id;
						item._Ptr->texture_name = DroppedFile->c_str();
						item._Ptr->texture_w = tex_w;
						item._Ptr->texture_h = tex_h;
						item._Ptr->texture_d = tex_d;
					}
					LOGP("Texture aplied to geometry.");
				}
			}
			else
				LOGP("The dropped file has aborted loading process because there is no geometry to aply texture, in case that the dropped file was a texture.");
		}
	}
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
	if (geomLoaded)
		CleanGeometryDataVector(&geomData);
	// Kill it after the work is done
	Assimp::DefaultLogger::kill();
	ilShutDown();
	// detach log stream
	//aiDetachAllLogStreams();
	return true;
}

bool ModuleLoadMesh::LoadGeometryFromModelFile(std::string* file)
{
	bool ret = true;
	if (file == nullptr)
		return false;

	const aiScene* scene = aiImportFile(file->c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		//Get the working path to load textures from it
		std::size_t lastPos = file->rfind("\\");
		WorkingPath = file->substr(0, lastPos);
		WorkingPath += "\\";

		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (uint i = 0; i < scene->mNumMeshes; ++i)
			LoadGeometry(scene, i);

		WorkingPath.clear();
		aiReleaseImport(scene);
	}
	else
	{
		LOGP("Error loading scene %s", file->c_str());
		LOGP("Error: %s", aiGetErrorString());
		ret = false;
	}

	return ret;
}

void ModuleLoadMesh::LoadGeometry(const aiScene* scene, uint mesh_id)
{
	aiMesh* MeshInstance = scene->mMeshes[mesh_id];
	aiNode* MeshNode = SearchForMesh(scene->mRootNode, mesh_id);

	//Load Buffers
	//LoadBuffers();

	//Create GameObjects&Components and add them to root as child App->scene->AddChildToRoot

}

bool ModuleLoadMesh::CleanGeometryDataVector(std::vector<GeometryData>* meshDataVec)
{
	if (meshDataVec == nullptr)
		return false;
	for (std::vector<GeometryData>::iterator item = meshDataVec->begin(); item != meshDataVec->cend(); ++item)
	{
		item._Ptr->name.clear();
		if (item._Ptr->vertices != nullptr)
		{
			glDeleteBuffers(1, &item._Ptr->id_vertices);
			RELEASE_ARRAY(item._Ptr->vertices);
		}
		if (item._Ptr->indices != nullptr)
		{
			glDeleteBuffers(1, &item._Ptr->id_indices);
			RELEASE_ARRAY(item._Ptr->indices);
		}
		if (item._Ptr->normals != nullptr)
		{
			glDeleteBuffers(1, &item._Ptr->id_normals);
			RELEASE_ARRAY(item._Ptr->normals);
		}
		if (item._Ptr->colors != nullptr)
		{
			glDeleteBuffers(1, &item._Ptr->id_colors);
			RELEASE_ARRAY(item._Ptr->colors);
		}
		if (item._Ptr->texture_coords != nullptr)
		{
			glDeleteBuffers(1, &item._Ptr->id_texture_coords);
			RELEASE_ARRAY(item._Ptr->texture_coords);
		}
		if (item._Ptr->texture_name != "")
			glDeleteTextures(1, &item._Ptr->id_texture);
		item._Ptr->texture_name.clear();
	}
	meshDataVec->clear();
	return true;
}

bool ModuleLoadMesh::Load(std::string* file, std::vector<GeometryData>* meshDataOutput)
{
	bool ret = true;
	if (file == nullptr)
		return false;

	//aiIsExtensionSupported (const char *szExtension)

	const aiScene* scene = aiImportFile(file->c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		if (geomLoaded)
			CleanGeometryDataVector(meshDataOutput);

		//Get the working path to load textures from it
		std::size_t lastPos = file->rfind("\\");
		std::string WorkingPath = file->substr(0, lastPos);
		WorkingPath += "\\";

		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			const aiMesh* new_mesh = scene->mMeshes[i];

			GeometryData geomData;
			LoadMeshGeometry(geomData, scene, new_mesh, i, WorkingPath);
			LoadMeshBuffers(geomData);
			meshDataOutput->push_back(geomData);
		}
		WorkingPath.clear();
		aiReleaseImport(scene);
		geomLoaded = true;
	}
	else
	{
		LOGP("Error loading scene %s", file->c_str());
		LOGP("Error: %s", aiGetErrorString());
		ret = false;
	}

	return ret;
}

void ModuleLoadMesh::LoadMeshGeometry(GeometryData& geomData, const aiScene* scene, const aiMesh* new_mesh, uint meshID, std::string& WorkingPath)
{
	//This is necessary to load the transform of this node, we do here now to get the name of the node
	aiNode* MeshNode = SearchForMesh(scene->mRootNode, meshID);
	if (MeshNode != nullptr)
		geomData.name = MeshNode->mName.C_Str();

	// copy vertices
	geomData.num_vertices = new_mesh->mNumVertices;
	geomData.vertices = new float[geomData.num_vertices * 3];
	memcpy(geomData.vertices, new_mesh->mVertices, sizeof(float) * geomData.num_vertices * 3);
	LOGP("New mesh with %d vertices", geomData.num_vertices);

	// copy faces
	if (new_mesh->HasFaces())
	{
		geomData.num_faces = new_mesh->mNumFaces;
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

	//this causes some problems sometimes, so as this is a feature we don't use, we comment it and avoid crashes
	/*
	// colors
	if (new_mesh->HasVertexColors(0))
	{
		geomData.colors = new float[geomData.num_vertices * 3];
		memcpy(geomData.colors, new_mesh->mColors, sizeof(float) * geomData.num_vertices * 3);
	}
	*/

	// texture coords (only one texture for now)
	if (new_mesh->HasTextureCoords(0))
	{
		geomData.texture_coords = new float[geomData.num_vertices * 3];
		memcpy(geomData.texture_coords, new_mesh->mTextureCoords[0], sizeof(float) * geomData.num_vertices * 3);
	}

	//loadmeshtexture
	//Textures need to be inside Game folder
	//This can load fbx from any directory (desktop dor example), but textures are loaded from Game folder
	if (scene->HasMaterials())
	{
		aiString material_path;
		scene->mMaterials[new_mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &material_path);
		//geomData.texture_name = AssetsPath + material_path.C_Str();
		geomData.texture_name = WorkingPath + material_path.C_Str();
		//Check if this texture is already loaded
		int id_texture = 0;
		bool tex_alraedyLoaded = false;
		for (std::vector<GeometryData>::iterator item = this->geomData.begin(); item != this->geomData.cend(); ++item)
			if (item._Ptr->texture_name == geomData.texture_name)
			{
				//if the texture is already loaded just assign the same ID
				id_texture = item._Ptr->id_texture;
				geomData.texture_w = item._Ptr->texture_w;
				geomData.texture_h = item._Ptr->texture_h;
				geomData.texture_d = item._Ptr->texture_d;
				tex_alraedyLoaded = true;
				break;
			}
		//If the texture is new, load it
		if (!tex_alraedyLoaded)
			id_texture = LoadImageFromFile(geomData.texture_name.c_str(), geomData.texture_w, geomData.texture_h, geomData.texture_d);
		if (id_texture < 0)
		{
			LOGP("Error loading texture with path: %s", geomData.texture_name.c_str());
			geomData.texture_name.clear();
		}
		else
		{
			LOGP("Texture loaded with path: %s", geomData.texture_name.c_str());
			geomData.id_texture = id_texture;
		}
		material_path.Clear();
	}

	// Generate AABB
	geomData.BoundBox.SetNegativeInfinity();
	geomData.BoundBox.Enclose((float3*)geomData.vertices, geomData.num_vertices);

	//For first test, load pos/rot/scale
	float3 pos = { 0.0f,0.0f,0.0f };
	float3 scale = { 1.0f,1.0f,1.0f };
	Quat rot = { 0.0f,0.0f,0.0f,0.0f };
	if ((scene->mRootNode != nullptr) && (scene->mRootNode->mNumChildren > 0))
	{
		//Sum up all transformations from root node to the node where the mesh is stored
		aiMatrix4x4 transform;
		if (MeshNode != nullptr)
		{
			for (aiNode* iterator = MeshNode; iterator->mParent != nullptr; iterator = iterator->mParent)
				transform *= iterator->mTransformation;
			aiVector3D translation;
			aiVector3D scaling;
			aiQuaternion rotation;
			transform.Decompose(scaling, rotation, translation);
			pos = { translation.x, translation.y, translation.z };
			scale = { scaling.x, scaling.y, scaling.z };
			rot = { rotation.x, rotation.y, rotation.z, rotation.w };
		}
	}
	geomData.pos = pos;
	geomData.rot = rot;
	geomData.scale = scale;
}

void ModuleLoadMesh::LoadMeshBuffers(GeometryData& geomData)
{
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * geomData.num_vertices * 3, geomData.texture_coords, GL_STATIC_DRAW);
	}
}

// Function load a image, turn it into a texture, and return the texture ID as a GLuint for use
int ModuleLoadMesh::LoadImageFromFile(const char* theFileName, uint& tex_w, uint& tex_h, uint& tex_d)
{
	// Create an image ID as a ULuint
	ILuint imageID;
	// Create a texture ID as a GLuint
	GLuint textureID;
	// Create a flag to keep track of success/failure
	ILboolean success;
	// Create a flag to keep track of the IL error state
	ILenum error;
	// Generate the image ID
	ilGenImages(1, &imageID);
	// Bind the image
	ilBindImage(imageID);
	// Load the image file
	success = ilLoadImage(theFileName);

	// If we managed to load the image, then we can start to do things with it...
	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		tex_w = ImageInfo.Width;
		tex_h = ImageInfo.Height;
		tex_d = ImageInfo.Depth;
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			LOGP("Image conversion failed - IL reports error: %i - %s", error, iluErrorString(error));
			return -1;
		}

		// Generate a new texture
		glGenTextures(1, &textureID);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// Set texture interpolation method to use linear interpolation (no MIPMAPS)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
			0,				// Pyramid level (for mip-mapping) - 0 is the top level
			ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
			ilGetInteger(IL_IMAGE_WIDTH),	// Image width
			ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
			0,				// Border width in pixels (can either be 1 or 0)
			ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
			GL_UNSIGNED_BYTE,		// Image data type
			ilGetData());			// The actual image data itself

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else // If we failed to open the image file in the first place...
	{
		error = ilGetError();
		LOGP("Image load failed - IL reports error: %i - %s", error, iluErrorString(error));
		return -1;
	}

	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

	LOGP("Texture creation successful.");

	return textureID; // Return the GLuint to the texture so you can use it!
}

aiNode* ModuleLoadMesh::SearchForMesh(aiNode* root, uint mesh_id)
{
	aiNode* node = nullptr;
	if ((root != nullptr) && (root->mNumChildren > 0))
		node = SearchForMeshIterator(root, mesh_id);
	return node;
}

aiNode* ModuleLoadMesh::SearchForMeshIterator(aiNode* root, uint mesh_id)
{
	aiNode* node = nullptr;
	uint child = 0;
	for (node = root->mChildren[child]; child < root->mNumChildren; node = root->mChildren[++child])
	{
		if (node->mNumMeshes > 0)
			for (int i = 0; i < node->mNumMeshes; i++)
				if (node->mMeshes[i] == mesh_id)
					return node;
		if (node->mNumChildren > 0)
		{
			node = SearchForMeshIterator(root->mChildren[child], mesh_id);
			if (node != nullptr)
				return node;
		}
	}
	return nullptr;
}

bool ModuleLoadMesh::SaveConf(JSON_Object* conf) const
{
	App->parsonjson->SetString(conf, "AssetsPath", AssetsPath.c_str());
	return true;
}

bool ModuleLoadMesh::LoadConf(JSON_Object* conf)
{
	AssetsPath = App->parsonjson->GetString(conf, "AssetsPath", "Assets/");
	return true;
}

void ModuleLoadMesh::ImGuiModuleVariables()
{

}