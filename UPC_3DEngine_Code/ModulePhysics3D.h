#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

class DebugDrawer;

class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(Application* app, bool start_enabled = true);
	~ModulePhysics3D();

	update_status Update(float dt);

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);

	void ImGuiModuleVariables();
private:
	DebugDrawer* debug_draw = nullptr;
};

class DebugDrawer
{
public:
	DebugDrawer() : line(0,0,0)
	{}

	void drawGrid(int gridWH);
	void drawAxis(float axislen);

	void drawLine(const float3& from, const float3& to, const float3& color);
	void drawContactPoint(const float3& PointOnB, const float3& color);

	P2Line line;
	Primitive point;

	bool DrawGrid = true;
	bool DrawAxis = true;
	uint GridSize = 20;
	float AxisLength = 2.0f;
	float LineThickness = 2.0f;
};