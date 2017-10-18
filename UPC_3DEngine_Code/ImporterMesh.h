#pragma once
#include "Importer.h"

class ImporterMesh : public Importer
{
public:
	ImporterMesh();
	~ImporterMesh();

	bool Init();
	bool CleanUp();

	bool Import(std::string* file_to_import, std::string& output_file);
	bool Save(const Component* component, std::string* file_to_save);
	Component* Load(std::string* file_to_load);
};