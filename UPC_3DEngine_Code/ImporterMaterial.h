#pragma once

#include "ComponentMaterial.h"

class ImporterMaterial
{
public:
	ImporterMaterial();
	~ImporterMaterial();

	bool Init();
	bool CleanUp();

	bool Save(const std::string* texture_name, std::string* file_to_save);
	bool Load(MaterialData& DataMaterial, std::string* file_to_load);
};