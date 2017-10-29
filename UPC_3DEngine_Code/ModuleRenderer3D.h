#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

#define CHECKERS_HEIGHT 128
#define CHECKERS_WIDTH 128

class ComponentMesh;
class ComponentMaterial;
class ComponentTransform;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool Draw(const GeometryData* meshData) const;
	bool Draw(const std::vector<GeometryData>* meshData) const;

	bool DrawComponentMeshMaterial(const ComponentTransform* transform, const ComponentMesh* mesh, const ComponentMaterial* material) const;
	
	//The points are ordered like: 0: ---, 1: --+, 2: -+-, 3: -++, 4: +--, 5: +-+, 6: ++-, 7: +++
	bool DrawDebugBox(const float3& p0, const float3& p1, const float3& p2, const float3& p3, const float3& p4, const float3& p5, const float3& p6, const float3& p7) const;
	bool DrawDebugBox(const float3& p0, const float3& p1, const float3& p2, const float3& p3, const float3& p4, const float3& p5, const float3& p6, const float3& p7, float thickness) const;
	bool DrawDebugBox(const float3& p0, const float3& p1, const float3& p2, const float3& p3, const float3& p4, const float3& p5, const float3& p6, const float3& p7, float r, float g, float b) const;
	bool DrawDebugBox(const float3& p0, const float3& p1, const float3& p2, const float3& p3, const float3& p4, const float3& p5, const float3& p6, const float3& p7, float r, float g, float b, float thickness) const;

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);

	void OnResize(int width, int height);

	void ImGuiModuleVariables();

	bool GL_Wireframe_Active()
	{
		return GL_Wireframe;
	}

	bool GL_Point_Active()
	{
		return GL_Point;
	}

private:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	bool GL_DepthTest = false;
	bool GL_CullFace = false;
	bool GL_Lighting = false;
	float LightModelAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	bool GL_ColorMaterial = false;
	bool GL_Texture2D = false;
	bool GL_Fog = false;
	bool GL_Diffuse = false;
	bool GL_Specular = false;
	bool GL_Wireframe = false;
	bool GL_Point = false;
	bool DebugVNormals = false;
	float NormalLength = 1.0f;

	//uint Lenna_tex = 0;
	uint id_checkImage = 0;
	unsigned char checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

	//Debug Draw Default
	float DebugDrawDefaultR = 1.0f;
	float DebugDrawDefaultG = 0.0f;
	float DebugDrawDefaultB = 0.0f;
	float DebugDrawDefaultThickness = 5.0f;
};