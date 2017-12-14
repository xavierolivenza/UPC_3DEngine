#include "ModuleParson.h"
#include "Module.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"

#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"

#include "ParticleSystem.h"

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

ParsonJSON::ParsonJSON(const char* filename, bool nameWithPath, bool isScene, bool loadingScene) : isScene(isScene)
{
	//json_set_allocation_functions(counted_malloc, counted_free);
	//root_value = json_value_init_object();
	//root_object = json_value_get_object(root_value);
	if (!nameWithPath)
	{
		if (isScene)
			if (loadingScene)
				file_name = filename;
			else
				file_name = *App->importer->Get_Scenes_path() + "\\" + filename + ".json";
		else
			file_name = *App->importer->Get_Settings_path() + "\\" + filename + ".json";
	}
	else
		file_name = std::string(filename) + ".json";
}

ParsonJSON::~ParsonJSON()
{
	json_value_free(root_value);
}

bool ParsonJSON::Init()
{
	root_value = json_parse_file(file_name.c_str());
	if (root_value == NULL)
		root_value = json_value_init_object();
	root_object = json_value_get_object(root_value);
	return true;
}

bool ParsonJSON::SaveModulesConfig() const
{
	JSON_Object* conf = nullptr;
	for (std::list<Module*>::const_reverse_iterator item = App->GetModuleList()->rbegin(); item != App->GetModuleList()->crend(); ++item)
	{
		//If this entry does not exist, create it
		if (json_object_get_object(root_object, (*item)->name.c_str()) == NULL)
			json_object_set_value(root_object, (*item)->name.c_str(), json_value_init_object());
		//Save
		conf = json_object_get_object(root_object, (*item)->name.c_str());
		(*item)->SaveConf(conf);
	}
	//json_serialize_to_string_pretty(root_value);
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

bool ParsonJSON::SaveScene(const GameObject* root) const
{
	JSON_Value* array_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(array_value);
	if (json_object_set_value(root_object, "GameObjects", array_value) == JSONFailure)
		return false;
	root->SaveGameObject(array);
	//json_value_free(array_value);
	/*
	char * serialized_string = json_serialize_to_string_pretty(root_value);
	json_free_serialized_string(serialized_string);
	*/
	json_serialize_to_file(root_value, file_name.c_str());
	return true;
}

bool ParsonJSON::LoadScene(GameObject* root)
{
	/*
	char * serialized_string = json_serialize_to_string_pretty(root_value);
	json_free_serialized_string(serialized_string);
	*/
	JSON_Array* array = json_object_get_array(root_object, "GameObjects");
	if (array != nullptr)
	{
		uint array_count = json_array_get_count(array);
		if (array_count < 0)
		{
			LOGP("Empty GameObject array when trying to load scene.");
			return false;
		}
		for (uint i = 0; i < array_count; i++)
		{
			JSON_Object* array_object = json_array_get_object(array, i);
			uint parentUUID = GetUInt(array_object, "UUID_Parent", 0);
			//If parent UUID is = 0, means that this has no parent, so just create the game object and add to root
			if (parentUUID == 0)
			{
				GameObject* newGameObject = App->scene->CreateGameObject("Default name", true, true);
				newGameObject->LoadGameObject(array_object);
				App->scene->AddChildToRoot(newGameObject);
			}
			else
			{
				GameObject* ParentGameObject = App->scene->FindGameObjectWithUUID(parentUUID);
				if (ParentGameObject == nullptr)
				{
					LOGP("Error finding parent game object with UUID: %i", parentUUID);
					continue;
				}
				GameObject* newGameObject = App->scene->CreateGameObject("Default name", true, true);
				newGameObject->LoadGameObject(array_object);
				ParentGameObject->AddChild(newGameObject);
			}
		}
	}
	else
		LOGP("No array GameObject Found when trying to load scene.");
	return true;
}

bool ParsonJSON::SaveResource(const Resource* mesh) const
{
	JSON_Object* conf = nullptr;

	//If this entry does not exist, create it
	if (json_object_get_object(root_object, mesh->GetTypeStr()) == NULL)
		json_object_set_value(root_object, mesh->GetTypeStr(), json_value_init_object());
	//Save
	conf = json_object_get_object(root_object, mesh->GetTypeStr());

	switch (mesh->GetType())
	{
	case Resource::mesh: ((ResourceMesh*)mesh)->Save(conf); break;
	case Resource::texture: ((ResourceTexture*)mesh)->Save(conf); break;
	}

	//json_serialize_to_string_pretty(root_value);
	json_serialize_to_file(root_value, file_name.c_str());
	return true;
}

bool ParsonJSON::LoadResource(Resource& mesh)
{
	JSON_Object* conf = nullptr;
	conf = json_object_get_object(root_object, mesh.GetTypeStr());

	switch (mesh.GetType())
	{
	case Resource::mesh: ((ResourceMesh*)&mesh)->Load(conf); break;
	case Resource::texture: ((ResourceTexture*)&mesh)->Load(conf); break;
	}

	return true;
}

bool ParsonJSON::SaveParticleStates(const ParticleState* stateI, const ParticleState* stateF) const
{
	JSON_Object* file_conf = nullptr;

	//If this entry does not exist, create it
	if (json_object_get_object(root_object, "particlestate") == NULL)
		json_object_set_value(root_object, "particlestate", json_value_init_object());
	//Save
	file_conf = json_object_get_object(root_object, "particlestate");

	const ParticleState* State = nullptr;
	JSON_Object* conf = nullptr;
	for (uint i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			State = stateI;
			json_object_set_value(root_object, "initial_state", json_value_init_object());
			conf = json_object_get_object(root_object, "initial_state");
		}
		else
		{
			State = stateF;
			json_object_set_value(root_object, "final_state", json_value_init_object());
			conf = json_object_get_object(root_object, "final_state");
		}
		SetFloat3(conf, "force", State->force);
		SetFloat3(conf, "forceVariation", State->forceVariation);
		SetFloat(conf, "Size", State->Size);
		SetFloat(conf, "SizeVariation", State->SizeVariation);
		SetFloat4(conf, "forceVariation", State->RGBATint);
		SetFloat4(conf, "forceVariation", State->RGBATintVariation);
		SetBool(conf, "alpha_preview", State->alpha_preview);
		SetBool(conf, "alpha_half_preview", State->alpha_half_preview);
		SetBool(conf, "options_menu", State->options_menu);
		SetBool(conf, "alpha", State->alpha);
		SetBool(conf, "alpha_bar", State->alpha_bar);
		SetBool(conf, "side_preview", State->side_preview);
		SetUInt(conf, "inputs_mode", State->inputs_mode);
		SetUInt(conf, "picker_mode", State->picker_mode);
	}

	json_serialize_to_file(root_value, file_name.c_str());
	return true;
}

