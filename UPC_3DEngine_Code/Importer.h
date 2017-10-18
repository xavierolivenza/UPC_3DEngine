#pragma once
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

class Importer
{
public:
	Importer();
	~Importer();

	virtual bool Init();
	virtual bool CleanUp();

	virtual bool Import(std::string* file_to_import, std::string& output_file);
	virtual bool Save(const Component* component, std::string* file_to_load);
	virtual Component* Load(std::string* file_to_load);
};