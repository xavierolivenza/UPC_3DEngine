#include <fstream>

#include "ImporterMesh.h"

#include "ComponentMesh.h"

#include "Assimp/include/Logger.hpp"
#include "Assimp/include/LogStream.hpp"
#include "Assimp/include/DefaultLogger.hpp"

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

bool ImporterMesh::Save(const MeshData& DataMesh, std::string& loaded_file) const
{
	//Serialize MeshData to file
	// amount of num_faces / vertices / indices / normals / texture_coords / colors / Asociated Texture Name.dds / SpherePosition / SphereRadius  / BoundBoxMinPoint / BoundBoxMaxPoint / BoundOBox(WIP)
	
	//Check if the file exists

	std::string mesh_name = "\\" + DataMesh.Mesh_name + ".meshalvoli";
	
	LOGP("Importing process start, to file: %s", mesh_name.c_str());

	std::string mesh_path = *App->importer->Get_Library_mesh_path() + mesh_name;
	FILE* file = fopen(mesh_path.c_str(), "r");
	if (file != nullptr)
	{
		fclose(file);
		LOGP("File already exists: %s", mesh_path.c_str());
		return false;
	}

	//Clac size of the file
	uint normals_amount = 0;
	if(DataMesh.normals != nullptr)
		normals_amount = DataMesh.num_vertices;
	uint texture_coords_amount = 0;
	if (DataMesh.texture_coords != nullptr)
		texture_coords_amount = DataMesh.num_vertices;
	uint colors_amount = 0;
	if (DataMesh.colors != nullptr)
		colors_amount = DataMesh.num_vertices;

	size_t bar_pos = DataMesh.Asociated_texture_name.rfind("\\") + 1;
	std::string tex_name = DataMesh.Asociated_texture_name.substr(bar_pos, DataMesh.Asociated_texture_name.rfind(".") - bar_pos);
	tex_name += ".dds";

	uint amount_of_each[6] =
	{
		DataMesh.num_vertices,
		DataMesh.num_indices,
		normals_amount,
		texture_coords_amount,
		colors_amount,
		(uint)tex_name.length() + 1
	};

	uint file_size = sizeof(amount_of_each);
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

	//Store numfaces
	current_allocation_size = sizeof(uint);
	memcpy(cursor, &DataMesh.num_faces, current_allocation_size);

	//Store amount_of_each
	cursor += current_allocation_size;
	current_allocation_size = sizeof(amount_of_each);
	memcpy(cursor, amount_of_each, current_allocation_size);

	// Store vertices
	cursor += current_allocation_size;
	current_allocation_size = sizeof(float) * DataMesh.num_vertices * 3;
	memcpy(cursor, DataMesh.vertices, current_allocation_size);

	// Store indices
	cursor += current_allocation_size;
	current_allocation_size = sizeof(uint) * DataMesh.num_indices;
	memcpy(cursor, DataMesh.indices, current_allocation_size);

	// Normals, texture_coords, colors use this size.
	current_allocation_size = sizeof(float) * DataMesh.num_vertices * 3;

	// Store normals
	if (DataMesh.normals != nullptr)
	{
		cursor += current_allocation_size;
		memcpy(cursor, DataMesh.normals, current_allocation_size);
	}

	// Store texture_coords
	if (DataMesh.texture_coords != nullptr)
	{
		cursor += current_allocation_size;
		memcpy(cursor, DataMesh.texture_coords, current_allocation_size);
	}

	// Store colors
	if (DataMesh.colors != nullptr)
	{
		cursor += current_allocation_size;
		memcpy(cursor, DataMesh.colors, current_allocation_size);
	}

	// Asociated Texture Name.dds
	cursor += current_allocation_size;
	current_allocation_size = sizeof(char) * (tex_name.length() + 1);
	memcpy(cursor, tex_name.c_str(), current_allocation_size);
	
	// SpherePosition, SphereRadius, BoundBoxMinPoint, BoundBoxMaxPoint use this size.
	current_allocation_size = sizeof(float);

	// Store SpherePosition
	cursor += current_allocation_size;
	memcpy(cursor, &DataMesh.BoundSphere.pos.x, current_allocation_size);
	cursor += current_allocation_size;
	memcpy(cursor, &DataMesh.BoundSphere.pos.y, current_allocation_size);
	cursor += current_allocation_size;
	memcpy(cursor, &DataMesh.BoundSphere.pos.z, current_allocation_size);

	// Store SphereRadius
	cursor += current_allocation_size;
	memcpy(cursor, &DataMesh.BoundSphere.r, current_allocation_size);

	// Store BoundBoxMinPoint
	cursor += current_allocation_size;
	memcpy(cursor, &DataMesh.BoundBox.minPoint.x, current_allocation_size);
	cursor += current_allocation_size;
	memcpy(cursor, &DataMesh.BoundBox.minPoint.y, current_allocation_size);
	cursor += current_allocation_size;
	memcpy(cursor, &DataMesh.BoundBox.minPoint.z, current_allocation_size);

	// Store BoundBoxMaxPoint
	cursor += current_allocation_size;
	memcpy(cursor, &DataMesh.BoundBox.maxPoint.x, current_allocation_size);
	cursor += current_allocation_size;
	memcpy(cursor, &DataMesh.BoundBox.maxPoint.y, current_allocation_size);
	cursor += current_allocation_size;
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

bool ImporterMesh::Load(MeshData& DataMesh, std::string* file_to_load)
{
	return false;
}