#pragma once
#include "Component.h"

class ComponentCamera : public Component
{
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

	bool IsMainCamera() const;

	const float4x4* GetViewProjMatrix() const;

private:
	void GenerateFrustumDraw();
	void CleanFrustumDraw();

private:
	bool FrustumCulling = false;
	bool MainCamera = false;

	Frustum frustum;
	float NearPlaneDistance = 0.1f;
	float FarPlaneDistance = 5.0f;
	float FOVVertical = 60.0f;
	float AspectRatio = 1280.0f / 720.0f;
	float3 Pos = { 0.0f, 0.0f, 0.0f };
	float3 Up = { 0.0f, 1.0f, 0.0f };
	float3 Front = { 0.0f, 0.0f, 1.0f };

	bool DebugDrawFrustum = false;
	uint DebugDrawFrustum_id_vertices = 0; // id in VRAM
	float* DebugDrawFrustum_vertices = nullptr;
	uint DebugDrawFrustum_id_indices = 0; // id in VRAM
	uint* DebugDrawFrustum_indices = nullptr;

	bool first_time = true;
};