bool ParsonJSON::LoadParticleStates(ParticleState& stateI, ParticleState& stateF) const
{
	return true;
}

bool ParsonJSON::SaveParticleEmitter(const ParticleEmitter* emitter) const
{
	JSON_Object* conf = nullptr;

	//If this entry does not exist, create it
	if (json_object_get_object(root_object, "emitter") == NULL)
		json_object_set_value(root_object, "emitter", json_value_init_object());
	//Save
	conf = json_object_get_object(root_object, "emitter");

	SetFloat(conf, "EmitterLifeMax", emitter->EmitterLifeMax);
	SetFloat4x4(conf, "Transform", emitter->Transform);
	SetUInt(conf, "SpawnRate", emitter->SpawnRate);
	SetFloat(conf, "PreviewState", emitter->PreviewState);
	SetFloat(conf, "Lifetime", emitter->Lifetime);
	SetFloat(conf, "LifetimeVariation", emitter->LifetimeVariation);
	SetFloat(conf, "EmissionDuration", emitter->EmissionDuration);
	SetBool(conf, "Loop", emitter->Loop);
	SetFloat(conf, "Speed", emitter->Speed);
	SetFloat(conf, "SpeedVariation", emitter->SpeedVariation);
	SetFloat3(conf, "BoundingBox_min", emitter->BoundingBox.minPoint);
	SetFloat3(conf, "BoundingBox_max", emitter->BoundingBox.maxPoint);
	SetUInt(conf, "EmissionType", emitter->EmissionType);
	SetUInt(conf, "Type", emitter->Type);
	SetUInt(conf, "ParticleFacingOptions", emitter->ParticleFacingOptions);

	json_serialize_to_file(root_value, file_name.c_str());
	return true;
}

