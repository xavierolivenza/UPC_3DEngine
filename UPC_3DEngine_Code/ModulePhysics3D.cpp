#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"
#include "Primitive.h"

ModulePhysics3D::ModulePhysics3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "World Debug (Old Physics)";
	debug_draw = new DebugDrawer();
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	RELEASE(debug_draw);
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	if(debug_draw->DrawGrid)
		debug_draw->drawGrid(debug_draw->GridSize);
	if(debug_draw->DrawAxis)
		debug_draw->drawAxis(debug_draw->AxisLength);
	return UPDATE_CONTINUE;
}

bool ModulePhysics3D::SaveConf(JSON_Object* conf) const
{
	App->parsonjson->SetBool(conf, "DrawGrid", debug_draw->DrawGrid);
	App->parsonjson->SetBool(conf, "DrawAxis", debug_draw->DrawAxis);
	App->parsonjson->SetUInt(conf, "GridSize", debug_draw->GridSize);
	App->parsonjson->SetFloat(conf, "AxisLength", debug_draw->AxisLength);
	return true;
}

bool ModulePhysics3D::LoadConf(JSON_Object* conf)
{
	debug_draw->DrawGrid = App->parsonjson->GetBool(conf, "DrawGrid", true);
	debug_draw->DrawAxis = App->parsonjson->GetBool(conf, "DrawAxis", true);
	debug_draw->GridSize = App->parsonjson->GetUInt(conf, "GridSize", 20);
	debug_draw->AxisLength = App->parsonjson->GetFloat(conf, "AxisLength", 2.0f);
	return true;
}

void ModulePhysics3D::ImGuiModuleVariables()
{
	static char str0[50] = "";
	static char str1[25] = "";
	ImGui::Checkbox("DrawGrid", &debug_draw->DrawGrid);
	strcpy(str0, itoa(debug_draw->GridSize, str1, 10));
	if (ImGui::InputText("GridSize", str0, 50, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal))
		debug_draw->GridSize = atoi(str0);
	ImGui::Checkbox("DrawAxis", &debug_draw->DrawAxis);
	strcpy(str0, itoa(debug_draw->AxisLength, str1, 10));
	if (ImGui::InputText("AxisLength", str0, 50, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal))
		debug_draw->AxisLength = atoi(str0);
}

// =============================================
void DebugDrawer::drawGrid(int gridWH)
{
	float3 line_color = { 255, 255 ,255 };
	for (int i = -gridWH; i <= gridWH; i++)
	{
		float3 line1 = { (float)i, 0.0f , (float)-gridWH };
		float3 line2 = { (float)i, 0.0f , (float)gridWH };
		drawLine(line1, line2, line_color);
		line1 = { (float)-gridWH, 0.0f , (float)i };
		line2 = { (float)gridWH, 0.0f , (float)i };
		drawLine(line1, line2, line_color);
	}
}

void DebugDrawer::drawAxis(float axislen)
{
	float3 line_color = { 255, 255 ,255 };
	float3 origin = { 0.0f ,0.01f ,0.0f };
	float3 axis = { axislen ,0.01f ,0.0f };
	LineThickness = 4.0f;
	line_color = { 255 ,0 ,0 };
	drawLine(origin, axis, line_color);
	axis = { 0.0f ,axislen + 0.01f,0.0f };
	line_color = { 0 ,255 ,0 };
	drawLine(origin, axis, line_color);
	axis = { 0.0f ,0.01f ,axislen };
	line_color = { 0 ,0 ,255 };
	drawLine(origin, axis, line_color);
	LineThickness = 2.0f;
}

void DebugDrawer::drawLine(const float3& from, const float3& to, const float3& color)
{
	line.origin.Set(from.x, from.y, from.z);
	line.destination.Set(to.x, to.y, to.z);
	line.color.Set(color.x, color.y, color.z);
	line.thickness = LineThickness;
	line.Render();
}

void DebugDrawer::drawContactPoint(const float3& PointOnB, const float3& color)
{
	point.transform.Translate(PointOnB.x, PointOnB.y, PointOnB.z);
	point.color.Set(color.x, color.y, color.z);
	point.Render();
}