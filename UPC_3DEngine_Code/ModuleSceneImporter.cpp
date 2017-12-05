#include <experimental\filesystem>
#include <fstream>

#include "ModuleSceneImporter.h"
#include "ImporterMesh.h"
#include "ImporterMaterial.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"

#include "ResourceMesh.h"
#include "ResourceTexture.h"

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

	if (App->window->GetLibraryHidden())
		SetFileAttributes(Library_path.c_str(), FILE_ATTRIBUTE_HIDDEN);
	else
		SetFileAttributes(Library_path.c_str(), FILE_ATTRIBUTE_NORMAL);

	//Iterate all Assets folder including files and directories
	for (auto& file_in_path : std::experimental::filesystem::recursive_directory_iterator(Assets_path.c_str()))
	{
		LOGP("%S", file_in_path.path().c_str());
		if (std::experimental::filesystem::is_regular_file(file_in_path.path()))
		{
			std::string output_file;
			if (App->resources->ImportFile(file_in_path.path().string().c_str())) LOGP("Regular File found and imported.");
			LOGP("Regular File found and import error.");
			/*
			std::string output_file;
			if(ImportFBX(&file_in_path.path().string(), output_file)) LOGP("Regular File found and imported.");
			LOGP("Regular File found and import error.");
			*/
		}
	}

	CheckAssetsImport.Start();

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
	{
		size_t bar_pos = DroppedFile->rfind("\\") + 1;
		std::experimental::filesystem::path path_origin = *DroppedFile;
		std::experimental::filesystem::path path_destination = *App->importer->Get_Assets_path() + "\\" + DroppedFile->substr(bar_pos, DroppedFile->size());
		std::experimental::filesystem::copy_file(path_origin, path_destination, std::experimental::filesystem::copy_options::overwrite_existing);
		if (App->resources->ImportFile(path_destination.string().c_str(), true)) LOGP("Dropped File found and imported.");
		LOGP("Dropped File found and import error.");
		Load(&path_destination.string());
	}
	return UPDATE_CONTINUE;
}

update_status ModuleSceneImporter::PostUpdate(float dt)
{
	//Assume all asets are in assets directory from begining of execution
	if((App->GetEngineTimeStatus() == EngineTimeStatus::stop) && (CheckAssetsImport.Read() >= miliseconds_check))
	{
		/**/
		//Iterate all Assets folder including files and directories
		char title[1000] = "";
		for (std::experimental::filesystem::recursive_directory_iterator::value_type file_in_path : std::experimental::filesystem::recursive_directory_iterator(Assets_path.c_str()))
		{
			//LOGP("%S", file_in_path.path().c_str());
			//LOGP("%S", file_in_path.path().extension().c_str());
			if (std::experimental::filesystem::is_regular_file(file_in_path.path()) && (file_in_path.path().extension() != ".json") && (file_in_path.path().string().length() < 1000))
			{
				sprintf_s(title, 1000, "%S", file_in_path.path().c_str());
				Resource* res = App->resources->GetResource(title);
				/**/
				if (res != nullptr)
				{
					/**/
					//Reimport
					std::experimental::filesystem::file_time_type ftime = std::experimental::filesystem::last_write_time(file_in_path.path());
					std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
					if (res->file_date != std::asctime(std::localtime(&cftime)))
					{
						App->resources->ReimportResource(*res);
						res->file_date = std::asctime(std::localtime(&cftime));
					}
					/**/
				}
				/*
				else
				{
					//Import
					std::string output_file;
					App->resources->ImportFile(file_in_path.path().string().c_str());
					//if (App->resources->ImportFile(file_in_path.path().string().c_str())) LOGP("Regular File found and imported.");
					//LOGP("Regular File found and import error.");
				}
				*/
				/**/
			}
		}
		/**/
		//LOGP("Assets check.");
		CheckAssetsImport.Start();
	}
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

bool ModuleSceneImporter::ImportFBX(std::string* file_to_import, std::string& output_file, bool Reimporting)
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
					NodesVector.clear();
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
				uint* indices = new uint[MeshDataStruct.num_indices];
				for (uint i = 0; i < MeshInstance->mNumFaces; ++i)
				{
					if (MeshInstance->mFaces[i].mNumIndices != 3)
					{
						MeshDataStruct.num_indices -= MeshInstance->mFaces[i].mNumIndices;
						LOGP("WARNING, geometry face with != 3 indices!");
					}
					else
						memcpy(&indices[i * 3], MeshInstance->mFaces[i].mIndices, 3 * sizeof(uint));
				}
				MeshDataStruct.indices = new uint[MeshDataStruct.num_indices];
				memcpy(MeshDataStruct.indices, indices, sizeof(uint) * MeshDataStruct.num_indices);
				RELEASE_ARRAY(indices);
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
				MeshDataStruct.Asociated_texture_OriginalPath = WorkingPath + material_path.C_Str();
				MaterialImporter->Save(&MeshDataStruct.Asociated_texture_OriginalPath, output, Reimporting);
			}

			//------------------------------------------//
			//-------Serialize Mesh To Own Format-------//
			//------------------------------------------//
			std::string output;
			MeshImporter->Save(pos, scale, rot, MeshDataStruct, output, Reimporting);
			fbx_MeshComponents.push_back(output);
		}

		//Save fbx_MeshComponents
		ImportFBXComponents(scene, file_to_import, &fbx_MeshComponents, output_file);

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

