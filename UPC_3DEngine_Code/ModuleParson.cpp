#include "ModuleParson.h"
#include "Module.h"
#include "Application.h"

/*
static int malloc_count;
static void* counted_malloc(size_t size);
static void counted_free(void *ptr);
*/

/*
static void* counted_malloc(size_t size) {
void* res = malloc(size);
if (res != nullptr)
malloc_count++;
return res;
}

static void counted_free(void* ptr) {
if (ptr != nullptr)
malloc_count--;
free(ptr);
}
*/

ParsonJSON::ParsonJSON(const char* filename)
{
	//json_set_allocation_functions(counted_malloc, counted_free);
	root_value = json_value_init_object();
	root_object = json_value_get_object(root_value);
	file_name = *App->importer->Get_Settings_path() + "\\" + filename + ".json";
}

ParsonJSON::~ParsonJSON()
{
	json_value_free(root_value);
}

bool ParsonJSON::Init()
{
	root_value = json_parse_file(file_name.c_str());
	if (root_value == NULL)
	{
		root_value = json_value_init_object();
		json_serialize_to_file(root_value, file_name.c_str());
	}
	else
		root_object = json_value_get_object(root_value);
	
	for (std::list<Module*>::const_reverse_iterator item = App->GetModuleList()->rbegin(); item != App->GetModuleList()->crend(); ++item)
		if (json_object_get_object(root_object, (*item)->name.c_str()) == NULL)
			json_object_set_value(root_object, (*item)->name.c_str(), json_value_init_object());
	return true;
}

bool ParsonJSON::SaveModulesConfig() const
{
	JSON_Object* conf = nullptr;
	for (std::list<Module*>::const_reverse_iterator item = App->GetModuleList()->rbegin(); item != App->GetModuleList()->crend(); ++item)
	{
		conf = json_object_get_object(root_object, (*item)->name.c_str());
		(*item)->SaveConf(conf);
	}
	json_serialize_to_string_pretty(root_value);
	json_serialize_to_file(root_value, file_name.c_str());
	return true;
}

bool ParsonJSON::LoadModulesConfig()
{
	JSON_Object* conf = nullptr;
	for (std::list<Module*>::const_reverse_iterator item = App->GetModuleList()->rbegin(); item != App->GetModuleList()->crend(); ++item)
	{
		conf = json_object_get_object(root_object, (*item)->name.c_str());
		(*item)->LoadConf(conf);
	}
	return true;
}

bool ParsonJSON::SaveScene(const char* filename) const
{
	return true;
}

bool ParsonJSON::LoadScene(const char* filename)
{
	return true;
}

int ParsonJSON::GetInt(JSON_Object* conf, const char* field, int default) const
{
	JSON_Value* value = json_object_get_value(conf, field);
	if (value && (json_value_get_type(value) == JSONNumber))
		return (int)json_value_get_number(value);
	return default;
}

uint ParsonJSON::GetUInt(JSON_Object* conf, const char* field, uint default) const
{
	JSON_Value* value = json_object_get_value(conf, field);
	if (value && (json_value_get_type(value) == JSONNumber))
		return (uint)json_value_get_number(value);
	return default;
}

float ParsonJSON::GetFloat(JSON_Object* conf, const char* field, float default) const
{
	JSON_Value* value = json_object_get_value(conf, field);
	if (value && (json_value_get_type(value) == JSONNumber))
		return (float)json_value_get_number(value);
	return default;
}

double ParsonJSON::GetDouble(JSON_Object* conf, const char* field, double default) const
{
	JSON_Value* value = json_object_get_value(conf, field);
	if (value && (json_value_get_type(value) == JSONNumber))
		return json_value_get_number(value);
	return default;
}

bool ParsonJSON::GetBool(JSON_Object* conf, const char* field, bool default) const
{
	JSON_Value* value = json_object_get_value(conf, field);
	if (value && (json_value_get_type(value) == JSONNumber))
		return json_value_get_boolean(value);
	return default;
}

