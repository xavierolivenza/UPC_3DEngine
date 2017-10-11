#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

struct PhysVehicle3D;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();
	float* GetInverseViewMatrix();
	void CenterCameraToGeometry(const AABB* meshAABB);
	void RecentreCameraToGeometry();

	void ImGuiModuleVariables();

private:

	void CalculateViewMatrix();
	void NormalizeVec(float* vector);

public:
	
	vec3 X, Y, Z, Position, Reference;

private:
	bool			Cam_move_LOG = true;
	bool			Cam_move = false;
	mat4x4			ViewMatrix;
	mat4x4			ViewMatrixInverse;

	const AABB*		LastCentreGeometry = nullptr;

};