bool ParsonJSON::LoadParticleEmitter(ParticleEmitter& emitter) const
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
	if (value && (json_value_get_type(value) == JSONBoolean))
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
	//If this entry does not exist, create it
	if (json_object_get_object(conf, field) == NULL)
		json_object_set_value(conf, field, json_value_init_object());
	float2_iterate = json_object_get_object(conf, field);

	ret.x = GetFloat(float2_iterate, "x", default.x);
	ret.y = GetFloat(float2_iterate, "y", default.y);
	return ret;
}

float3 ParsonJSON::GetFloat3(JSON_Object* conf, const char* field, float3 default) const
{
	float3 ret = float3::zero;
	JSON_Object* float3_iterate = nullptr;
	//If this entry does not exist, create it
	if (json_object_get_object(conf, field) == NULL)
		json_object_set_value(conf, field, json_value_init_object());
	float3_iterate = json_object_get_object(conf, field);

	ret.x = GetFloat(float3_iterate, "x", default.x);
	ret.y = GetFloat(float3_iterate, "y", default.y);
	ret.z = GetFloat(float3_iterate, "z", default.z);
	return ret;
}

float4 ParsonJSON::GetFloat4(JSON_Object * conf, const char * field, float4 default) const
{
	float4 ret = float4::zero;
	JSON_Object* float4_iterate = nullptr;
	//If this entry does not exist, create it
	if (json_object_get_object(conf, field) == NULL)
		json_object_set_value(conf, field, json_value_init_object());
	float4_iterate = json_object_get_object(conf, field);

	ret.x = GetFloat(float4_iterate, "x", default.x);
	ret.y = GetFloat(float4_iterate, "y", default.y);
	ret.z = GetFloat(float4_iterate, "z", default.z);
	ret.w = GetFloat(float4_iterate, "w", default.w);
	return ret;
}

