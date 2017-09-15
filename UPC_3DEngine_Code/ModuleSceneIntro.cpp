#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

#include "Glew\include\glew.h"
#include "imgui-1.51\imgui.h"
#include "imgui-1.51\imgui_impl_sdl_gl3.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

bool ModuleSceneIntro::Init()
{
	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);
	return true;
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	//--------------------------------------------//
	//Start
	cube1.size.x = 15;
	cube1.size.y = 1;
	cube1.size.z = 15;
	cube1.color = Chocolate;
	cube1.SetPos(0, 0, 0);
	cube1body = App->physics->AddBody(cube1, 0);
	//Arc
	cube1_1.size.x = 1;
	cube1_1.size.y = 10;
	cube1_1.size.z = 1;
	cube1_1.color = Red;
	cube1_1.SetPos(7, 5.5, 5.5);
	cube1_1body = App->physics->AddBody(cube1_1, 0);
	cube1_2.size.x = 13;
	cube1_2.size.y = 1;
	cube1_2.size.z = 1;
	cube1_2.color = Red;
	cube1_2.SetPos(0, 10, 5.5);
	cube1_2body = App->physics->AddBody(cube1_2, 0);
	cube1_3.size.x = 1;
	cube1_3.size.y = 10;
	cube1_3.size.z = 1;
	cube1_3.color = Red;
	cube1_3.SetPos(-7, 5.5, 5.5);
	cube1_3body = App->physics->AddBody(cube1_3, 0);
	//--------------------------------------------//
	return ret;
}

update_status ModuleSceneIntro::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	return UPDATE_CONTINUE;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	ImGui::Text("Hello, world!");
	ImGui::ShowTestWindow();

	//ImGui::End();

	//--------------------------------------------//
	//Start
	cube1.Render();
	//Arc
	cube1_1.Render();
	cube1_2.Render();
	cube1_3.Render();
	//--------------------------------------------//
	
	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	ImGui::Render();
	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	
}