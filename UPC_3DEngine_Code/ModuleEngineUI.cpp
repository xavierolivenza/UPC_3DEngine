#include "Globals.h"
#include "Application.h"
#include "ModuleEngineUI.h"

ModuleEngineUI::ModuleEngineUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleEngineUI::~ModuleEngineUI()
{
	
}

bool ModuleEngineUI::Init()
{
	active = true;
	return true;
}

// Load assets
bool ModuleEngineUI::Start()
{
	LOGP("Loading ModuleEngineUI");
	bool ret = true;

	return ret;
}

update_status ModuleEngineUI::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Update
update_status ModuleEngineUI::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleEngineUI::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleEngineUI::CleanUp()
{
	LOGP("Unloading ModuleEngineUI");
	ImGui_ImplSdlGL3_Shutdown();
	active = false;
	return true;
}

void ModuleEngineUI::DrawModuleImGui()
{
	//------------------------------------------//
	//-----------------Menu Bar-----------------//
	//------------------------------------------//
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View"))
	{
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Show Gui Demo Window"))
			showTestWindow = !showTestWindow;
		if (ImGui::MenuItem("Documentation"))
		{
			//App->OpenLink();
		}
		if (ImGui::MenuItem("Download latest"))
			App->OpenLink("https://github.com/xavierolivenza/UPC_3DEngine/releases");
		if (ImGui::MenuItem("Report Bug"))
		{
			//App->OpenLink();
		}
		if (ImGui::MenuItem("About"))
		{

		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Close"))
	{
		if (ImGui::MenuItem("Close Engine"))
		{
			App->WantToClose();
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	//------------------------------------------//
	//-------------GUI Test Window--------------//
	//------------------------------------------//
	if(showTestWindow)
		ImGui::ShowTestWindow();

	//------------------------------------------//
	//-----------Configuration Window-----------//
	//------------------------------------------//
	ImGui::Begin("Configuration");

	ImGui::End();

	//------------------------------------------//
	//-----------------Console------------------//
	//------------------------------------------//
	ImGui::Begin("Console");
	for (std::list<std::string>::iterator item = console_logs.begin(); item != console_logs.cend(); ++item)
		ImGui::Text(item._Ptr->_Myval.c_str());
	static char str0[128] = "";
	if (ImGui::InputText("input text", str0, 128, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		LOGP(str0);
	}
	ImGui::End();

	//------------------------------------------//
	//-----------------Rand Gen-----------------//
	//------------------------------------------//
	ImGui::Begin("Random generator");
	if (ImGui::Button("Generate"))
	{
		LCG rand_test;
		LOGP("Random generator Flo: %f", rand_test.Float(0.0f, 1.0f));
		LOGP("Random generator Int: %i", rand_test.Int(0, 100));
	}
	ImGui::End();
}

void ModuleEngineUI::PushNewConsoleLabel(std::string* newlabel)
{
	console_logs.push_back(*newlabel);
}

void ModuleEngineUI::GetEvent(SDL_Event* event)
{
	ImGui_ImplSdlGL3_ProcessEvent(event);
}

bool ModuleEngineUI::IsActive()
{
	return active;
}