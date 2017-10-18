#pragma once
#include "Importer.h"

class ImporterMaterial : public Importer
{
public:
	ImporterMaterial();
	~ImporterMaterial();

	bool Import(std::string* file_to_import, std::string& output_file);
	Component* Load(std::string* file_to_load);
};