float4x4 ParsonJSON::GetFloat4x4(JSON_Object* conf, const char* field, float4x4 default) const
{
	float4x4 ret = float4x4::identity;
	JSON_Object* float4x4_iterate = nullptr;
	//If this entry does not exist, create it
	if (json_object_get_object(conf, field) == NULL)
		json_object_set_value(conf, field, json_value_init_object());
	float4x4_iterate = json_object_get_object(conf, field);

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

Quat ParsonJSON::GetQuat(JSON_Object* conf, const char* field, Quat default) const
{
	Quat ret = Quat::identity;
	JSON_Object* quat_iterate = nullptr;
	//If this entry does not exist, create it
	if (json_object_get_object(conf, field) == NULL)
		json_object_set_value(conf, field, json_value_init_object());
	quat_iterate = json_object_get_object(conf, field);

	ret.x = GetFloat(quat_iterate, "x", default.x);
	ret.y = GetFloat(quat_iterate, "y", default.y);
	ret.z = GetFloat(quat_iterate, "z", default.z);
	ret.w = GetFloat(quat_iterate, "w", default.w);
	return ret;
}

Color ParsonJSON::GetColor(JSON_Object* conf, const char* field, Color default) const
{
	Color ret = default;
	JSON_Object* color_iterate = nullptr;
	//If this entry does not exist, create it
	if (json_object_get_object(conf, field) == NULL)
		json_object_set_value(conf, field, json_value_init_object());
	color_iterate = json_object_get_object(conf, field);

	ret.r = GetFloat(color_iterate, "r", default.r);
	ret.g = GetFloat(color_iterate, "g", default.g);
	ret.b = GetFloat(color_iterate, "b", default.b);
	ret.a = GetFloat(color_iterate, "a", default.a);
	return ret;
}

bool ParsonJSON::SetInt(JSON_Object* conf, const char * field, int value) const
{
	return json_object_set_number(conf, field, (double)value) == JSONSuccess;
}

bool ParsonJSON::SetUInt(JSON_Object* conf, const char * field, uint value) const
{
	return json_object_set_number(conf, field, (double)value) == JSONSuccess;
}

bool ParsonJSON::SetFloat(JSON_Object* conf, const char * field, float value) const
{
	return json_object_set_number(conf, field, (double)value) == JSONSuccess;
}

bool ParsonJSON::SetDouble(JSON_Object* conf, const char * field, double value) const
{
	return json_object_set_number(conf, field, (double)value) == JSONSuccess;
}

bool ParsonJSON::SetBool(JSON_Object* conf, const char * field, bool value) const
{
	uint boolean = 0;
	if(value) boolean = 1;
	return json_object_set_boolean(conf, field, boolean) == JSONSuccess;
}

bool ParsonJSON::SetString(JSON_Object* conf, const char * field, const char* value) const
{
	return json_object_set_string(conf, field, value) == JSONSuccess;
}

bool ParsonJSON::SetFloat2(JSON_Object* conf, const char* field, float2 value) const
{
	bool ret = true;
	JSON_Object* float2_iterate = nullptr;
	//If this entry does not exist, create it
	if (json_object_get_object(conf, field) == NULL)
		json_object_set_value(conf, field, json_value_init_object());
	float2_iterate = json_object_get_object(conf, field);

	ret = SetFloat(float2_iterate, "x", value.x);
	if (!ret) return false;
	ret = SetFloat(float2_iterate, "y", value.y);
	return ret;
}

bool ParsonJSON::SetFloat3(JSON_Object* conf, const char* field, float3 value) const
{
	bool ret = true;
	JSON_Object* float3_iterate = nullptr;
	//If this entry does not exist, create it
	if (json_object_get_object(conf, field) == NULL)
		json_object_set_value(conf, field, json_value_init_object());
	float3_iterate = json_object_get_object(conf, field);

	ret = SetFloat(float3_iterate, "x", value.x);
	if (!ret) return false;
	ret = SetFloat(float3_iterate, "y", value.y);
	if (!ret) return false;
	ret = SetFloat(float3_iterate, "z", value.z);
	return ret;
}

bool ParsonJSON::SetFloat4(JSON_Object * conf, const char * field, float4 value) const
{
	bool ret = true;
	JSON_Object* float4_iterate = nullptr;
	//If this entry does not exist, create it
	if (json_object_get_object(conf, field) == NULL)
		json_object_set_value(conf, field, json_value_init_object());
	float4_iterate = json_object_get_object(conf, field);

	ret = SetFloat(float4_iterate, "x", value.x);
	if (!ret) return false;
	ret = SetFloat(float4_iterate, "y", value.y);
	if (!ret) return false;
	ret = SetFloat(float4_iterate, "z", value.z);
	if (!ret) return false;
	ret = SetFloat(float4_iterate, "w", value.w);
	return ret;
}

bool ParsonJSON::SetFloat4x4(JSON_Object* conf, const char* field, float4x4 value) const
{
	bool ret = true;
	JSON_Object* float4x4_iterate = nullptr;
	//If this entry does not exist, create it
	if (json_object_get_object(conf, field) == NULL)
		json_object_set_value(conf, field, json_value_init_object());
	float4x4_iterate = json_object_get_object(conf, field);

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

bool ParsonJSON::SetQuat(JSON_Object* conf, const char* field, Quat value) const
{
	bool ret = true;
	JSON_Object* quat_iterate = nullptr;
	//If this entry does not exist, create it
	if (json_object_get_object(conf, field) == NULL)
		json_object_set_value(conf, field, json_value_init_object());
	quat_iterate = json_object_get_object(conf, field);

	ret = SetFloat(quat_iterate, "x", value.x);
	if (!ret) return false;
	ret = SetFloat(quat_iterate, "y", value.y);
	if (!ret) return false;
	ret = SetFloat(quat_iterate, "z", value.z);
	if (!ret) return false;
	ret = SetFloat(quat_iterate, "w", value.w);
	return ret;
}

bool ParsonJSON::SetColor(JSON_Object* conf, const char* field, Color color) const
{
	bool ret = true;
	JSON_Object* color_iterate = nullptr;
	//If this entry does not exist, create it
	if (json_object_get_object(conf, field) == NULL)
		json_object_set_value(conf, field, json_value_init_object());
	color_iterate = json_object_get_object(conf, field);

	ret = SetFloat(color_iterate, "r", color.r);
	if (!ret) return false;
	ret = SetFloat(color_iterate, "g", color.g);
	if (!ret) return false;
	ret = SetFloat(color_iterate, "b", color.b);
	if (!ret) return false;
	ret = SetFloat(color_iterate, "a", color.a);
	return ret;
}