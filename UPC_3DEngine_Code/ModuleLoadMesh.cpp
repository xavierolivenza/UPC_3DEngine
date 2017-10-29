#include "Globals.h"
#include "Application.h"
#include "ModuleLoadMesh.h"
#include "p2Log.h"
#include "Primitive.h"

#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

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

	// Create a logger instance 
	Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
	// Now I am ready for logging my stuff
	Assimp::DefaultLogger::get()->info("this is my info-call");
	// Select the kinds of messages you want to receive on this log stream
	const unsigned int severity = Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Err | Assimp::Logger::Warn;
	// Attaching it to the default logger
	Assimp::DefaultLogger::get()->attachStream(new Assimp::myStream(), severity);

	/*
	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilClearColour(255, 255, 255, 000);
	ILenum ilError = ilGetError();
	if (ilError != IL_NO_ERROR)
		ret = false;
	*/
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
	/*
	std::string* DroppedFile = App->input->GetDroppedFile();
	if (DroppedFile != nullptr)
		LoadGeometryFromModelFile(DroppedFile);
	*/
	/*
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
	*/
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
	// Kill it after the work is done
	Assimp::DefaultLogger::kill();
	ilShutDown();
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
		WorkingPath = file->substr(0, file->rfind("\\"));
		WorkingPath += "\\";

		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			aiNode* MeshNode = SearchForMesh(scene->mRootNode, i);
			aiMesh* MeshInstance = scene->mMeshes[i];
			GameObject* gameObject = new GameObject(MeshNode->mName.C_Str(), true, true);
			LoadGeometry(scene, gameObject, MeshNode, MeshInstance);
			App->scene->AddChildToRoot(gameObject);
		}
		
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

