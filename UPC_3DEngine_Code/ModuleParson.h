#ifndef __ModuleParson_H__
#define __ModuleParson_H__

#include <string>
#include "Globals.h"
#include "parson\parson.h"

class ParsonJSON
{
public:
	ParsonJSON();
	~ParsonJSON();

	bool Init();

	bool Save() const;
	bool Load();

	int GetInt(JSON_Object* conf, const char* field, int default = 0) const;
	uint GetUInt(JSON_Object* conf, const char* field, uint default = 0) const;
	float GetFloat(JSON_Object* conf, const char* field, float default = 0.0f) const;
	double GetDouble(JSON_Object* conf, const char* field, double default = 0.0) const;
	bool GetBool(JSON_Object* conf, const char* field, bool default = false) const;
	const char* GetString(JSON_Object* conf, const char* field, const char* default = "") const;

	bool SetInt(JSON_Object* conf, const char* field, int value);
	bool SetUInt(JSON_Object* conf, const char* field, uint value);
	bool SetFloat(JSON_Object* conf, const char* field, float value);
	bool SetDouble(JSON_Object* conf, const char* field, double value);
	bool SetBool(JSON_Object* conf, const char* field, bool value);
	bool SetString(JSON_Object* conf, const char* field, const char* value);

public:
	std::string file_name;
	JSON_Value* root_value = nullptr;
	JSON_Object* root_object = nullptr;

};

#endif
//__ModuleParson_H__