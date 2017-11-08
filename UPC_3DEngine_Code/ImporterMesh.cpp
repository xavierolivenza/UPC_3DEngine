#include <fstream>

#include "ImporterMesh.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include "Assimp/include/Logger.hpp"
#include "Assimp/include/LogStream.hpp"
#include "Assimp/include/DefaultLogger.hpp"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

ImporterMesh::ImporterMesh()
{
	
}

ImporterMesh::~ImporterMesh()
{
	
}

bool ImporterMesh::Init()
{
	// Create a logger instance 
	Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
	// Now I am ready for logging my stuff
	Assimp::DefaultLogger::get()->info("this is my info-call");
	// Select the kinds of messages you want to receive on this log stream
	const unsigned int severity = Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Err | Assimp::Logger::Warn;
	// Attaching it to the default logger
	Assimp::DefaultLogger::get()->attachStream(new Assimp::myStream(), severity);
	return true;
}

bool ImporterMesh::CleanUp()
{
	// Kill it after the work is done
	Assimp::DefaultLogger::kill();
	return true;
}

bool ImporterMesh::Save(const float3& pos, const float3& scale, const Quat& rot, const MeshData& DataMesh, std::string& loaded_file) const
{
	//Serialize MeshData to file
	// amount of each / mesh name / transform:pos / transform:scale / transform:rot / num_faces / vertices / indices / normals / texture_coords / colors / Asociated Texture Name.dds / SpherePosition / SphereRadius  / BoundBoxMinPoint / BoundBoxMaxPoint / BoundOBox(WIP)
	
	//Check if the file exists

	std::string mesh_name = DataMesh.Mesh_name + "." + *App->importer->Get_Mesh_Extention();
	
	LOGP("Importing mesh process start, to file: %s", mesh_name.c_str());

	std::string mesh_path = *App->importer->Get_Library_mesh_path() + "\\" + mesh_name;
	FILE* file = fopen(mesh_path.c_str(), "r");
	if (file != nullptr)
	{
		fclose(file);
		LOGP("Mesh file already exists: %s", mesh_path.c_str());
		return false;
	}

	size_t bar_pos = DataMesh.Asociated_texture_name.rfind("\\") + 1;
	std::string tex_name = DataMesh.Asociated_texture_name.substr(bar_pos, DataMesh.Asociated_texture_name.rfind(".") - bar_pos);
	tex_name += ".dds";

	//Clac size of the file
	uint normals_amount = 0;
	if (DataMesh.normals != nullptr)
		normals_amount = DataMesh.num_vertices;
	uint texture_coords_amount = 0;
	if (DataMesh.texture_coords != nullptr)
		texture_coords_amount = DataMesh.num_vertices;
	uint colors_amount = 0;
	if (DataMesh.colors != nullptr)
		colors_amount = DataMesh.num_vertices;

	uint amount_of_each[7] =
	{
		DataMesh.num_vertices,
		DataMesh.num_indices,
		normals_amount,
		texture_coords_amount,
		colors_amount,
		(uint)DataMesh.Mesh_name.length() + 1,
		(uint)tex_name.length() + 1
	};

	uint file_size = sizeof(amount_of_each);
	file_size += sizeof(char) * (DataMesh.Mesh_name.length() + 1);// Mesh Name
	file_size += sizeof(float) * 3;								// transform:pos
	file_size += sizeof(float) * 3;								// transform:scale 
	file_size += sizeof(float) * 4;								// transform:rot	//Quaternion 4 float
	file_size += sizeof(uint);									// num_faces
	file_size += sizeof(float) * DataMesh.num_vertices * 3;		// vertices
	file_size += sizeof(uint) * DataMesh.num_indices;			// indices
	file_size += sizeof(float) * DataMesh.num_vertices * 3;		// normals
	file_size += sizeof(float) * DataMesh.num_vertices * 3;		// texture_coords
	file_size += sizeof(float) * DataMesh.num_vertices * 3;		// colors
	file_size += sizeof(char) * (tex_name.length() + 1);		// Asociated Texture Name.dds
	file_size += sizeof(float) * 3;								// SpherePosition
	file_size += sizeof(float);									// SpherePosition
	file_size += sizeof(float) * 3;								// BoundBoxMinPoint
	file_size += sizeof(float) * 3;								// BoundBoxMaxPoint
	//file_size += sizeof(OBB);									// BoundOBox(WIP)

	//Create char* to allocate data and another char* to move around the previous one
	char* data = new char[file_size];
	char* cursor = data;
	uint current_allocation_size = 0;

	//Store amount_of_each
	current_allocation_size = sizeof(amount_of_each);
	memcpy(cursor, amount_of_each, current_allocation_size);

	// Mesh Name
	cursor += current_allocation_size;
	current_allocation_size = sizeof(char) * (DataMesh.Mesh_name.length() + 1);
	memcpy(cursor, DataMesh.Mesh_name.c_str(), current_allocation_size);

	//Store transform:pos
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &pos.x, current_allocation_size);
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &pos.y, current_allocation_size);
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &pos.z, current_allocation_size);

	//Store transform:scale
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &scale.x, current_allocation_size);
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &scale.y, current_allocation_size);
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &scale.z, current_allocation_size);

	//Store transform:rot
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &rot.x, current_allocation_size);
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &rot.y, current_allocation_size);
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &rot.z, current_allocation_size);
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &rot.w, current_allocation_size);

	//Store numfaces
	cursor += current_allocation_size;
	current_allocation_size = sizeof(uint);
	memcpy(cursor, &DataMesh.num_faces, current_allocation_size);

	// Store vertices
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float) * DataMesh.num_vertices * 3;
	memcpy(cursor, DataMesh.vertices, current_allocation_size);

	// Store indices
	cursor += current_allocation_size;
	current_allocation_size = sizeof(uint) * DataMesh.num_indices;
	memcpy(cursor, DataMesh.indices, current_allocation_size);

	// Store normals
	if (DataMesh.normals != nullptr)
	{
		cursor += current_allocation_size;
		current_allocation_size = sizeof(float) * DataMesh.num_vertices * 3;
		memcpy(cursor, DataMesh.normals, current_allocation_size);
	}

	// Store texture_coords
	if (DataMesh.texture_coords != nullptr)
	{
		cursor += current_allocation_size;
		current_allocation_size = sizeof(float) * DataMesh.num_vertices * 3;
		memcpy(cursor, DataMesh.texture_coords, current_allocation_size);
	}

	// Store colors
	if (DataMesh.colors != nullptr)
	{
		cursor += current_allocation_size;
		current_allocation_size = sizeof(float) * DataMesh.num_vertices * 3;
		memcpy(cursor, DataMesh.colors, current_allocation_size);
	}

	// Asociated Texture Name.dds
	cursor += current_allocation_size;
	current_allocation_size = sizeof(char) * (tex_name.length() + 1);
	memcpy(cursor, tex_name.c_str(), current_allocation_size);
	
	// Store SpherePosition
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &DataMesh.BoundSphere.pos.x, current_allocation_size);
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &DataMesh.BoundSphere.pos.y, current_allocation_size);
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &DataMesh.BoundSphere.pos.z, current_allocation_size);

	// Store SphereRadius
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &DataMesh.BoundSphere.r, current_allocation_size);

	// Store BoundBoxMinPoint
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &DataMesh.BoundBox.minPoint.x, current_allocation_size);
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &DataMesh.BoundBox.minPoint.y, current_allocation_size);
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &DataMesh.BoundBox.minPoint.z, current_allocation_size);

	// Store BoundBoxMaxPoint
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &DataMesh.BoundBox.maxPoint.x, current_allocation_size);
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &DataMesh.BoundBox.maxPoint.y, current_allocation_size);
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float);
	memcpy(cursor, &DataMesh.BoundBox.maxPoint.z, current_allocation_size);
	
	// Store BoundOBox(WIP)

	//Write to file
	std::ofstream outfile(mesh_path, std::ofstream::binary);
	if (outfile.good()) //write file
		outfile.write(data, file_size);
	else
		LOGP("Failed to write the file with path: %s", mesh_path.c_str());
	outfile.close();

	RELEASE_ARRAY(data);

	LOGP("Importing process ended, to file: %s", mesh_name.c_str());
	loaded_file = mesh_name;

	return true;
}

