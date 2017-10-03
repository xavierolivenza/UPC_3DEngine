#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool Draw(GeometryData* meshData);
	bool Draw(std::vector<GeometryData>* meshData);

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

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	bool GL_DepthTest = false;
	bool GL_CullFace = false;
	bool GL_Lighting = false;
	bool GL_ColorMaterial = false;
	bool GL_Texture2D = false;
	bool GL_Fog = false;
	bool GL_Diffuse = false;
	bool GL_Specular = false;
	bool GL_Wireframe = false;
	bool GL_Point = false;
};