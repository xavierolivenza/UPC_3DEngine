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
	void SetTextureResource(uint uuid, int columns = 1, int rows = 1, int numberOfFrames = 1, uint AnimationOrder = 0);
	void SetTextureResource(const char* Path, int columns, int rows, int numberOfFrames, uint AnimationOrder);

	void SetParticleResource(uint uuid);
	void SetEmitterResource(uint uuid);
	void SetDebugOptions(bool ShowEmitterBoundBox, bool ShowEmitter);
	void GetDebugOptions(bool& ShowEmitterBoundBox, bool& ShowEmitter);

	void SetChild(const char* Particle, const char* Emitter);

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

	void LoadParticleResource(const char* filename);
	void LoadEmitterResource(const char* filename);
	bool IsEmitterDead(); //Usefull to execute an other emitter from the outside when this dies

	const std::string* GetChildParticle() const;
	const std::string* GetChildEmitter() const;

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

	std::string ChildParticle;
	std::string ChildParticleName;
	std::string ChildEmitter;
	std::string ChildEmitterName;
	bool ChildLoaded = false;

	enum
	{
		Texture_Resource,
		Particle_Resource,
		Emitter_Resource,
		Child_Particle_Resource,
		Child_Emitter_Resource,
		MeshResource
	} FileType = Texture_Resource;
};

#endif // __ComponentParticleSystem_H__