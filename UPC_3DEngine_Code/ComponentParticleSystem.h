#ifndef __ComponentParticleSystem_H__
#define __ComponentParticleSystem_H__

#include "Component.h"

class ParticleSystem;
class ResourceTexture;

class ComponentParticleSystem : public Component
{
public:
	ComponentParticleSystem(GameObject* parent, bool Active);
	~ComponentParticleSystem();

	bool Enable();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool Disable();
	bool CleanUp();
	void DrawComponentImGui();

	void SetMeshResource(uint uuid);
	void SetTextureResource(uint uuid);
	void SetParticleResource(uint uuid);
	void SetEmitterResource(uint uuid);

	bool SaveComponent(JSON_Object* conf) const;
	bool LoadComponent(JSON_Object* conf);

	void ImGuiLoadPopUp();
	void ImGuiLoadTexturePopUp();
	void ImGuiLoadParticlePopUp();
	void ImGuiLoadEmitterPopUp();
	void ImGuiLoadMeshPopUp();
	void ImGuiSavePopUp();
	void ImGuiSaveParticlePopUp();
	void ImGuiSaveEmitterPopUp();

private:
	void DrawDirectory(const char* directory);

private:
	ParticleSystem* PartSystem = nullptr;
	ResourceTexture* TextureResource = nullptr;
	bool PopUpLoadOpen = false;
	bool PopUpSaveOpen = false;
	std::string FileToLoad;
	std::string FileToLoadName;
	std::string FileToSave;
	std::string FileToSaveName;
	std::string DirectoryTemporalStr;

	enum
	{
		Texture_Resource,
		Particle_Resource,
		Emitter_Resource,
		MeshResource
	} FileType = Texture_Resource;
};

#endif // __ComponentParticleSystem_H__