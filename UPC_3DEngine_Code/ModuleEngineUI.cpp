#include "Globals.h"
#include "Application.h"
#include "ModuleEngineUI.h"

#include "Glew\include\glew.h"
#include "imgui-1.51\imgui.h"
#include "imgui-1.51\imgui_impl_sdl_gl3.h"

ModuleEngineUI::ModuleEngineUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleEngineUI::~ModuleEngineUI()
{
	
}

bool ModuleEngineUI::Init()
{
	return true;
}

// Load assets
bool ModuleEngineUI::Start()
{
	LOG("Loading ModuleEngineUI");
	bool ret = true;

	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);

	return ret;
}

update_status ModuleEngineUI::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Update
update_status ModuleEngineUI::Update(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	ImGui::ShowTestWindow();

	if (ImGui::Button("close"))
		App->WantToClose();

	ImGui::Render();

	return UPDATE_CONTINUE;
}

update_status ModuleEngineUI::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleEngineUI::CleanUp()
{
	LOG("Unloading ModuleEngineUI");
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}