const char* ParsonJSON::GetString(JSON_Object* conf, const char* field, const char* default) const
{
	JSON_Value* value = json_object_get_value(conf, field);
	if (value && (json_value_get_type(value) == JSONString))
		return json_value_get_string(value);
	return default;
}

float2 ParsonJSON::GetFloat2(JSON_Object* conf, const char* field, float2 default) const
{
	float2 ret = float2::zero;
	JSON_Object* float2_iterate = nullptr;
	float2_iterate = json_object_get_object(conf, "float2");
	ret.x = GetFloat(float2_iterate, "x", default.x);
	ret.y = GetFloat(float2_iterate, "y", default.y);
	return ret;
}

float3 ParsonJSON::GetFloat3(JSON_Object* conf, const char* field, float3 default) const
{
	float3 ret = float3::zero;
	JSON_Object* float3_iterate = nullptr;
	float3_iterate = json_object_get_object(conf, "float3");
	ret.x = GetFloat(float3_iterate, "x", default.x);
	ret.y = GetFloat(float3_iterate, "y", default.y);
	ret.z = GetFloat(float3_iterate, "z", default.z);
	return ret;
}

float4x4 ParsonJSON::GetFloat4x4(JSON_Object* conf, const char* field, float4x4 default) const
{
	float4x4 ret = float4x4::identity;

	JSON_Object* float4x4_iterate = nullptr;
	float4x4_iterate = json_object_get_object(conf, "float4x4");
	ret[0][0] = GetFloat(float4x4_iterate, "00", default[0][0]);
	ret[0][1] = GetFloat(float4x4_iterate, "01", default[0][1]);
	ret[0][2] = GetFloat(float4x4_iterate, "02", default[0][2]);
	ret[0][3] = GetFloat(float4x4_iterate, "03", default[0][3]);

	ret[1][0] = GetFloat(float4x4_iterate, "10", default[1][0]);
	ret[1][1] = GetFloat(float4x4_iterate, "11", default[1][1]);
	ret[1][2] = GetFloat(float4x4_iterate, "12", default[1][2]);
	ret[1][3] = GetFloat(float4x4_iterate, "13", default[1][3]);

	ret[2][0] = GetFloat(float4x4_iterate, "20", default[2][0]);
	ret[2][1] = GetFloat(float4x4_iterate, "21", default[2][1]);
	ret[2][2] = GetFloat(float4x4_iterate, "22", default[2][2]);
	ret[2][3] = GetFloat(float4x4_iterate, "23", default[2][3]);

	ret[3][0] = GetFloat(float4x4_iterate, "30", default[3][0]);
	ret[3][1] = GetFloat(float4x4_iterate, "31", default[3][1]);
	ret[3][2] = GetFloat(float4x4_iterate, "32", default[3][2]);
	ret[3][3] = GetFloat(float4x4_iterate, "33", default[3][3]);

	return ret;
}

Color ParsonJSON::GetColor(JSON_Object* conf, const char* field, Color default) const
{
	Color ret = default;
	JSON_Object* color_iterate = nullptr;
	color_iterate = json_object_get_object(conf, "color");
	ret.r = GetFloat(color_iterate, "r", default.r);
	ret.g = GetFloat(color_iterate, "g", default.g);
	ret.b = GetFloat(color_iterate, "b", default.b);
	ret.a = GetFloat(color_iterate, "a", default.a);
	return ret;
}

bool ParsonJSON::SetInt(JSON_Object* conf, const char * field, int value)
{
	return json_object_set_number(conf, field, (double)value) == JSONSuccess;
}

bool ParsonJSON::SetUInt(JSON_Object* conf, const char * field, uint value)
{
	return json_object_set_number(conf, field, (double)value) == JSONSuccess;
}

bool ParsonJSON::SetFloat(JSON_Object* conf, const char * field, float value)
{
	return json_object_set_number(conf, field, (double)value) == JSONSuccess;
}

