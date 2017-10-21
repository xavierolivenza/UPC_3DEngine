#include <experimental\filesystem>

#include "ModuleSceneImporter.h"
#include "ImporterMesh.h"
#include "ImporterMaterial.h"

//Assimp includes here because the main core of 3dmodel file read action is done here
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleSceneImporter::ModuleSceneImporter(Application* parent, bool start_enabled) : Module(parent, start_enabled)
{
	name = "scene_importer";
}

ModuleSceneImporter::~ModuleSceneImporter()
{

}

bool ModuleSceneImporter::Init()
{
	MeshImporter = new ImporterMesh();
	MeshImporter->Init();
	MaterialImporter = new ImporterMaterial();
	MaterialImporter->Init();
	
	/*
	//Create Assets folder, this just fails if the folder is already created
	if (CreateDirectory(Assets_path.c_str(), false)) LOGP("Assets folder created");
	else LOGP("Assets folder already exists");

	//Create Library folder, this just fails if the folder is already created
	if (CreateDirectory(Library_path.c_str(), false)) LOGP("Library folder created");
	else LOGP("Library folder already exists");

	//Create Library/Mesh folder, this just fails if the folder is already created
	if (CreateDirectory(Library_mesh_path.c_str(), false)) LOGP("Mesh folder created");
	else LOGP("Mesh folder already exists");

	//Create Library/Material folder, this just fails if the folder is already created
	if (CreateDirectory(Library_material_path.c_str(), false)) LOGP("Material folder created");
	else LOGP("Material folder already exists");
	*/

	/**/
	//Create Assets folder, this just fails if the folder is already created
	if (std::experimental::filesystem::create_directory(Assets_path.c_str())) LOGP("Assets folder created");
	else LOGP("Assets folder already exists");

	//Create Library folder, this just fails if the folder is already created
	if (std::experimental::filesystem::create_directory(Library_path.c_str())) LOGP("Library folder created");
	else LOGP("Library folder already exists");

	//Create Library/Mesh folder, this just fails if the folder is already created
	if (std::experimental::filesystem::create_directory(Library_mesh_path.c_str())) LOGP("Mesh folder created");
	else LOGP("Mesh folder already exists");

	//Create Library/Material folder, this just fails if the folder is already created
	if (std::experimental::filesystem::create_directory(Library_material_path.c_str())) LOGP("Material folder created");
	else LOGP("Material folder already exists");
	/**/

	//Iterate all Assets folder including files and directories
	for (auto& file_in_path : std::experimental::filesystem::recursive_directory_iterator(Assets_path.c_str()))
	{
		LOGP("%S", file_in_path.path().c_str());
		if (std::experimental::filesystem::is_regular_file(file_in_path.path()))
		{
			std::string output_file;
			if(Import(&file_in_path.path().string(), output_file)) LOGP("Regular File found and imported.");
			LOGP("Regular File found and import error.");
		}
	}

	return true;
}

bool ModuleSceneImporter::Start()
{
	return true;
}

update_status ModuleSceneImporter::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneImporter::Update(float dt)
{
	std::string* DroppedFile = App->input->GetDroppedFile();
	if (DroppedFile != nullptr)
		Load(DroppedFile);
	return UPDATE_CONTINUE;
}

update_status ModuleSceneImporter::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleSceneImporter::CleanUp()
{
	MeshImporter->CleanUp();
	RELEASE(MeshImporter);
	MaterialImporter->CleanUp();
	RELEASE(MaterialImporter);
	return true;
}

void ModuleSceneImporter::DrawModuleImGui()
{

}

void ModuleSceneImporter::ImGuiModuleVariables()
{

}

