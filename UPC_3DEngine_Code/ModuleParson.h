#ifndef __ModuleParson_H__
#define __ModuleParson_H__

#include <string>
#include "Globals.h"
#include "parson\parson.h"
#include "Color.h"
#include "MathGeoLib\Math.h"

class GameObject;
class Resource;
struct ParticleState;
class ParticleEmitter;
class ResourceTexture;
struct ParticleTextureData;
class ComponentParticleSystem;

class ParsonJSON
{
public:
	ParsonJSON(const char* filename, bool nameWithPath, bool isScene, bool loadingScene);
	~ParsonJSON();

	bool Init();

	bool SaveModulesConfig() const;
	bool LoadModulesConfig();

	bool SaveScene(const GameObject* root) const;
	bool LoadScene(GameObject* root);

	bool SaveResource(const Resource* mesh) const;
	bool LoadResource(Resource& mesh);

	//States and emitters are here because we don't want to have JSON_Object inside ParticleSystem.h/.cpp, we want to be as independent as possible
	bool SaveParticleStates(const ResourceTexture* TextureResource, const ParticleTextureData* TexData, const ParticleState* stateI, const ParticleState* stateF) const;
	bool LoadParticleStates(ComponentParticleSystem* system, ParticleState& stateI, ParticleState& stateF) const;

	bool SaveParticleEmitter(ComponentParticleSystem* system, const ParticleEmitter* emitter) const;
	bool LoadParticleEmitter(ComponentParticleSystem* system, ParticleEmitter& emitter) const;

	int GetInt(JSON_Object* conf, const char* field, int default = 0) const;
	uint GetUInt(JSON_Object* conf, const char* field, uint default = 0) const;
	float GetFloat(JSON_Object* conf, const char* field, float default = 0.0f) const;
	double GetDouble(JSON_Object* conf, const char* field, double default = 0.0) const;
	bool GetBool(JSON_Object* conf, const char* field, bool default = false) const;
	const char* GetString(JSON_Object* conf, const char* field, const char* default = "") const;
	float2 GetFloat2(JSON_Object* conf, const char* field, float2 default = float2::zero) const;
	float3 GetFloat3(JSON_Object* conf, const char* field, float3 default = float3::zero) const;
	float4 GetFloat4(JSON_Object* conf, const char* field, float4 default = float4::zero) const;
	float4x4 GetFloat4x4(JSON_Object* conf, const char* field, float4x4 default = float4x4::identity) const;
	Quat GetQuat(JSON_Object* conf, const char* field, Quat default = Quat::identity) const;
	Color GetColor(JSON_Object* conf, const char* field, Color default = White) const;

	bool SetInt(JSON_Object* conf, const char* field, int value) const;
	bool SetUInt(JSON_Object* conf, const char* field, uint value) const;
	bool SetFloat(JSON_Object* conf, const char* field, float value) const;
	bool SetDouble(JSON_Object* conf, const char* field, double value) const;
	bool SetBool(JSON_Object* conf, const char* field, bool value) const;
	bool SetString(JSON_Object* conf, const char* field, const char* value) const;
	bool SetFloat2(JSON_Object* conf, const char* field, float2 value) const;
	bool SetFloat3(JSON_Object* conf, const char* field, float3 value) const;
	bool SetFloat4(JSON_Object* conf, const char* field, float4 value) const;
	bool SetFloat4x4(JSON_Object* conf, const char* field, float4x4 value) const;
	bool SetQuat(JSON_Object* conf, const char* field, Quat value) const;
	bool SetColor(JSON_Object* conf, const char* field, Color color) const;

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