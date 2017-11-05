#pragma once
#include "Module.h"
#include "Globals.h"
//#include "glmath.h"
#include "MathGeoLib\Math\float3.h"

struct PhysVehicle3D;
class ComponentCamera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);
	float4x4 GetViewMatrix() const;
	float4x4 GetViewProjMatrix() const;
	void CenterCameraToGeometry(const AABB* meshAABB);

	void UpdateCamFov(int width, int height);

	void ImGuiModuleVariables();

public:

	float3 X, Y, Z, Position, Reference;

private:
	bool			Cam_move_LOG = true;
	bool			Cam_move = false;

	ComponentCamera* CameraComp = nullptr;

	LineSegment		MousePickRay;
	LineSegment		RayLocal;
	bool			RayDebugDraw = false;

};