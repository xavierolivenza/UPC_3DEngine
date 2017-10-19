#pragma once
#include "Component.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera(bool Active);
	~ComponentCamera();

	bool Enable();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool Disable();
	bool CleanUp();
	void DrawComponentImGui();

private:
	Frustum frustrum;
	float NearPlaneDistance = 0.1f;
	float FarPlaneDistance = 5.0f;
	float FOVHoritzontal = 60.0f;
	float AspectRatio = 1.78f;
	vec Pos = { 0.0f, 0.0f, 0.0f };
	vec Up = { 0.0f, 1.0f, 0.0f };
	vec Front = { 0.0f, 0.0f, 1.0f };

	bool DebugDrawFrustrum = false;
	uint DebugDrawFrustrum_id_vertices = 0; // id in VRAM
	float* DebugDrawFrustrum_vertices = nullptr;
	uint DebugDrawFrustrum_id_indices = 0; // id in VRAM
	uint* DebugDrawFrustrum_indices = nullptr;
};