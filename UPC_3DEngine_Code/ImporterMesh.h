#pragma once

#include "ComponentMesh.h"

class ImporterMesh
{
public:
	ImporterMesh();
	~ImporterMesh();

	bool Init();
	bool CleanUp();

	bool Save(const MeshData* DataMesh, std::string* file_to_save);
	bool Load(MeshData& DataMesh, std::string* file_to_load);
};