bool ImporterMesh::Load(ComponentTransform& transform, MeshData& DataMesh, const std::string* file_to_load)
{
	//Get serialized MeshData from file
	// amount of each / mesh name / transform:pos / transform:scale / transform:rot / num_faces / vertices / indices / normals / texture_coords / colors / Asociated Texture Name.dds / SpherePosition / SphereRadius  / BoundBoxMinPoint / BoundBoxMaxPoint / BoundOBox(WIP)

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

	size_t bar_pos = file_to_load->rfind("\\") + 1;
	DataMesh.Mesh_File = file_to_load->substr(bar_pos, file_to_load->length());

	//Load amount_of_each
	uint amount_of_each[7];
	current_loading_size = sizeof(amount_of_each);
	memcpy(amount_of_each, cursor, current_loading_size);
	DataMesh.num_vertices = amount_of_each[0];
	DataMesh.num_indices = amount_of_each[1];
	uint Normals_amount = amount_of_each[2];
	uint Texture_Coords_amount = amount_of_each[3];
	uint Colors_amount = amount_of_each[4];
	uint Mesh_name_size = amount_of_each[5];
	uint Tex_name_size = amount_of_each[6];

	// Mesh Name
	cursor += current_loading_size;
	current_loading_size = sizeof(char) * Mesh_name_size;
	DataMesh.Mesh_name = cursor;

	//Load transform:pos
	float3 pos = float3::zero;
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&pos.x, cursor, current_loading_size);
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&pos.y, cursor, current_loading_size);
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&pos.z, cursor, current_loading_size);
	transform.SetPos(pos);

	//Load transform:scale
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
	transform.SetScale(scale);

	//Load transform:rot
	Quat rot = Quat::identity;
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&rot.x, cursor, current_loading_size);
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&rot.y, cursor, current_loading_size);
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&rot.z, cursor, current_loading_size);
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&rot.w, cursor, current_loading_size);
	transform.SetRot(rot);

	//Load numfaces
	cursor += current_loading_size;
	current_loading_size = sizeof(uint);
	memcpy(&DataMesh.num_faces, cursor, current_loading_size);

	// Load vertices
	cursor += current_loading_size;
	current_loading_size = sizeof(float) * DataMesh.num_vertices * 3;
	DataMesh.vertices = new float[DataMesh.num_vertices * 3];
	memcpy(DataMesh.vertices, cursor, current_loading_size);

	// Load indices
	cursor += current_loading_size;
	current_loading_size = sizeof(uint) * DataMesh.num_indices;
	DataMesh.indices = new uint[DataMesh.num_indices];
	memcpy(DataMesh.indices, cursor, current_loading_size);

	// Load normals
	if (Normals_amount > 0)
	{
		cursor += current_loading_size;
		current_loading_size = sizeof(float) * DataMesh.num_vertices * 3;
		DataMesh.normals = new float[DataMesh.num_vertices * 3];
		memcpy(DataMesh.normals, cursor, current_loading_size);
	}

	// Load texture_coords
	if (Texture_Coords_amount > 0)
	{
		cursor += current_loading_size;
		current_loading_size = sizeof(float) * DataMesh.num_vertices * 3;
		DataMesh.texture_coords = new float[DataMesh.num_vertices * 3];
		memcpy(DataMesh.texture_coords, cursor, current_loading_size);
	}

	// Load colors
	if (Colors_amount > 0)
	{
		cursor += current_loading_size;
		current_loading_size = sizeof(float) * DataMesh.num_vertices * 3;
		DataMesh.colors = new float[DataMesh.num_vertices * 3];
		memcpy(DataMesh.colors, cursor, current_loading_size);
	}

	// Asociated Texture Name.dds
	cursor += current_loading_size;
	current_loading_size = sizeof(char) * Tex_name_size;
	DataMesh.Asociated_texture_name = cursor;

	//Load SpherePosition
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&DataMesh.BoundSphere.pos.x, cursor, current_loading_size);
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&DataMesh.BoundSphere.pos.y, cursor, current_loading_size);
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&DataMesh.BoundSphere.pos.z, cursor, current_loading_size);

	// Load SphereRadius
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&DataMesh.BoundSphere.r, cursor, current_loading_size);

	//Load BoundBoxMinPoint
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&DataMesh.BoundBox.minPoint.x, cursor, current_loading_size);
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&DataMesh.BoundBox.minPoint.y, cursor, current_loading_size);
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&DataMesh.BoundBox.minPoint.z, cursor, current_loading_size);

	//Load BoundBoxMaxPoint
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&DataMesh.BoundBox.maxPoint.x, cursor, current_loading_size);
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&DataMesh.BoundBox.maxPoint.y, cursor, current_loading_size);
	cursor += current_loading_size;
	current_loading_size = sizeof(float);
	memcpy(&DataMesh.BoundBox.maxPoint.z, cursor, current_loading_size);

	// Load BoundOBox(WIP)

	//Generate Buffers
	// Buffer for vertices
	LOGP("Loading Buffers Vertex.");
	glGenBuffers(1, (GLuint*) &(DataMesh.id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, DataMesh.id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * DataMesh.num_vertices * 3, DataMesh.vertices, GL_STATIC_DRAW);

	// Buffer for indices
	LOGP("Loading Buffers Index.");
	glGenBuffers(1, (GLuint*) &(DataMesh.id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DataMesh.id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * DataMesh.num_indices, DataMesh.indices, GL_STATIC_DRAW);

	// Buffer for normals
	if (DataMesh.normals != nullptr)
	{
		LOGP("Loading Buffers Normals.");
		glGenBuffers(1, (GLuint*) &(DataMesh.id_normals));
		glBindBuffer(GL_ARRAY_BUFFER, DataMesh.id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * DataMesh.num_vertices * 3, DataMesh.normals, GL_STATIC_DRAW);
	}

	// Buffer for texture coords
	if (DataMesh.texture_coords != nullptr)
	{
		LOGP("Loading Buffers Texture Coords.");
		glGenBuffers(1, (GLuint*) &(DataMesh.id_texture_coords));
		glBindBuffer(GL_ARRAY_BUFFER, DataMesh.id_texture_coords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * DataMesh.num_vertices * 3, DataMesh.texture_coords, GL_STATIC_DRAW);
	}

	// Buffer for vertex colors
	if (DataMesh.colors != nullptr)
	{
		LOGP("Loading Buffers Colors.");
		glGenBuffers(1, (GLuint*) &(DataMesh.id_colors));
		glBindBuffer(GL_ARRAY_BUFFER, DataMesh.id_colors);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) *  DataMesh.num_vertices * 3, DataMesh.colors, GL_STATIC_DRAW);
	}

	RELEASE_ARRAY(data);

	return true;
}

bool ImporterMesh::AssimpCanLoad(const char * file)
{
	const aiScene* scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr)
	{
		aiReleaseImport(scene);
		return true;
	}
	return false;
}