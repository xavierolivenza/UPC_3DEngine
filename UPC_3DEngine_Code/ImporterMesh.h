#pragma once
#include "Importer.h"

class ImporterMesh : public Importer
{
public:
	ImporterMesh();
	~ImporterMesh();

	bool Import(std::string* file_to_import, std::string& output_file);
	Component* Load(std::string* file_to_load);
};