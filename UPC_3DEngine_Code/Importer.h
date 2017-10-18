#pragma once
#include "Component.h"

class Importer
{
public:
	Importer();
	~Importer();

	virtual bool Import(std::string* file_to_import, std::string& output_file);
	virtual Component* Load(std::string* file_to_load);
};