bool ModuleSceneImporter::ImportFBXComponents(const aiScene* scene, const std::string* file_to_import, const std::vector<std::string>* FBXComponents, std::string& output_file)
{
	//Num nodes / size of each(only string) / nodes(pos, rot, scale, own format file string)

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
	std::list<AssimpNodeTransOwnFile> AssimpNodeList;
	std::list<const aiNode*> AllFBXNodes;
	RecursiveIterateFBX(scene->mRootNode, AllFBXNodes);
	for (std::list<const aiNode*>::const_iterator item = AllFBXNodes.cbegin(); item != AllFBXNodes.cend(); ++item)
	{
		if (item._Ptr->_Myval->mNumMeshes > 0)
		{
			AssimpNodeTransOwnFile AssimpNodeTransOwnFileStruct;
			//Sum up all transformations from root node to the node where the mesh is stored
			aiMatrix4x4 transform;
			std::vector<const aiNode*> NodesVector;
			NodesVector.clear();
			for (const aiNode* iterator = item._Ptr->_Myval; iterator->mParent != nullptr; iterator = iterator->mParent)
				NodesVector.push_back(iterator);
			for (std::vector<const aiNode*>::reverse_iterator item = NodesVector.rbegin(); item != NodesVector.crend(); ++item)
				transform *= (*item)->mTransformation;
			aiVector3D translation;
			aiVector3D scaling;
			aiQuaternion rotation;
			transform.Decompose(scaling, rotation, translation);
			AssimpNodeTransOwnFileStruct.pos = { translation.x, translation.y, translation.z };
			AssimpNodeTransOwnFileStruct.scale = { scaling.x, scaling.y, scaling.z };
			AssimpNodeTransOwnFileStruct.rot = { rotation.x, rotation.y, rotation.z, rotation.w };
			//Now we have the transform of this node
			//Now we can evaluate the node meshes
			for (int i = 0; i < item._Ptr->_Myval->mNumMeshes; i++)
			{
				//With this uint is we can do this: FBXComponents[item._Ptr->_Myval->mMeshes[i]] to get this node mesh ownformat file
				AssimpNodeTransOwnFileStruct.mesh = item._Ptr->_Myval->mMeshes[i];
				AssimpNodeList.push_back(AssimpNodeTransOwnFileStruct);
			}
		}
	}

	//Now we have all nodes + what mesh they use + their transform, we can serialize to file

	//Calc file size
	//original file size
	uint file_size = sizeof(uint);
	//original path
	file_size += sizeof(char) * (file_to_import->length() + 1);
	//Num nodes
	file_size += sizeof(uint);
	//size of each (only string)
	file_size += sizeof(uint) * AssimpNodeList.size();
	//Size of each + nodes(pos, rot, scale, own format file string)
	for (std::list<AssimpNodeTransOwnFile>::const_iterator item = AssimpNodeList.cbegin(); item != AssimpNodeList.cend(); ++item)
	{
		file_size += sizeof(float) * 10; //3pos, 3 scale, 4 rot
		file_size += sizeof(char) * (FBXComponents->at(item._Ptr->_Myval.mesh).length() + 1);
	}

	//Serialize
	//Create char* to allocate data and another char* to move around the previous one
	char* data = new char[file_size];
	char* cursor = data;
	uint current_allocation_size = 0;

	//original file size
	current_allocation_size = sizeof(uint);
	uint originlen = file_to_import->length() + 1;
	memcpy(cursor, &originlen, current_allocation_size);

	//original path
	cursor += current_allocation_size;
	current_allocation_size = file_to_import->length() + 1;
	memcpy(cursor, file_to_import->c_str(), current_allocation_size);

	//Num nodes
	cursor += current_allocation_size;
	current_allocation_size = sizeof(uint);
	uint numnodes = AssimpNodeList.size();
	memcpy(cursor, &numnodes, current_allocation_size);

	// size of each (only string)
	for (std::list<AssimpNodeTransOwnFile>::const_iterator item = AssimpNodeList.cbegin(); item != AssimpNodeList.cend(); ++item)
	{
		cursor += current_allocation_size;
		current_allocation_size = sizeof(uint);
		uint leng = FBXComponents->at(item._Ptr->_Myval.mesh).length() + 1;
		memcpy(cursor, &leng, current_allocation_size);
	}

	//nodes(pos, rot, scale, own format file string)
	for (std::list<AssimpNodeTransOwnFile>::const_iterator item = AssimpNodeList.cbegin(); item != AssimpNodeList.cend(); ++item)
	{
		cursor += current_allocation_size;
		current_allocation_size = sizeof(float);
		memcpy(cursor, &item._Ptr->_Myval.pos.x, current_allocation_size);
		cursor += current_allocation_size;
		memcpy(cursor, &item._Ptr->_Myval.pos.y, current_allocation_size);
		cursor += current_allocation_size;
		memcpy(cursor, &item._Ptr->_Myval.pos.z, current_allocation_size);

		cursor += current_allocation_size;
		memcpy(cursor, &item._Ptr->_Myval.rot.x, current_allocation_size);
		cursor += current_allocation_size;
		memcpy(cursor, &item._Ptr->_Myval.rot.y, current_allocation_size);
		cursor += current_allocation_size;
		memcpy(cursor, &item._Ptr->_Myval.rot.z, current_allocation_size);
		cursor += current_allocation_size;
		memcpy(cursor, &item._Ptr->_Myval.rot.w, current_allocation_size);

		cursor += current_allocation_size;
		memcpy(cursor, &item._Ptr->_Myval.scale.x, current_allocation_size);
		cursor += current_allocation_size;
		memcpy(cursor, &item._Ptr->_Myval.scale.y, current_allocation_size);
		cursor += current_allocation_size;
		memcpy(cursor, &item._Ptr->_Myval.scale.z, current_allocation_size);

		cursor += current_allocation_size;
		current_allocation_size = sizeof(char) * (FBXComponents->at(item._Ptr->_Myval.mesh).length() + 1);
		memcpy(cursor, FBXComponents->at(item._Ptr->_Myval.mesh).c_str(), current_allocation_size);
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

	if ((extention == "fbx") || (extention == "FBX") || (extention == "obj") || (extention == "OBJ"))
	{
		std::string output_file;
		if (ImportFBX(file_to_load, output_file))
		{
			LOGP("Mesh File found and Imported.");
			if (Load(&(Library_mesh_path + "\\" + output_file)))
				LOGP("Mesh File loaded.");
		}
		else
			LOGP("Mesh File found and import error.");
	}
	else if (extention == Mesh_Extention)
	{
		//Create GameObject
		GameObject* NewGameObject = new GameObject("NewMesh", true, true);
		//Load resource
		uint uuid = App->resources->LoadResource(file_to_load->c_str());
		//Vinculate resource with component
		ComponentMesh* NewMesh = NewGameObject->CreateMeshComponent(true);
		NewMesh->SetResource(uuid);
		NewGameObject->name = NewMesh->resourceMesh->SimpleMeshDataStruct.Mesh_name;
		//Load resource
		uuid = App->resources->LoadResource((Library_material_path + "\\" + NewMesh->resourceMesh->SimpleMeshDataStruct.Asociated_texture_name).c_str());
		//Vinculate resource with component
		ComponentMaterial* NewMaterial = NewGameObject->CreateMaterialComponent(true);
		NewMaterial->SetResource(uuid);
		App->scene->AddChildToRoot(NewGameObject);
	}
	else if (extention == FBXComponents_Extention)
	{
		LoadFBXComponents(file_to_load); //Insice here do the same as in lines 508-518 of this file
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
	//Num nodes / size of each(only string) / nodes(pos, rot, scale, own format file string)
	
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

	//original file size
	uint originfileSize = 0;
	current_loading_size = sizeof(uint);
	memcpy(&originfileSize, cursor, current_loading_size);

	//original path
	cursor += current_loading_size;
	std::string originalPath = cursor;
	current_loading_size = originalPath.length() + 1;

	//Load amount of nodes
	cursor += current_loading_size;
	uint amount_of_nodes = 0;
	current_loading_size = sizeof(uint);
	memcpy(&amount_of_nodes, cursor, current_loading_size);

	//Load node str size
	std::vector<uint> size_of_each;
	for (uint i = 0; i < amount_of_nodes; i++)
	{
		cursor += current_loading_size;
		current_loading_size = sizeof(uint);
		uint size = 0;
		memcpy(&size, cursor, current_loading_size);
		size_of_each.push_back(size);
	}

	//nodes(pos, rot, scale, own format file string)
	size_t bar_pos = file_to_load->rfind("\\") + 1;
	std::string gameobject_name = file_to_load->substr(bar_pos, file_to_load->rfind(".") - bar_pos);

	GameObject* NewGameObject = new GameObject(gameobject_name.c_str(), true, true);
	std::string file_string;
	for (uint i = 0; i < amount_of_nodes; i++)
	{
		//Create GameObject
		GameObject* NewMeshGameObject = new GameObject("NewMesh", true, true);

		float3 position = float3::zero;
		cursor += current_loading_size;
		current_loading_size = sizeof(float);
		memcpy(&position.x, cursor, current_loading_size);
		cursor += current_loading_size;
		current_loading_size = sizeof(float);
		memcpy(&position.y, cursor, current_loading_size);
		cursor += current_loading_size;
		current_loading_size = sizeof(float);
		memcpy(&position.z, cursor, current_loading_size);
		NewMeshGameObject->GetTransform()->SetPos(position);

		Quat rotation = Quat::identity;
		cursor += current_loading_size;
		current_loading_size = sizeof(float);
		memcpy(&rotation.x, cursor, current_loading_size);
		cursor += current_loading_size;
		current_loading_size = sizeof(float);
		memcpy(&rotation.y, cursor, current_loading_size);
		cursor += current_loading_size;
		current_loading_size = sizeof(float);
		memcpy(&rotation.z, cursor, current_loading_size);
		cursor += current_loading_size;
		current_loading_size = sizeof(float);
		memcpy(&rotation.w, cursor, current_loading_size);
		NewMeshGameObject->GetTransform()->SetRot(rotation);

		float3 scale = float3::zero;
		cursor += current_loading_size;
		current_loading_size = sizeof(float);
		memcpy(&scale.x, cursor, current_loading_size);
		cursor += current_loading_size;
		current_loading_size = sizeof(float);
		memcpy(&scale.y, cursor, current_loading_size);
		cursor += current_loading_size;
		current_loading_size = sizeof(float);
		memcpy(&scale.z, cursor, current_loading_size);
		NewMeshGameObject->GetTransform()->SetScale(scale);

		cursor += current_loading_size;

		std::string name = cursor;
		current_loading_size = name.size() + 1;
		name = Library_mesh_path + "\\" + cursor;
		//Load resource
		uint uuid = App->resources->LoadResource(name.c_str(), originalPath.c_str());
		if (uuid != 0)
		{
			//Vinculate resource with component
			ComponentMesh* NewMesh = NewMeshGameObject->CreateMeshComponent(true);
			NewMesh->SetResource(uuid);
			if(NewMesh->resourceMesh != nullptr)
			{
				NewMeshGameObject->name = NewMesh->resourceMesh->SimpleMeshDataStruct.Mesh_name;
				//Load resource
				uuid = App->resources->LoadResource((Library_material_path + "\\" + NewMesh->resourceMesh->SimpleMeshDataStruct.Asociated_texture_name).c_str(), NewMesh->resourceMesh->SimpleMeshDataStruct.Asociated_texture_OriginalPath.c_str());
				if (uuid != 0)
				{
					//Vinculate resource with component
					ComponentMaterial* NewMaterial = NewMeshGameObject->CreateMaterialComponent(true);
					NewMaterial->SetResource(uuid);
				}
			}
		}
		NewGameObject->AddChild(NewMeshGameObject);
	}
	App->scene->AddChildToRoot(NewGameObject);

	/*
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
	*/
	RELEASE_ARRAY(data);
	return true;
}

void ModuleSceneImporter::RecursiveIterateFBX(const aiNode * node, std::list<const aiNode*>& list)
{
	for (int i = 0; i < node->mNumChildren; i++)
		RecursiveIterateFBX(node->mChildren[i], list);
	list.push_back(node);
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