void ModuleLoadMesh::LoadGeometry(const aiScene* scene, GameObject* gameObject, const aiNode* MeshNode, const aiMesh* MeshInstance)
{
	if ((scene == nullptr) || (gameObject == nullptr) || (MeshNode == nullptr) || (MeshInstance == nullptr))
		return;

	//------------------------------------------//
	//-------------Load Transform---------------//
	//------------------------------------------//
	ComponentTransform* transformComponent = gameObject->GetTransform();
	
	float3 pos = { 0.0f,0.0f,0.0f };
	float3 scale = { 1.0f,1.0f,1.0f };
	Quat rot = { 0.0f,0.0f,0.0f,1.0f };
	if ((scene->mRootNode != nullptr) && (scene->mRootNode->mNumChildren > 0))
	{
		//Sum up all transformations from root node to the node where the mesh is stored
		aiMatrix4x4 transform;
		if (MeshNode != nullptr)
		{
			for (const aiNode* iterator = MeshNode; iterator->mParent != nullptr; iterator = iterator->mParent)
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
	transformComponent->SetPos(pos);
	transformComponent->SetRot(rot);
	transformComponent->SetScale(scale);

	//------------------------------------------//
	//---------------Load Mesh------------------//
	//------------------------------------------//
	ComponentMesh* meshComponent = gameObject->CreateMeshComponent(true);

	// copy vertices
	meshComponent->MeshDataStruct.num_vertices = MeshInstance->mNumVertices;
	meshComponent->MeshDataStruct.vertices = new float[meshComponent->MeshDataStruct.num_vertices * 3];
	memcpy(meshComponent->MeshDataStruct.vertices, MeshInstance->mVertices, sizeof(float) * meshComponent->MeshDataStruct.num_vertices * 3);
	LOGP("New mesh with %d vertices", meshComponent->MeshDataStruct.num_vertices);

	// copy faces
	if (MeshInstance->HasFaces())
	{
		meshComponent->MeshDataStruct.num_faces = MeshInstance->mNumFaces;
		meshComponent->MeshDataStruct.num_indices = MeshInstance->mNumFaces * 3;
		meshComponent->MeshDataStruct.indices = new uint[meshComponent->MeshDataStruct.num_indices]; // assume each face is a triangle
		for (uint i = 0; i < MeshInstance->mNumFaces; ++i)
		{
			if (MeshInstance->mFaces[i].mNumIndices != 3)
				LOGP("WARNING, geometry face with != 3 indices!");
			else
				memcpy(&meshComponent->MeshDataStruct.indices[i * 3], MeshInstance->mFaces[i].mIndices, 3 * sizeof(uint));
		}
	}

	// normals
	if (MeshInstance->HasNormals())
	{
		meshComponent->MeshDataStruct.normals = new float[meshComponent->MeshDataStruct.num_vertices * 3];
		memcpy(meshComponent->MeshDataStruct.normals, MeshInstance->mNormals, sizeof(float) * meshComponent->MeshDataStruct.num_vertices * 3);
	}

	// texture coords (only one texture for now)
	if (MeshInstance->HasTextureCoords(0))
	{
		meshComponent->MeshDataStruct.texture_coords = new float[meshComponent->MeshDataStruct.num_vertices * 3];
		memcpy(meshComponent->MeshDataStruct.texture_coords, MeshInstance->mTextureCoords[0], sizeof(float) * meshComponent->MeshDataStruct.num_vertices * 3);
	}
	//this causes some problems sometimes, so as this is a feature we don't use, we comment it and avoid crashes
	// colors
	/*
	if (MeshInstance->HasVertexColors(0))
	{
		meshComponent->MeshDataStruct.colors = new float[meshComponent->MeshDataStruct.num_vertices * 3];
		memcpy(meshComponent->MeshDataStruct.colors, MeshInstance->mColors, sizeof(float) * meshComponent->MeshDataStruct.num_vertices * 3);
	}
	*/

	// Generate AABB
	meshComponent->MeshDataStruct.BoundBox.SetNegativeInfinity();
	meshComponent->MeshDataStruct.BoundBox.Enclose((float3*)meshComponent->MeshDataStruct.vertices, meshComponent->MeshDataStruct.num_vertices);

	meshComponent->MeshDataStruct.BoundSphere.SetNegativeInfinity();
	meshComponent->MeshDataStruct.BoundSphere.Enclose(meshComponent->MeshDataStruct.BoundBox);

	/*
	meshComponent->MeshDataStruct.BoundOBox.SetNegativeInfinity();
	for (uint i = 0; i < meshComponent->MeshDataStruct.num_vertices * 3; i += 3)
		meshComponent->MeshDataStruct.BoundOBox.Enclose(vec(meshComponent->MeshDataStruct.vertices[i], meshComponent->MeshDataStruct.vertices[i + 1], meshComponent->MeshDataStruct.vertices[i + 2]));
	meshComponent->GenerateOBBDraw();
	*/
	
	//------------------------------------------//
	//-------------Load Material----------------//
	//------------------------------------------//
	ComponentMaterial* materialComponent = gameObject->CreateMaterialComponent(true);

	//load texture
	if (scene->HasMaterials())
	{
		aiString material_path;
		scene->mMaterials[MeshInstance->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &material_path);
		materialComponent->MaterialDataStruct.texture_name = WorkingPath + material_path.C_Str();

		int id_texture = 0;
		ComponentMaterial* AlreadyLoaded = SearchForTexture(materialComponent, App->scene->GetRoot(), &materialComponent->MaterialDataStruct.texture_name);
		//If the texture is new, load it
		if (AlreadyLoaded == nullptr)
		{
			id_texture = LoadImageFromFile(materialComponent->MaterialDataStruct.texture_name.c_str(), &materialComponent->MaterialDataStruct);
			if (id_texture < 0)
			{
				LOGP("Error loading texture with path: %s", materialComponent->MaterialDataStruct.texture_name.c_str());
				materialComponent->MaterialDataStruct.texture_name.clear();
			}
			else
			{
				LOGP("Texture loaded with path: %s", materialComponent->MaterialDataStruct.texture_name.c_str());
				materialComponent->MaterialDataStruct.id_texture = id_texture;
			}
		}
		//if not, copy data
		else
		{
			materialComponent->MaterialDataStruct = AlreadyLoaded->MaterialDataStruct;
			LOGP("Texture copied from other GameObject");
			LOGP("Texture path: %s", materialComponent->MaterialDataStruct.texture_name.c_str());
		}
	}

	//Load Buffers
	LoadBuffers(meshComponent);
}

void ModuleLoadMesh::LoadBuffers(ComponentMesh* meshComponent)
{
	if (meshComponent != nullptr)
	{
		// Buffer for vertices
		LOGP("Loading Buffers Vertex.");
		glGenBuffers(1, (GLuint*) &(meshComponent->MeshDataStruct.id_vertices));
		glBindBuffer(GL_ARRAY_BUFFER, meshComponent->MeshDataStruct.id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshComponent->MeshDataStruct.num_vertices * 3, meshComponent->MeshDataStruct.vertices, GL_STATIC_DRAW);

		// Buffer for indices
		LOGP("Loading Buffers Index.");
		glGenBuffers(1, (GLuint*) &(meshComponent->MeshDataStruct.id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshComponent->MeshDataStruct.id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * meshComponent->MeshDataStruct.num_indices, meshComponent->MeshDataStruct.indices, GL_STATIC_DRAW);

		// Buffer for normals
		if (meshComponent->MeshDataStruct.normals != nullptr)
		{
			LOGP("Loading Buffers Normals.");
			glGenBuffers(1, (GLuint*) &(meshComponent->MeshDataStruct.id_normals));
			glBindBuffer(GL_ARRAY_BUFFER, meshComponent->MeshDataStruct.id_normals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshComponent->MeshDataStruct.num_vertices * 3, meshComponent->MeshDataStruct.normals, GL_STATIC_DRAW);
		}

		// Buffer for texture coords
		if (meshComponent->MeshDataStruct.texture_coords != nullptr)
		{
			LOGP("Loading Buffers Texture Coords.");
			glGenBuffers(1, (GLuint*) &(meshComponent->MeshDataStruct.id_texture_coords));
			glBindBuffer(GL_ARRAY_BUFFER, meshComponent->MeshDataStruct.id_texture_coords);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshComponent->MeshDataStruct.num_vertices * 3, meshComponent->MeshDataStruct.texture_coords, GL_STATIC_DRAW);
		}

		// Buffer for vertex colors
		if (meshComponent->MeshDataStruct.colors != nullptr)
		{
			LOGP("Loading Buffers Colors.");
			glGenBuffers(1, (GLuint*) &(meshComponent->MeshDataStruct.id_colors));
			glBindBuffer(GL_ARRAY_BUFFER, meshComponent->MeshDataStruct.id_colors);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) *  meshComponent->MeshDataStruct.num_vertices * 3, meshComponent->MeshDataStruct.colors, GL_STATIC_DRAW);
		}
	}
	else
		LOGP("Loading Buffers meshComponent was nullptr.");
}

