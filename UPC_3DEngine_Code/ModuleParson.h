#ifndef __ModuleParson_H__
#define __ModuleParson_H__

#include "Module.h"

class ModuleParson : public Module
{
public:
	ModuleParson(Application* app, bool start_enabled = true);
	~ModuleParson();

	bool Init();
	bool CleanUp();

	bool Save();
	bool Load();

public:
	std::string file_name;
	JSON_Value* root_value = nullptr;
	JSON_Object* root_object = nullptr;

};

#endif
//__ModuleParson_H__