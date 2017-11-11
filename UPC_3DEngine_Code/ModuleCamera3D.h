#pragma once
#include "Module.h"
#include "Globals.h"
//#include "glmath.h"
#include "MathGeoLib\Math\float3.h"

struct PhysVehicle3D;
class ComponentCamera;

class ModuleCamera3D : public Module
{
	friend class ComponentTransform;
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

	const ComponentCamera* GetCameraComp() const
	{
		return CameraComp;
	}

	//Store octree varable + one getter function(to get inview gameobjects)	bool GetGOInView(std::vector<ComponentMesh*>&)

private:
	void MousePicking();

public:

	float3 X, Y, Z, Position, Reference;

private:
	ComponentCamera* CameraComp = nullptr;

	LineSegment		MousePickRay;
	LineSegment		RayLocal;
	bool			RayDebugDraw = false;

};