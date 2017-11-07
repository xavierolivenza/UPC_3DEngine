#pragma once

#include "ComponentMaterial.h"

class ImporterMaterial
{
public:
	ImporterMaterial();
	~ImporterMaterial();

	bool Init();
	bool CleanUp();

	bool Save(const std::string* texture_name, std::string& loaded_file);
	bool Load(MaterialData& DataMaterial, std::string* file_to_load);

	int LoadImageFromFile(MaterialData& DataMaterial, std::string* file_to_load);

	bool DevilCanLoad(const char* file);
};