// Function load a image, turn it into a texture, and return the texture ID as a GLuint for use
int ModuleLoadMesh::LoadImageFromFile(const char* theFileName, MaterialData* MaterailDataStruct)
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
		if (MaterailDataStruct != nullptr)
		{
			MaterailDataStruct->texture_w = ImageInfo.Width;
			MaterailDataStruct->texture_h = ImageInfo.Height;
			MaterailDataStruct->texture_d = ImageInfo.Depth;
		}
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

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

aiNode* ModuleLoadMesh::SearchForMesh(const aiNode* root, uint mesh_id) const
{
	aiNode* node = nullptr;
	if ((root != nullptr) && (root->mNumChildren > 0))
		node = SearchForMeshIterator(root, mesh_id);
	return node;
}

aiNode* ModuleLoadMesh::SearchForMeshIterator(const aiNode* root, uint mesh_id) const
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

ComponentMaterial* ModuleLoadMesh::SearchForTexture(const ComponentMaterial* materialComponent, const GameObject* root, std::string* texture_name) const
{
	ComponentMaterial* ComponentMaterial_ret = nullptr;
	if ((root != nullptr) && (root->GetChildren()->size() > 0))
		ComponentMaterial_ret = SearchForTextureIterator(materialComponent, root, texture_name);
	return ComponentMaterial_ret;
}

ComponentMaterial* ModuleLoadMesh::SearchForTextureIterator(const ComponentMaterial* materialComponent, const GameObject* root, std::string* texture_name) const
{
	ComponentMaterial* ComponentMaterial_ret = nullptr;
	uint child = 0;
	for (std::vector<GameObject*>::const_iterator item = root->GetChildren()->cbegin(); item != root->GetChildren()->cend(); ++item, ++child)
	{
		const Component* material_comp = (*item)->FindComponentFirst(ComponentType::Material_Component);
		if ((material_comp != nullptr) && (material_comp != materialComponent))
			if (((ComponentMaterial*)material_comp)->MaterialDataStruct.texture_name == *texture_name)
				return (ComponentMaterial*)material_comp;
		if ((*item)->GetChildren()->size() > 0)
		{
			ComponentMaterial_ret = SearchForTextureIterator(materialComponent, (*item), texture_name);
			if (ComponentMaterial_ret != nullptr)
				return ComponentMaterial_ret;
		}
	}
	return nullptr;
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