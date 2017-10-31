#ifndef __ModuleParson_H__
#define __ModuleParson_H__

#include <string>
#include "Globals.h"
#include "parson\parson.h"
#include "Color.h"
#include "MathGeoLib\Math.h"

class GameObject;

class ParsonJSON
{
public:
	ParsonJSON(const char* filename, bool isScene = true);
	~ParsonJSON();

	bool Init();

	bool SaveModulesConfig() const;
	bool LoadModulesConfig();

	bool SaveScene(const GameObject* root) const;
	bool LoadScene();

	int GetInt(JSON_Object* conf, const char* field, int default = 0) const;
	uint GetUInt(JSON_Object* conf, const char* field, uint default = 0) const;
	float GetFloat(JSON_Object* conf, const char* field, float default = 0.0f) const;
	double GetDouble(JSON_Object* conf, const char* field, double default = 0.0) const;
	bool GetBool(JSON_Object* conf, const char* field, bool default = false) const;
	const char* GetString(JSON_Object* conf, const char* field, const char* default = "") const;
	float2 GetFloat2(JSON_Object* conf, const char* field, float2 default = float2::zero) const;
	float3 GetFloat3(JSON_Object* conf, const char* field, float3 default = float3::zero) const;
	float4x4 GetFloat4x4(JSON_Object* conf, const char* field, float4x4 default = float4x4::identity) const;
	Quat GetQuat(JSON_Object* conf, const char* field, Quat default = Quat::identity) const;
	Color GetColor(JSON_Object* conf, const char* field, Color default = White) const;

	bool SetInt(JSON_Object* conf, const char* field, int value);
	bool SetUInt(JSON_Object* conf, const char* field, uint value);
	bool SetFloat(JSON_Object* conf, const char* field, float value);
	bool SetDouble(JSON_Object* conf, const char* field, double value);
	bool SetBool(JSON_Object* conf, const char* field, bool value);
	bool SetString(JSON_Object* conf, const char* field, const char* value);
	bool SetFloat2(JSON_Object* conf, const char* field, float2 value);
	bool SetFloat3(JSON_Object* conf, const char* field, float3 value);
	bool SetFloat4x4(JSON_Object* conf, const char* field, float4x4 value);
	bool SetQuat(JSON_Object* conf, const char* field, Quat value);
	bool SetColor(JSON_Object* conf, const char* field, Color color);

public:
	std::string file_name;
	JSON_Value* root_value = nullptr;
	JSON_Object* root_object = nullptr;
	//JSON_Array* array = nullptr;

private:
	bool isScene = false;
};

#endif
//__ModuleParson_H__