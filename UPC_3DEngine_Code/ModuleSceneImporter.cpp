#include <experimental\filesystem>
#include <fstream>

#include "ModuleSceneImporter.h"
#include "ImporterMesh.h"
#include "ImporterMaterial.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"

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

	//Create Settings folder, this just fails if the folder is already created
	if (CreateDirectory(Settings_path.c_str(), false)) LOGP("Material folder created");
	else LOGP("Material folder already exists");

	//Create Scenes folder, this just fails if the folder is already created
	if (CreateDirectory(Scenes_path.c_str(), false)) LOGP("Material folder created");
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

	//Create Settings folder, this just fails if the folder is already created
	if (std::experimental::filesystem::create_directory(Settings_path.c_str())) LOGP("Settings folder created");
	else LOGP("Settings folder already exists");

	//Create Scenes folder, this just fails if the folder is already created
	if (std::experimental::filesystem::create_directory(Scenes_path.c_str())) LOGP("Scenes folder created");
	else LOGP("Scenes folder already exists");
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

	if (file_to_import == nullptr)
		return false;

	const aiScene* scene = aiImportFile(file_to_import->c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		//Get the working path to load textures from it
		WorkingPath = file_to_import->substr(0, file_to_import->rfind("\\"));
		WorkingPath += "\\";

		std::vector<std::string> fbx_MeshComponents;

		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			aiNode* MeshNode = SearchForMesh(scene->mRootNode, i);
			aiMesh* MeshInstance = scene->mMeshes[i];
			
			//------------------------------------------//
			//-------------Load Transform---------------//
			//------------------------------------------//
			float3 pos = { 0.0f,0.0f,0.0f };
			float3 scale = { 1.0f,1.0f,1.0f };
			Quat rot = { 0.0f,0.0f,0.0f,1.0f };
			if ((scene->mRootNode != nullptr) && (scene->mRootNode->mNumChildren > 0))
			{
				//Sum up all transformations from root node to the node where the mesh is stored
				aiMatrix4x4 transform;
				if (MeshNode != nullptr)
				{
					std::vector<const aiNode*> NodesVector;
					for (const aiNode* iterator = MeshNode; iterator->mParent != nullptr; iterator = iterator->mParent)
						NodesVector.push_back(iterator);
					for (std::vector<const aiNode*>::reverse_iterator item = NodesVector.rbegin(); item != NodesVector.crend(); ++item)
						transform *= (*item)->mTransformation;
					aiVector3D translation;
					aiVector3D scaling;
					aiQuaternion rotation;
					transform.Decompose(scaling, rotation, translation);
					pos = { translation.x, translation.y, translation.z };
					scale = { scaling.x, scaling.y, scaling.z };
					rot = { rotation.x, rotation.y, rotation.z, rotation.w };
				}
			}

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
			//MeshDataStruct.BoundSphere.SetNegativeInfinity();
			//MeshDataStruct.BoundSphere.Enclose(MeshDataStruct.BoundBox);

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
			MeshImporter->Save(pos, scale, rot, MeshDataStruct, output);
			fbx_MeshComponents.push_back(output);
		}

		//Save fbx_MeshComponents
		ImportFBXComponents(file_to_import, &fbx_MeshComponents, output_file);

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

bool ModuleSceneImporter::ImportFBXComponents(const std::string* file_to_import, const std::vector<std::string>* FBXComponents, std::string& output_file)
{
	//Num strings / length of each / strings

	size_t bar_pos = file_to_import->rfind("\\") + 1;
	output_file = file_to_import->substr(bar_pos, file_to_import->rfind(".") - bar_pos);
	output_file += "." + FBXComponents_Extention;

	std::string FBXComponents_path = Library_mesh_path + "\\" + output_file;
	FILE* file = fopen(FBXComponents_path.c_str(), "r");
	if (file != nullptr)
	{
		fclose(file);
		LOGP("Mesh file already exists: %s", FBXComponents_path.c_str());
		return false;
	}

	std::vector<uint> amount_of_each;
	for (std::vector<std::string>::const_iterator item = FBXComponents->cbegin(); item != FBXComponents->cend(); ++item)
		amount_of_each.push_back(item->length() + 1);

	uint file_size = sizeof(uint);
	file_size += sizeof(uint) * amount_of_each.size();
	for (std::vector<uint>::const_iterator item = amount_of_each.cbegin(); item != amount_of_each.cend(); ++item)
		file_size += sizeof(char) * (*item);

	//Create char* to allocate data and another char* to move around the previous one
	char* data = new char[file_size];
	char* cursor = data;
	uint current_allocation_size = 0;

	current_allocation_size = sizeof(uint);
	uint amount_of_each_size = amount_of_each.size();
	memcpy(cursor, &amount_of_each_size, current_allocation_size);

	for (std::vector<std::string>::const_iterator item = FBXComponents->cbegin(); item != FBXComponents->cend(); ++item)
	{
		cursor += current_allocation_size;
		current_allocation_size = sizeof(uint);
		uint leng = item->length() + 1;
		memcpy(cursor, &leng, current_allocation_size);
	}

	for (std::vector<std::string>::const_iterator item = FBXComponents->cbegin(); item != FBXComponents->cend(); ++item)
	{
		cursor += current_allocation_size;
		current_allocation_size = sizeof(char) * (item->length() + 1);
		memcpy(cursor, item->c_str(), current_allocation_size);
	}

	//Write to file
	std::ofstream outfile(FBXComponents_path, std::ofstream::binary);
	if (outfile.good()) //write file
		outfile.write(data, file_size);
	else
		LOGP("Failed to write the file with path: %s", FBXComponents_path.c_str());
	outfile.close();

	RELEASE_ARRAY(data);

	return true;
}

