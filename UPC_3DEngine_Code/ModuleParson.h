#ifndef __ModuleParson_H__
#define __ModuleParson_H__

#include <string>

#include "parson\parson.h"

class ParsonJSON
{
public:
	ParsonJSON();
	~ParsonJSON();

	bool Init();

	bool Save() const;
	bool Load();

public:
	std::string file_name;
	JSON_Value* root_value = nullptr;
	JSON_Object* root_object = nullptr;

};

#endif
//__ModuleParson_H__