bool ModuleSceneImporter::Import(std::string* file_to_import, std::string& output_file)
{
	bool ret = true;

	/*
	if (!std::experimental::filesystem::is_regular_file(file_to_import))
	{
		LOGP("The file you are trying to import is not a regular file: %s", file_to_import->c_str());
		return false;
	}
	*/

	if (file_to_import == nullptr)
		return false;

	const aiScene* scene = aiImportFile(file_to_import->c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		//Get the working path to load textures from it
		WorkingPath = file_to_import->substr(0, file_to_import->rfind("\\"));
		WorkingPath += "\\";

		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			aiNode* MeshNode = SearchForMesh(scene->mRootNode, i);
			aiMesh* MeshInstance = scene->mMeshes[i];
			
			//------------------------------------------//
			//---------------Load Mesh------------------//
			//------------------------------------------//
			MeshData MeshDataStruct;

			//Load name
			MeshDataStruct.Mesh_name = MeshNode->mName.C_Str();

			// copy vertices
			MeshDataStruct.num_vertices = MeshInstance->mNumVertices;
			MeshDataStruct.vertices = new float[MeshDataStruct.num_vertices * 3];
			memcpy(MeshDataStruct.vertices, MeshInstance->mVertices, sizeof(float) * MeshDataStruct.num_vertices * 3);
			LOGP("New mesh with %d vertices importing process...", MeshDataStruct.num_vertices);

			// copy faces
			if (MeshInstance->HasFaces())
			{
				MeshDataStruct.num_faces = MeshInstance->mNumFaces;
				MeshDataStruct.num_indices = MeshInstance->mNumFaces * 3;
				MeshDataStruct.indices = new uint[MeshDataStruct.num_indices]; // assume each face is a triangle
				for (uint i = 0; i < MeshInstance->mNumFaces; ++i)
				{
					if (MeshInstance->mFaces[i].mNumIndices != 3)
						LOGP("WARNING, geometry face with != 3 indices!");
					else
						memcpy(&MeshDataStruct.indices[i * 3], MeshInstance->mFaces[i].mIndices, 3 * sizeof(uint));
				}
			}

			// normals
			if (MeshInstance->HasNormals())
			{
				MeshDataStruct.normals = new float[MeshDataStruct.num_vertices * 3];
				memcpy(MeshDataStruct.normals, MeshInstance->mNormals, sizeof(float) * MeshDataStruct.num_vertices * 3);
			}

			// texture coords (only one texture for now)
			if (MeshInstance->HasTextureCoords(0))
			{
				MeshDataStruct.texture_coords = new float[MeshDataStruct.num_vertices * 3];
				memcpy(MeshDataStruct.texture_coords, MeshInstance->mTextureCoords[0], sizeof(float) * MeshDataStruct.num_vertices * 3);
			}

			// colors
			if (MeshInstance->HasVertexColors(0))
			{
				//this causes some problems sometimes, so as this is a feature we don't use, we comment it and avoid crashes
				//meshComponent->MeshDataStruct.colors = new float[meshComponent->MeshDataStruct.num_vertices * 3];
				//memcpy(meshComponent->MeshDataStruct.colors, MeshInstance->mColors, sizeof(float) * meshComponent->MeshDataStruct.num_vertices * 3);
			}

			// Generate AABB
			MeshDataStruct.BoundBox.SetNegativeInfinity();
			MeshDataStruct.BoundBox.Enclose((float3*)MeshDataStruct.vertices,MeshDataStruct.num_vertices);
			
			// Generate Sphere
			MeshDataStruct.BoundSphere.SetNegativeInfinity();
			MeshDataStruct.BoundSphere.Enclose(MeshDataStruct.BoundBox);

			// Generate OBB
			/*
			meshComponent->MeshDataStruct.BoundOBox.SetNegativeInfinity();
			for (uint i = 0; i < meshComponent->MeshDataStruct.num_vertices * 3; i += 3)
			meshComponent->MeshDataStruct.BoundOBox.Enclose(vec(meshComponent->MeshDataStruct.vertices[i], meshComponent->MeshDataStruct.vertices[i + 1], meshComponent->MeshDataStruct.vertices[i + 2]));
			meshComponent->GenerateOBBDraw();
			*/

			//------------------------------------------//
			//-------------Load Material----------------//
			//------------------------------------------//

			//load texture
			if (scene->HasMaterials())
			{
				aiString material_path;
				scene->mMaterials[MeshInstance->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &material_path);
				MeshDataStruct.Asociated_texture_name = material_path.C_Str();

				//------------------------------------------//
				//-------Serialize Tex To Own Format--------//
				//------------------------------------------//
				std::string output;
				MaterialImporter->Save(&(WorkingPath + material_path.C_Str()), output);
			}

			//------------------------------------------//
			//-------Serialize Mesh To Own Format-------//
			//------------------------------------------//
			std::string output;
			MeshImporter->Save(MeshDataStruct, output);
		}
		WorkingPath.clear();
		aiReleaseImport(scene);
	}
	else
	{
		LOGP("While importing process. Error loading scene %s", file_to_import->c_str());
		LOGP("While importing process. Error: %s", aiGetErrorString());
		ret = false;
	}

	return ret;
}

bool ModuleSceneImporter::Load(std::string* file_to_load)
{
	//Check if this is a mesh file
	if (file_to_load->substr(file_to_load->rfind(".") + 1, file_to_load->length()) != Mesh_Extention)
	{
		LOGP("The dropped file is not a .%s file", Mesh_Extention.c_str());
		return false;
	}

	GameObject* NewGameObject = new GameObject("NewMesh", true, true);
	ComponentMesh* NewMesh = NewGameObject->CreateMeshComponent(true);
	MeshImporter->Load(NewMesh->MeshDataStruct, file_to_load);
	NewGameObject->name = NewMesh->MeshDataStruct.Mesh_name;
	ComponentMaterial* NewMaterial = NewGameObject->CreateMaterialComponent(true);
	MaterialImporter->Load(NewMaterial->MaterialDataStruct, &(Library_material_path + NewMesh->MeshDataStruct.Asociated_texture_name));
	App->scene->AddChildToRoot(NewGameObject);

	return true;
}

bool ModuleSceneImporter::SaveConf(JSON_Object* conf) const
{
	return true;
}

bool ModuleSceneImporter::LoadConf(JSON_Object* conf)
{
	return true;
}

aiNode* ModuleSceneImporter::SearchForMesh(const aiNode* root, uint mesh_id) const
{
	aiNode* node = nullptr;
	if ((root != nullptr) && (root->mNumChildren > 0))
		node = SearchForMeshIterator(root, mesh_id);
	return node;
}

aiNode* ModuleSceneImporter::SearchForMeshIterator(const aiNode* root, uint mesh_id) const
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

const std::string* ModuleSceneImporter::Get_Assets_path() const
{
	return &Assets_path;
}

const std::string* ModuleSceneImporter::Get_Library_path() const
{
	return &Library_path;
}

const std::string* ModuleSceneImporter::Get_Library_mesh_path() const
{
	return &Library_mesh_path;
}

const std::string* ModuleSceneImporter::Get_Library_material_path() const
{
	return &Library_material_path;
}

const std::string* ModuleSceneImporter::Get_Mesh_Extention() const
{
	return &Mesh_Extention;
}