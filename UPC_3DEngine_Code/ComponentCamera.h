#pragma once
#include "Component.h"

class ComponentCamera : public Component
{
	friend class ModuleCamera3D;
	friend class ComponentTransform;
public:
	ComponentCamera(GameObject* parent, bool Active);
	~ComponentCamera();

	bool Enable();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool Disable();
	bool CleanUp();
	void DrawComponentImGui();
	void SetFrame(float3& pos, float3& front, float3& up);

	bool SaveComponent(JSON_Object* conf) const;
	bool LoadComponent(JSON_Object* conf);

	bool IsMainCamera() const;
	void SetMainCamera(bool maincam);

	bool IsCulling() const;

	float4x4 GetViewMatrix() const;
	float4x4 GetViewProjMatrix() const;

private:
	bool FrustumCulling = false;
	bool MainCamera = false;

	Frustum frustum;
	float NearPlaneDistance = 0.125f;
	float FarPlaneDistance = 512.0f;
	float FOVVertical = 60.0f;
	float AspectRatio = 1280.0f / 720.0f;
	float3 Pos = { 0.0f, 0.0f, 0.0f };
	float3 Up = { 0.0f, 1.0f, 0.0f };
	float3 Front = { 0.0f, 0.0f, 1.0f };

	bool DebugDrawFrustum = false;
};