bool ParsonJSON::SetDouble(JSON_Object* conf, const char * field, double value)
{
	return json_object_set_number(conf, field, (double)value) == JSONSuccess;
}

bool ParsonJSON::SetBool(JSON_Object* conf, const char * field, bool value)
{
	uint boolean = 0;
	if(value) boolean = 1;
	return json_object_set_boolean(conf, field, boolean) == JSONSuccess;
}

bool ParsonJSON::SetString(JSON_Object* conf, const char * field, const char* value)
{
	return json_object_set_string(conf, field, value) == JSONSuccess;
}

bool ParsonJSON::SetFloat2(JSON_Object* conf, const char* field, float2 value)
{
	bool ret = true;
	JSON_Object* float2_iterate = nullptr;
	float2_iterate = json_object_get_object(conf, "float2");
	ret = SetFloat(float2_iterate, "x", value.x);
	if (!ret) return false;
	ret = SetFloat(float2_iterate, "y", value.y);
	return ret;
}

bool ParsonJSON::SetFloat3(JSON_Object* conf, const char* field, float3 value)
{
	bool ret = true;
	JSON_Object* float3_iterate = nullptr;
	float3_iterate = json_object_get_object(conf, "float3");
	ret = SetFloat(float3_iterate, "x", value.x);
	if (!ret) return false;
	ret = SetFloat(float3_iterate, "y", value.y);
	if (!ret) return false;
	ret = SetFloat(float3_iterate, "z", value.z);
	return ret;
}

bool ParsonJSON::SetFloat4x4(JSON_Object* conf, const char* field, float4x4 value)
{
	bool ret = true;
	JSON_Object* float4x4_iterate = nullptr;
	float4x4_iterate = json_object_get_object(conf, "float4x4");
	ret = SetFloat(float4x4_iterate, "00", value[0][0]);
	if (!ret) return false;
	ret = SetFloat(float4x4_iterate, "01", value[0][1]);
	if (!ret) return false;
	ret = SetFloat(float4x4_iterate, "02", value[0][2]);
	if (!ret) return false;
	ret = SetFloat(float4x4_iterate, "03", value[0][3]);

	if (!ret) return false;
	ret = SetFloat(float4x4_iterate, "10", value[1][0]);
	if (!ret) return false;
	ret = SetFloat(float4x4_iterate, "11", value[1][1]);
	if (!ret) return false;
	ret = SetFloat(float4x4_iterate, "12", value[1][2]);
	if (!ret) return false;
	ret = SetFloat(float4x4_iterate, "13", value[1][3]);

	if (!ret) return false;
	ret = SetFloat(float4x4_iterate, "20", value[2][0]);
	if (!ret) return false;
	ret = SetFloat(float4x4_iterate, "21", value[2][1]);
	if (!ret) return false;
	ret = SetFloat(float4x4_iterate, "22", value[2][2]);
	if (!ret) return false;
	ret = SetFloat(float4x4_iterate, "23", value[2][3]);

	if (!ret) return false;
	ret = SetFloat(float4x4_iterate, "30", value[3][0]);
	if (!ret) return false;
	ret = SetFloat(float4x4_iterate, "31", value[3][1]);
	if (!ret) return false;
	ret = SetFloat(float4x4_iterate, "32", value[3][2]);
	if (!ret) return false;
	ret = SetFloat(float4x4_iterate, "33", value[3][3]);
	return ret;
}

bool ParsonJSON::SetColor(JSON_Object* conf, const char* field, Color color)
{
	bool ret = true;
	JSON_Object* color_iterate = nullptr;
	color_iterate = json_object_get_object(conf, "float3");
	ret = SetFloat(color_iterate, "r", color.r);
	if (!ret) return false;
	ret = SetFloat(color_iterate, "g", color.g);
	if (!ret) return false;
	ret = SetFloat(color_iterate, "b", color.b);
	if (!ret) return false;
	ret = SetFloat(color_iterate, "a", color.a);
	return ret;
}