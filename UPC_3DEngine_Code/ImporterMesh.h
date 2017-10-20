#pragma once

#include "ComponentMesh.h"

class ImporterMesh
{
public:
	ImporterMesh();
	~ImporterMesh();

	bool Init();
	bool CleanUp();

	bool Save(const MeshData& DataMesh, std::string& loaded_file) const;
	bool Load(MeshData& DataMesh, std::string* file_to_load);

private:
	uint imported_meshes = 0;
};