bool ModuleSceneImporter::Load(std::string* file_to_load)
{
	if (file_to_load == nullptr)
	{
		LOGP("Loading file was nullptr.");
		return false;
	}

	//Check if this is a mesh file
	std::string extention = file_to_load->substr(file_to_load->rfind(".") + 1, file_to_load->length());

	if (extention == "fbx")
	{
		std::string output_file;
		if (Import(file_to_load, output_file))
		{
			LOGP("Mesh File found and Imported.");
			if (Load(&(Library_mesh_path + "\\" + output_file)))
				LOGP("Mesh File loaded.");
		}
		LOGP("Mesh File found andimport error.");
	}
	else if (extention == Mesh_Extention)
	{
		GameObject* NewGameObject = new GameObject("NewMesh", true, true);
		ComponentMesh* NewMesh = NewGameObject->CreateMeshComponent(true);
		MeshImporter->Load(*NewGameObject->GetTransform(), NewMesh->MeshDataStruct, file_to_load);
		NewGameObject->name = NewMesh->MeshDataStruct.Mesh_name;
		ComponentMaterial* NewMaterial = NewGameObject->CreateMaterialComponent(true);
		MaterialImporter->Load(NewMaterial->MaterialDataStruct, &(Library_material_path + "\\" + NewMesh->MeshDataStruct.Asociated_texture_name));
		App->scene->AddChildToRoot(NewGameObject);
	}
	else if (extention == FBXComponents_Extention)
	{
		LoadFBXComponents(file_to_load);
	}
	else
	{
		LOGP("The dropped file is not a .fbx or .%s or .%s file", Mesh_Extention.c_str(), FBXComponents_Extention.c_str());
		return false;
	}
	return true;
}

bool ModuleSceneImporter::LoadSimpleMesh(std::string* file_to_load, MeshData& Data)
{
	bool ret = true;
	ComponentTransform* transform = new ComponentTransform(nullptr, true);
	ret = MeshImporter->Load(*transform, Data, file_to_load);
	RELEASE(transform);
	return ret;
}

bool ModuleSceneImporter::LoadTexture(std::string* file_to_load, MaterialData& DataMaterial)
{
	bool ret = true;
	ret = MaterialImporter->Load(DataMaterial, file_to_load);
	return ret;
}

bool ModuleSceneImporter::LoadFBXComponents(const std::string* file_to_load)
{
	//Num strings / length of each / strings

	char* data = nullptr;
	std::ifstream file(file_to_load->c_str(), std::ifstream::binary);
	if (file) {
		// get length of file:
		file.seekg(0, file.end);
		int length = file.tellg();
		file.seekg(0, file.beg);
		data = new char[length];
		LOGP("Reading %i characters. File: %s", length, file_to_load->c_str());
		// read data as a block:
		file.read(data, length);
		if (file)
			LOGP("All characters read successfully. File: %s", file_to_load->c_str());
		else
		{
			LOGP("error: only %ll could be read. File: %s", file.gcount(), file_to_load->c_str());
			RELEASE_ARRAY(data);
			return false;
		}
		file.close();
		// ...buffer contains the entire file...
	}
	else
		LOGP("File loading problem. File: %s", file_to_load->c_str());

	if (data == nullptr)
		return false;
	char* cursor = data;
	uint current_loading_size = 0;

	//Load amount_of_strings
	uint amount_of_strings = 0;
	current_loading_size = sizeof(uint);
	memcpy(&amount_of_strings, cursor, current_loading_size);

	std::vector<uint> size_of_each;
	for (uint i = 0; i < amount_of_strings; i++)
	{
		cursor += current_loading_size;
		current_loading_size = sizeof(uint);
		uint size = 0;
		memcpy(&size, cursor, current_loading_size);
		size_of_each.push_back(size);
	}
	
	size_t bar_pos = file_to_load->rfind("\\") + 1;
	std::string gameobject_name = file_to_load->substr(bar_pos, file_to_load->rfind(".") - bar_pos);

	GameObject* NewGameObject = new GameObject(gameobject_name.c_str(), true, true);
	std::string file_string;
	for (uint i = 0; i < amount_of_strings; i++)
	{
		cursor += current_loading_size;
		current_loading_size = sizeof(char) * size_of_each[i];

		GameObject* NewMeshGameObject = new GameObject("NewMesh", true, true);
		ComponentMesh* NewMesh = NewMeshGameObject->CreateMeshComponent(true);
		std::string name = Library_mesh_path + "\\" + cursor;
		MeshImporter->Load(*NewMeshGameObject->GetTransform(), NewMesh->MeshDataStruct, &name);
		NewMeshGameObject->name = NewMesh->MeshDataStruct.Mesh_name;
		ComponentMaterial* NewMaterial = NewMeshGameObject->CreateMaterialComponent(true);
		MaterialImporter->Load(NewMaterial->MaterialDataStruct, &(Library_material_path + "\\" + NewMesh->MeshDataStruct.Asociated_texture_name));

		NewGameObject->AddChild(NewMeshGameObject);
	}
	App->scene->AddChildToRoot(NewGameObject);
	RELEASE_ARRAY(data);
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

const std::string* ModuleSceneImporter::Get_Settings_path() const
{
	return &Settings_path;
}

const std::string* ModuleSceneImporter::Get_Scenes_path() const
{
	return &Scenes_path;
}

const std::string* ModuleSceneImporter::Get_Mesh_Extention() const
{
	return &Mesh_Extention;
}

const std::string* ModuleSceneImporter::Get_FBXComponents_Extention() const
{
	return &FBXComponents_Extention;
}