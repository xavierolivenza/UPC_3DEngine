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
			App->camera->CenterCameraToGeometry(&geomData.back().BoundBox);
		else
		{
			LOGP("The file you dropped is being processed as a texture...");
			//Load texture?
			if (geomData.size() > 0)
			{
				int tex_id = LoadImageFromFile(DroppedFile->c_str());
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
						glDeleteTextures(1, &item._Ptr->id_texture);
						item._Ptr->id_texture = tex_id;
						item._Ptr->texture_name = DroppedFile->c_str();
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

bool ModuleLoadMesh::CleanGeometryDataVector(std::vector<GeometryData>* meshDataVec)
{
	if (meshDataVec == nullptr)
		return false;
	for (std::vector<GeometryData>::iterator item = meshDataVec->begin(); item != meshDataVec->cend(); ++item)
	{
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

			// colors
			if (new_mesh->HasVertexColors(0))
			{
				geomData.colors = new float[geomData.num_vertices * 3];
				memcpy(geomData.colors, new_mesh->mColors, sizeof(float) * geomData.num_vertices * 3);
			}

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
				aiString path;
				scene->mMaterials[new_mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
				geomData.texture_name = AssetsPath + path.C_Str();
				int id_texture = LoadImageFromFile(geomData.texture_name.c_str());
				if (id_texture < 0)
				{
					LOGP("Error loading texture with path: %s", geomData.texture_name.c_str());
					geomData.texture_name = "";
				}
				else
				{
					LOGP("Texture loaded with path: %s", geomData.texture_name.c_str());
					geomData.id_texture = id_texture;
				}
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
				aiMatrix4x4 transform;
				for (aiNode* node = scene->mRootNode->mChildren[0]; node->mNumChildren > 0; node = node->mChildren[0])
					transform *= node->mTransformation;
				aiVector3D translation;
				aiVector3D scaling;
				aiQuaternion rotation;
				transform.Decompose(scaling, rotation, translation);
				pos = { translation.x, translation.y, translation.z };
				scale = { scaling.x, scaling.y, scaling.z };
				rot = { rotation.x, rotation.y, rotation.z, rotation.w };
			}
			geomData.pos = pos;
			geomData.rot = rot;
			geomData.scale = scale;

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

			/*
			aiVector3D translation;
			aiVector3D scaling;
			aiQuaternion rotation;
			node->mTransformation.Decompose(scaling, rotation, translation);
			*/

			/*
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
			if (numTextures >= 0)
			{
				aiString path;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			}
			*/

			meshDataOutput->push_back(geomData);
		}
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

// Function load a image, turn it into a texture, and return the texture ID as a GLuint for use
int ModuleLoadMesh::LoadImageFromFile(const char* theFileName)
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