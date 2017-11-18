#include <experimental\filesystem>

#include "Globals.h"
#include "Application.h"
#include "ModuleEngineUI.h"

#include "Assimp\include\version.h"
#include "DevIL\include\il.h"

#include "DeviceId\DeviceId.h"

#include "GameObject.h"
#include "ComponentMesh.h"

#include "ResourceMesh.h"
#include "ResourceTexture.h"

ModuleEngineUI::ModuleEngineUI(Application* app, bool start_enabled) : Module(app, start_enabled),
	fpsPlotData(FPS_AND_MS_PLOT_DATA_LENGTH), msPlotData(FPS_AND_MS_PLOT_DATA_LENGTH), memPlotData(FPS_AND_MS_PLOT_DATA_LENGTH)
{
	name = "engineUI";
}

ModuleEngineUI::~ModuleEngineUI()
{
	
}

bool ModuleEngineUI::Init()
{
	active = true;
	strcpy(appnamestr, App->window->GetTitle()->c_str());

	if (SDL_Has3DNow()) Modes3D += "3D Now Avalible \n";
	else Modes3D += "3D Now Unavalible \n";
	if (SDL_HasAVX()) Modes3D += "AVX Avalible \n";
	else Modes3D += "AVX Unavalible \n";
	//if (SDL_HasAVX2()) Modes3D += "AVX2 Avalible \n";
	//else Modes3D += "AVX2 Unavalible \n";
	if (SDL_HasAltiVec()) Modes3D += "AltiVec Avalible \n";
	else Modes3D += "AltiVec Unavalible \n";
	if (SDL_HasMMX()) Modes3D += "MMX Avalible \n";
	else Modes3D += "MMX Unavalible \n";
	if (SDL_HasRDTSC()) Modes3D += "RDTSC Avalible \n";
	else Modes3D += "RDTSC Unavalible \n";
	if (SDL_HasSSE()) Modes3D += "SSE Avalible \n";
	else Modes3D += "SSE Unavalible \n";
	if (SDL_HasSSE2()) Modes3D += "SSE2 Avalible \n";
	else Modes3D += "SSE2 Unavalible \n";
	if (SDL_HasSSE3()) Modes3D += "SSE3 Avalible \n";
	else Modes3D += "SSE3 Unavalible \n";
	if (SDL_HasSSE41()) Modes3D += "SSE41 Avalible \n";
	else Modes3D += "SSE41 Unavalible \n";
	if (SDL_HasSSE42()) Modes3D += "SSE42 Avalible \n";
	else Modes3D += "SSE42 Unavalible \n";

	return true;
}

// Load assets
bool ModuleEngineUI::Start()
{
	LOGP("Loading ModuleEngineUI");

	//IMGUI STYLE START
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowPadding = ImVec2(10, 15);
	style.WindowRounding = 5.0f;
	style.FramePadding = ImVec2(5, 5);
	style.FrameRounding = 5.0f;
	style.ItemSpacing = ImVec2(4, 4);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 5.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 5.0f;
	style.ChildWindowRounding = 2.5f;

	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.41f, 0.41f, 0.41f, 0.70f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.85f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.80f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.52f, 0.52f, 0.52f, 0.70f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.39f, 0.39f, 0.39f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.59f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.65f, 0.65f, 0.65f, 0.80f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.62f, 0.22f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.68f, 0.34f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.84f, 0.57f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.69f, 0.43f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	style.Colors[ImGuiCol_Separator] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.80f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.55f, 0.00f, 0.85f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.55f, 0.00f, 0.85f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.83f, 0.51f, 0.20f, 0.80f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);


	//IMGUI STYLE END

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
	if ((App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) showMenuBar = !showMenuBar;
	if ((App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) showHierarchyWindow = !showHierarchyWindow;
	if ((App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) showModuleVariablesWindow = !showModuleVariablesWindow;
	if ((App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) showInspectorWindow = !showInspectorWindow;
	if ((App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) showConsoleWindow = !showConsoleWindow;
	if ((App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) showConfigurationWindow = !showConfigurationWindow;
	if ((App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) showProfilerWindow = !showProfilerWindow;
	if ((App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) showTimeManager = !showTimeManager;
	if ((App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) showCulling= !showCulling;
	if ((App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) showResources = !showResources;
	/*
	if ((App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) startAsGame = !startAsGame;
	if (startAsGame)
		showMenuBar = showHierarchyWindow = showModuleVariablesWindow = showInspectorWindow = showConsoleWindow = showConfigurationWindow = showProfilerWindow = showTimeManager = false;
	else
		showMenuBar = showHierarchyWindow = showModuleVariablesWindow = showInspectorWindow = showConsoleWindow = showTimeManager = true;
	*/

	//------------------------------------------//
	//-----------------Menu Bar-----------------//
	//------------------------------------------//
	if (showMenuBar)
		ImGuiDrawMenuBar();

	//------------------------------------------//
	//-------------GUI Test Window--------------//
	//------------------------------------------//
	if(showTestWindow)
		ImGui::ShowTestWindow();

	//------------------------------------------//
	//-------------Brofiler Window--------------//
	//------------------------------------------//
	if (showProfilerWindow)
		ImGuiProfierWindow();

	//------------------------------------------//
	//---------Module Variables Window----------//
	//------------------------------------------//
	if (showModuleVariablesWindow)
		ImGuiModuleVariablesWindow();

	//------------------------------------------//
	//-----------Configuration Window-----------//
	//------------------------------------------//
	if(showConfigurationWindow)
		ImGuiConfigurationWindow();

	//------------------------------------------//
	//-----------------Console------------------//
	//------------------------------------------//
	if(showConsoleWindow)
		ImGuiConsole();

	//------------------------------------------//
	//------------Inspector Window--------------//
	//------------------------------------------//
	if (showInspectorWindow)
		ImGuiInspectorWindow();

	//------------------------------------------//
	//-------------Hierarchy Window-------------//
	//------------------------------------------//
	if (showHierarchyWindow)
		ImGuiHierarchyWindow();

	//------------------------------------------//
	//----------Save & Load File PopUp----------//
	//------------------------------------------//
	if (showLoadFilePopUp)
		ImGuiLoadFilePopUp();

	if (showSaveFilePopUp)
		ImGuiSaveFilePopUp();

	//------------------------------------------//
	//---------------Time Manager---------------//
	//------------------------------------------//
	if (showTimeManager)
		ImGuiTimeManager();

	//------------------------------------------//
	//-----------------Culling------------------//
	//------------------------------------------//
	if (showCulling)
		ImGuiCulling();

	//------------------------------------------//
	//----------------Resources-----------------//
	//------------------------------------------//
	if (showResources)
		ImGuiResources();

	//------------------------------------------//
	//-----------------Rand Gen-----------------//
	//------------------------------------------//
	/*
	ImGui::Begin("Random generator", false, ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize);
	//ImGui::Begin("Random generator", false);
	if (ImGui::Button("Generate"))
	{
		LCG rand_test;
		LOGP("Random generator Flo: %f", rand_test.Float(0.0f, 1.0f));
		LOGP("Random generator Int: %i", rand_test.Int(0, 100));
	}
	ImGui::End();
	*/
}

void ModuleEngineUI::ImGuiDrawMenuBar()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Load File"))
		{
			showLoadFilePopUp = true;
			showSaveFilePopUp = false;
		}
		if (ImGui::MenuItem("Save File"))
		{
			showLoadFilePopUp = false;
			showSaveFilePopUp = true;
		}
		if (ImGui::MenuItem("Clear Scene"))
		{
			App->scene->CleanUpScene();
			App->resources->ClearAllResources();
			GameObjectSelected = nullptr;
			ResouceSelected = nullptr;
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View"))
	{
		ImGui::MenuItem("Hierarchy", "LALT + 1", &showHierarchyWindow);
		ImGui::MenuItem("Module Variables", "LALT + 2", &showModuleVariablesWindow);
		ImGui::MenuItem("Inspector", "LALT + 3", &showInspectorWindow);
		ImGui::MenuItem("Console", "LALT + 4", &showConsoleWindow);
		ImGui::MenuItem("Configuration", "LALT + 5", &showConfigurationWindow);
		ImGui::MenuItem("Profiler", "LALT + 6", &showProfilerWindow);
		ImGui::MenuItem("Time", "LALT + 7", &showTimeManager);
		ImGui::MenuItem("Culling", "LALT + 8", &showCulling);
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Create"))
	{
		if (ImGui::MenuItem("Create Camera"))
		{
			GameObject* TestCamera = new GameObject("Camera", true, true);
			TestCamera->CreateCameraComponent(true);
			App->scene->AddChildToRoot(TestCamera);
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Show Gui Demo Window"))
			showTestWindow = !showTestWindow;
		if (ImGui::MenuItem("Documentation/README"))
			App->OpenLink("https://github.com/xavierolivenza/UPC_3DEngine/blob/master/README.md");
		if (ImGui::MenuItem("Download latest"))
			App->OpenLink("https://github.com/xavierolivenza/UPC_3DEngine/releases");
		if (ImGui::MenuItem("Report Bug"))
			App->OpenLink("https://github.com/xavierolivenza/UPC_3DEngine/issues");
		if (ImGui::BeginMenu("About"))
		{
			if (ImGui::BeginMenu("Name"))
			{
				ImGui::Text("UPC_3DEngine");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Description"))
			{
				ImGui::Text("This 3D Engine is an educational purpose exercise to learn the basics of\nprogramming and working with a 3D Engine in the videogame developing environment");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Authors"))
			{
				ImGui::Text("Sergio Alvarez Llorca");
				ImGui::Text("Xavier Olivenza Busquets");
				ImGui::Text("UPC_3DEngine is a project for the subject 3DEngine of the\nVideoGame Developing career in CITM, UPC fundation in Terrassa.\n2017-2018");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Libraries"))
			{
				char title[50];
				uint title_size = sizeof title;
				SDL_version SDLVersion;
				SDL_GetVersion(&SDLVersion);
				sprintf_s(title, title_size, "SDL: %i.%i.%i", SDLVersion.major, SDLVersion.minor, SDLVersion.patch);
				ImGui::Text(title);sprintf_s(title, title_size, "ImGui: %s", IMGUI_VERSION);
				ImGui::Text(title);
				ImGui::Text("MathGeoLib 1.5");
				ImGui::Text("mmgr");
				ImGui::Text("parson");
				sprintf_s(title, title_size, "Assimp: %i.%i.%i", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());
				ImGui::Text(title);
				sprintf_s(title, title_size, "DevIl: %i", IL_VERSION);
				ImGui::Text(title);
				ImGui::Text("DeviceID");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("License"))
			{
				ImGui::Text("GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Close"))
	{
		if (ImGui::MenuItem("Close Engine"))
			App->WantToClose();
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

void ModuleEngineUI::ImGuiConfigurationWindow()
{
	const PerformanceStruct* PerformanceData = App->GetPerformanceStruct();
	sMStats MemoryStats = m_getMemoryStatistics();
	PushFPSandMSPlot(PerformanceData->frames_last_second, PerformanceData->miliseconds_per_frame, MemoryStats.totalReportedMemory);
	ImGui::Begin("Configuration", false, ImGuiWindowFlags_HorizontalScrollbar);
	//ImGui::Begin("Configuration", false, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::CollapsingHeader("Application"))
	{
		//App name
		if (ImGui::InputText("App name", appnamestr, 128, ImGuiInputTextFlags_EnterReturnsTrue))
			App->window->SetTitle(appnamestr);

		//Max FPS Slider
		char buffer[10];
		snprintf(buffer, sizeof buffer, "%i", App->GetFramerateCapModif());
		if (ImGui::InputText("MaxFPSValue", buffer, sizeof buffer, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal))
			App->GetFramerateCapModif() = atoi(buffer);

		ImGui::Separator();

		//Some useful variables
		char title[100];
		uint title_size = sizeof title;

		//Freeze plots
		ImGui::Checkbox("FrezePlotData", &freezeplots);

		//Framerate PlotHistogram
		sprintf_s(title, title_size, "Framerate: %i", PerformanceData->frames_last_second);
		ImGui::PlotHistogram("##Framerate", &fpsPlotData[0], fpsPlotData.size(), 0, title, 0.0f, 150.0f, ImVec2(310, 100));

		//Miliseconds PlotHistogram
		sprintf_s(title, title_size, "Frame Miliseconds: %i", PerformanceData->miliseconds_per_frame);
		ImGui::PlotHistogram("##Frame Miliseconds", &msPlotData[0], msPlotData.size(), 0, title, 0.0f, 50.0f, ImVec2(310, 100));

		ImGui::Separator();

		//Memory Consumption PlotHistogram
		sprintf_s(title, title_size, "Memory: %i", MemoryStats.totalReportedMemory);
		ImGui::PlotHistogram("##Memory", &memPlotData[0], memPlotData.size(), 0, title, 0.0f, (float)MemoryStats.peakReportedMemory * 1.2f, ImVec2(310, 100));

		//Memory data
		sprintf_s(title, title_size, "totalReportedMemory: %i", MemoryStats.totalReportedMemory);
		ImGui::Text(title);
		sprintf_s(title, title_size, "totalActualMemory: %i", MemoryStats.totalActualMemory);
		ImGui::Text(title);
		sprintf_s(title, title_size, "peakReportedMemory: %i", MemoryStats.peakReportedMemory);
		ImGui::Text(title);
		sprintf_s(title, title_size, "peakActualMemory: %i", MemoryStats.peakActualMemory);
		ImGui::Text(title);
		sprintf_s(title, title_size, "accumulatedReportedMemory: %i", MemoryStats.accumulatedReportedMemory);
		ImGui::Text(title);
		sprintf_s(title, title_size, "accumulatedActualMemory: %i", MemoryStats.accumulatedActualMemory);
		ImGui::Text(title);
		sprintf_s(title, title_size, "accumulatedAllocUnitCount: %i", MemoryStats.accumulatedAllocUnitCount);
		ImGui::Text(title);
		sprintf_s(title, title_size, "totalAllocUnitCount: %i", MemoryStats.totalAllocUnitCount);
		ImGui::Text(title);
		sprintf_s(title, title_size, "peakAllocUnitCount: %i", MemoryStats.peakAllocUnitCount);
		ImGui::Text(title);
	}
	if (ImGui::CollapsingHeader("Time"))
	{
		TimeManager& time = App->GetTimeManagerStruct();
		char title[100];
		uint title_size = sizeof title;
		sprintf_s(title, title_size, "ms_timer: %i", time.ms_timer.Read());
		ImGui::Text(title);
		sprintf_s(title, title_size, "startup_timer: %.3fs", time.startup_timer.Read() / 1000.0f);
		ImGui::Text(title);
		sprintf_s(title, title_size, "last_sec_frame_time: %i", time.last_sec_frame_time.Read());
		ImGui::Text(title);
		sprintf_s(title, title_size, "dt: %.3f", time.dt);
		ImGui::Text(title);
		sprintf_s(title, title_size, "prev_last_sec_frame_count: %i", time.prev_last_sec_frame_count);
		ImGui::Text(title);
		sprintf_s(title, title_size, "last_sec_frame_count: %i", time.last_sec_frame_count);
		ImGui::Text(title);

		ImGui::Separator();

		sprintf_s(title, title_size, "GameSecSinceStartUp: %.3fs", time.GameSecSinceStartUp);
		ImGui::Text(title);
		sprintf_s(title, title_size, "TimeStatus: %i", time.TimeStatus);
		ImGui::Text(title);
		sprintf_s(title, title_size, "TimeUpdate: %.3f", time.TimeUpdate);
		ImGui::Text(title);
		ImGui::DragFloat("Time Distortion", &time.TimeDistortion, 0.05f, 0.1f, 3.0f, "%.2f");
	}
	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::SliderInt("Screen Width", &App->window->w_width, 0, 1920);
		ImGui::SliderInt("Screen Height", &App->window->w_height, 0, 1080);
		if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen))
			App->window->SetFullscreen(App->window->fullscreen);
		ImGui::SameLine();
		ImGui::Checkbox("Resizable", &App->window->resizable);
		ImGui::Checkbox("Fullscreen Desktop", &App->window->fullscreen_desktop);
		ImGui::SameLine();
		ImGui::Checkbox("Borderless", &App->window->borderless);
	}
	if (ImGui::CollapsingHeader("Hardware"))
	{
		static char title[100];
		static uint title_size = sizeof title;
		
		static SDL_version SDLVersion;

		SDL_GetVersion(&SDLVersion);
		sprintf_s(title, title_size, "SDL Major Version: %i", SDLVersion.major);
		ImGui::Text(title);
		sprintf_s(title, title_size, "SDL Minor Version: %i", SDLVersion.minor);
		ImGui::Text(title);
		sprintf_s(title, title_size, "SDL Patch Version: %i", SDLVersion.patch);
		ImGui::Text(title);
		sprintf_s(title, title_size, "SDL Compiled Version: %i", SDL_COMPILEDVERSION);
		ImGui::Text(title);
		
		ImGui::Separator();

		static std::string cpubrand;
		static std::string cpuvendor;
		getCPUInfo(&cpubrand, &cpuvendor);
		sprintf_s(title, title_size, "CPU Vendor: %s", cpuvendor.c_str());
		ImGui::Text(title);
		sprintf_s(title, title_size, "CPU Brand: %s", cpubrand.c_str());
		ImGui::Text(title);
		sprintf_s(title, title_size, "CPU Cache Line Size: %i", SDL_GetCPUCacheLineSize());
		ImGui::Text(title);
		sprintf_s(title, title_size, "CPU Cores: %i", SDL_GetCPUCount());
		ImGui::Text(title);
		
		ImGui::Text(Modes3D.c_str());
		
		ImGui::Separator();

		sprintf_s(title, title_size, "RAM: %i Mb", SDL_GetSystemRAM());
		ImGui::Text(title);

		ImGui::Separator();

		const GLubyte* GPUData = nullptr;
		GPUData = glGetString(GL_VENDOR);
		sprintf_s(title, title_size, "GL_Vendor: %s", GPUData);
		ImGui::Text(title);
		GPUData = glGetString(GL_RENDERER);
		sprintf_s(title, title_size, "GL_Renderer: %s", GPUData);
		ImGui::Text(title);
		GPUData = glGetString(GL_VERSION);
		sprintf_s(title, title_size, "GL_Version: %s", GPUData);
		ImGui::Text(title);
		
		/*
		//Huge str buffer>1000
		char title2[1000];
		GPUData = glGetString(GL_EXTENSIONS);
		sprintf_s(title2, 1000, "GL_Extensions: %s", GPUData);
		ImGui::Text(title2);
		*/

		static char title2[250];
		static uint title2_size = sizeof title2;

		uint vendor, device_id;
		static std::wstring brand;
		unsigned __int64 video_mem_budget;
		unsigned __int64 video_mem_usage;
		unsigned __int64 video_mem_available;
		unsigned __int64 video_mem_reserved;

		if (getGraphicsDeviceInfo(&vendor, &device_id, &brand, &video_mem_budget, &video_mem_usage, &video_mem_available, &video_mem_reserved))
		{
			sprintf_s(title2, title2_size, "GPU: vendor %u device %u", vendor, device_id);
			ImGui::Text(title2);
			sprintf_s(title2, title2_size, "Brand: %S", brand.c_str());
			ImGui::Text(title2);
			sprintf_s(title2, title2_size, "VRAM Budget: %.3f Mb", float(video_mem_budget) / 1073741824.0f);
			ImGui::Text(title2);
			sprintf_s(title2, title2_size, "VRAM Usage: %.3f Mb", float(video_mem_usage) / 1073741824.0f);
			ImGui::Text(title2);
			sprintf_s(title2, title2_size, "VRAM Available: %.3f Mb", float(video_mem_available) / 1073741824.0f);
			ImGui::Text(title2);
			sprintf_s(title2, title2_size, "VRAM Reserved: %.3f Mb", float(video_mem_reserved) / 1073741824.0f);
			ImGui::Text(title2);
		}
	}
	ImGui::End();
}

void  ModuleEngineUI::ImGuiProfierWindow()
{
	ImGui::Begin("Profiler", false);
	//ImGui::Begin("Profiler", false);
	char title[100];
	uint title_size = sizeof title;
	//Module Miliseconds PlotHistogram
	for (std::list<Module*>::const_iterator item = App->GetModuleList()->begin(); item != App->GetModuleList()->cend(); ++item)
	{
		sprintf_s(title, title_size, "Module: %s", item._Ptr->_Myval->name.c_str());
		if (ImGui::CollapsingHeader(title))
		{
			sprintf_s(title, title_size, "PreUpdate Ms: %i", (uint)item._Ptr->_Myval->ModulePreUpdateMs.back());
			ImGui::PlotHistogram("##PreUpdate Ms", &item._Ptr->_Myval->ModulePreUpdateMs[0], item._Ptr->_Myval->ModulePreUpdateMs.size(), 0, title, 0.0f, 30.0f, ImVec2(310, 50));
			sprintf_s(title, title_size, "Update Ms: %i", (uint)item._Ptr->_Myval->ModuleUpdateMs.back());
			ImGui::PlotHistogram("##Update Ms", &item._Ptr->_Myval->ModuleUpdateMs[0], item._Ptr->_Myval->ModuleUpdateMs.size(), 0, title, 0.0f, 30.0f, ImVec2(310, 50));
			sprintf_s(title, title_size, "PostUpdate Ms: %i", (uint)item._Ptr->_Myval->ModulePostUpdateMs.back());
			ImGui::PlotHistogram("##PostUpdate Ms", &item._Ptr->_Myval->ModulePostUpdateMs[0], item._Ptr->_Myval->ModulePostUpdateMs.size(), 0, title, 0.0f, 30.0f, ImVec2(310, 50));
		}
	}
	ImGui::End();
}

void ModuleEngineUI::ImGuiModuleVariablesWindow()
{
	int w = 0; int h = 0; App->window->GetWindowSize(w, h); ImGui::SetNextWindowPos(ImVec2(w - 350, 23 + 520));//w - inspector window width, menu bar height + inspector heigth
	ImGui::Begin("Module Variables", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	//ImGui::Begin("Module Variables", false);
	char title[50];

	for (std::list<Module*>::const_iterator item = App->GetModuleList()->begin(); item != App->GetModuleList()->cend(); ++item)
	{
		sprintf_s(title, sizeof title, "Module: %s", item._Ptr->_Myval->name.c_str());
		if (ImGui::CollapsingHeader(title))
			item._Ptr->_Myval->ImGuiModuleVariables();
	}
	ImGui::End();
}

void ModuleEngineUI::ImGuiConsole()
{
	//Set window to good position
	int w = 0; int h = 0; App->window->GetWindowSize(w, h); ImGui::SetNextWindowPos(ImVec2(w * 0.5f - 600 * 0.5f, h - 177)); //w/2 - console width/2, h - console height
	ImGui::Begin("Console", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	//ImGui::Begin("Console", false);
	if (ImGui::Checkbox("Console", &ShowConsole))
	{
		if (ShowConsole)
		{
			ShowAssets = false;
			ShowLibrary = false;
		}
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Assets", &ShowAssets))
	{
		if (ShowAssets)
		{
			ShowConsole = false;
			ShowLibrary = false;
		}
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Library", &ShowLibrary))
	{
		if (ShowLibrary)
		{
			ShowConsole = false;
			ShowAssets = false;
		}
	}

	if (ShowConsole)
	{
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
		for (std::list<std::string>::iterator item = console_logs.begin(); item != console_logs.cend(); ++item)
			ImGui::Text(item._Ptr->_Myval.c_str());
		if (ScrollDownConsole)
		{
			ImGui::SetScrollHere();
			ScrollDownConsole = false;
		}
		ImGui::EndChild();
		ImGui::Separator();
		static char str0[128] = "";
		if (ImGui::InputText("input text", str0, 128, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			LOGP(str0);
			strcpy(str0, "");
		}
		ImGui::SameLine();
		if (ImGui::Button("Scroll Down"))
			ScrollDownConsole = true;
	}
	else if (ShowAssets)
	{
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		RecursiveDrawDirectory(App->importer->Get_Assets_path()->c_str());
		ImGui::EndChild();
	}
	else if (ShowLibrary)
	{
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		RecursiveDrawDirectory(App->importer->Get_Library_path()->c_str());
		ImGui::EndChild();
	}

	ImGui::End();
}

void ModuleEngineUI::ImGuiInspectorWindow()
{
	//Set window to good position
	int w = 0; int h = 0; App->window->GetWindowSize(w, h); ImGui::SetNextWindowPos(ImVec2(w-350, 23));//w - inspector window width, menu bar height
	ImGui::Begin("Inspector", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar);
	//ImGui::Begin("Inspector", false);
	ImGui::Text("Click one GameObject to see its components.");
	ImGui::Text("If one of the IDs/Paths below is zero/null,\nit means that it does not have\nthat characteristic.");
	
	if (GameObjectSelected)
	{
		GameObjectSelected->DrawGameObjectImGui();
		GameObjectSelected->DrawComponentImGui();
	}

	ImGui::End();
}

void ModuleEngineUI::ImGuiHierarchyWindow()
{
	ImGui::Begin("Hierarchy", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar);
	//ImGui::Begin("Hierarchy", false);

	RecursiveDrawHierarchy(App->scene->GetRoot());

	ImGui::End();
}

void ModuleEngineUI::RecursiveDrawHierarchy(const GameObject* node)
{
	uint flags = 0;
	const std::vector<GameObject*>* children = node->GetChildren();

	if (node == App->scene->GetRoot())
	{
		for (std::vector<GameObject*>::const_iterator item = children->cbegin(); item != children->cend(); ++item)
			RecursiveDrawHierarchy(*item);
	}
	else
	{
		if (children->empty())
			flags |= ImGuiTreeNodeFlags_Leaf;
		if (node == GameObjectSelected)
			flags |= ImGuiTreeNodeFlags_Selected;
		if (ImGui::TreeNodeEx(node, flags, node->name.c_str()))
		{
			if (ImGui::IsItemClicked())
				GameObjectSelected = const_cast<GameObject*>(node); //Warning const cast
			for (std::vector<GameObject*>::const_iterator item = children->cbegin(); item != children->cend(); ++item)
				RecursiveDrawHierarchy(*item);
			ImGui::TreePop();
		}
	}
}

void ModuleEngineUI::ImGuiLoadFilePopUp()
{
	ImGui::OpenPopup("Load File");
	if (ImGui::BeginPopupModal("Load File", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_HorizontalScrollbar))
	{
		ImGui::BeginChild("File Browser", ImVec2(300, 300), true);
		//Iterate all Assets folder including files and directories
		if (ImGui::TreeNodeEx(App->importer->Get_Assets_path()->c_str(), 0))
		{
			RecursiveDrawDirectory(App->importer->Get_Assets_path()->c_str());
			ImGui::TreePop();
		}
		/*
		if (ImGui::TreeNodeEx(App->importer->Get_Library_path()->c_str(), 0))
		{
			RecursiveDrawDirectory(App->importer->Get_Library_path()->c_str());
			ImGui::TreePop();
		}
		*/
		/*
		if (ImGui::TreeNodeEx(App->importer->Get_Settings_path()->c_str(), 0))
		{
			RecursiveDrawDirectory(App->importer->Get_Settings_path()->c_str());
			ImGui::TreePop();
		}
		*/
		if (ImGui::TreeNodeEx(App->importer->Get_Scenes_path()->c_str(), 0))
		{
			RecursiveDrawDirectory(App->importer->Get_Scenes_path()->c_str());
			ImGui::TreePop();
		}
		ImGui::EndChild();

		char file_path[1000] = "";
		sprintf_s(file_path, 1000, "%s", LoadFileNameFileBrowser.c_str());
		ImGui::InputText("input text", file_path, 1000, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("Ok", ImVec2(50, 20)))
		{
			std::string extention = LoadFileNameFileBrowser.substr(LoadFileNameFileBrowser.rfind(".") + 1, LoadFileNameFileBrowser.length());
			//if is a mesh file load
			if ((extention == "fbx") || (extention == "FBX") || (extention == "obj") || (extention == "OBJ") || (extention == *App->importer->Get_Mesh_Extention()) || (extention == *App->importer->Get_FBXComponents_Extention()))
			{
				//Load Scene here
				/*
				TCHAR directory[MAX_PATH + 1] = "";
				DWORD len = GetCurrentDirectory(MAX_PATH, directory);
				size_t bar_pos = LoadFileNameFileBrowser.find("\\") + 1;
				LoadFileNameFileBrowser = LoadFileNameFileBrowser.substr(bar_pos, LoadFileNameFileBrowser.length());
				bar_pos = LoadFileNameFileBrowser.find("\\");
				LoadFileNameFileBrowser = LoadFileNameFileBrowser.substr(bar_pos, LoadFileNameFileBrowser.length());
				*/
				App->importer->Load(&LoadFileNameFileBrowser);
			}
			//Scene file
			else if (extention == "json")
			{
				App->scene->LoadScene(LoadFileNameFileBrowser.c_str());
			}
			showLoadFilePopUp = false;
			LoadFileNameFileBrowser.clear();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(50, 20)))
		{
			showLoadFilePopUp = false;
			LoadFileNameFileBrowser.clear();
		}
		ImGui::EndPopup();
	}
}

void ModuleEngineUI::RecursiveDrawDirectory(const char* directory)
{
	for (std::experimental::filesystem::directory_iterator::value_type file_in_path : std::experimental::filesystem::directory_iterator(directory))
	{
		char title[1000] = "";
		if (std::experimental::filesystem::is_directory(file_in_path.path()) && (file_in_path.path().string().length() < 1000))
		{
			sprintf_s(title, 1000, "%S", file_in_path.path().filename().c_str());
			if (ImGui::TreeNodeEx(title, 0))
			{
				sprintf_s(title, 1000, "%S", file_in_path.path().c_str());
				RecursiveDrawDirectory(title);
				ImGui::TreePop();
			}
		}
		if (std::experimental::filesystem::is_regular_file(file_in_path.path()) && (file_in_path.path().string().length() < 1000))
		{
			sprintf_s(title, 1000, "%S", file_in_path.path().filename().c_str());
			if (ImGui::TreeNodeEx(title, ImGuiTreeNodeFlags_Leaf))
			{
				if (ImGui::IsItemClicked())
				{
					char path[1000] = "";
					sprintf_s(path, 1000, "%S", file_in_path.path().c_str());
					LoadFileNameFileBrowser = path;
				}
				ImGui::TreePop();
			}
		}
	}
}

void ModuleEngineUI::ImGuiSaveFilePopUp()
{
	ImGui::OpenPopup("Save File");
	if (ImGui::BeginPopupModal("Save File", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_HorizontalScrollbar))
	{
		char file_name[500] = "";
		ImGui::InputText("Scene to save name", file_name, 500);
		if (strcmp(file_name, ""))
			SaveFileNameFileBrowser = file_name;

		if (ImGui::Button("Ok", ImVec2(50, 20)))
		{
			//Save Scene here
			App->scene->SaveScene(SaveFileNameFileBrowser.c_str());
			showSaveFilePopUp = false;
			SaveFileNameFileBrowser.clear();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(50, 20)))
		{
			showSaveFilePopUp = false;
			SaveFileNameFileBrowser.clear();
		}
		ImGui::EndPopup();
	}
}

void ModuleEngineUI::ImGuiTimeManager()
{
	//Set window to good position
	int w = 0; int h = 0; App->window->GetWindowSize(w, h); ImGui::SetNextWindowPos(ImVec2(w * 0.5f - 170 * 0.5f, 23)); //w/2 - time width/2, menu height
	ImGui::Begin("Time Manager", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
	//ImGui::Begin("Time Manager", false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);

	EngineTimeStatus state = App->GetEngineTimeStatus();

	if (state == EngineTimeStatus::stop)
	{
		if (ImGui::Button("Play"))
		{
			App->scene->SaveScene("EditorScene_Backup");
			App->Play();
		}
	}
	else
	{
		if (ImGui::Button("Stop"))
		{
			App->Stop();
			//Destroy all current scene
			App->scene->CleanUpScene();
			GameObjectSelected = nullptr;
			//Load Saved Scene
			App->scene->LoadEditorScene((*App->importer->Get_Scenes_path() + "\\" + "EditorScene_Backup.json").c_str());
		}
	}
	ImGui::SameLine();
	if (state == EngineTimeStatus::play_unpause)
	{
		if (ImGui::Button("Pause"))
			App->Pause();
	}
	ImGui::SameLine();
	if (state == EngineTimeStatus::play_pause)
	{
		if (ImGui::Button("Continue"))
			App->Play();
		ImGui::SameLine();
		if (ImGui::Button("Frame"))
			App->Frame();
	}
	ImGui::End();
}

void ModuleEngineUI::ImGuiCulling()
{
	ImGui::Begin("Culling", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar);
	//ImGui::Begin("Culling", false);

	if (ImGui::Button("Calc Octree") || (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN))
	{
		const std::vector<GameObject*>* vec = App->scene->GetAllSceneGameObjects();
		App->scene->scene_octree.Clear(false);
		//Clac adaptative size of scene octree
		/**/
		AABB AdaptativeAABB;
		AdaptativeAABB.SetNegativeInfinity();
		for (std::vector<GameObject*>::const_iterator item = vec->cbegin(); item != vec->cend(); ++item)
		{
			if ((*item)->IsStatic())
			{
				ComponentMesh* mesh = (ComponentMesh*)(*item)->FindComponentFirst(ComponentType::Mesh_Component);
				if (mesh != nullptr)
				{
					AABB Box;
					mesh->GetTransformedAABB(Box);
					AdaptativeAABB.Enclose(Box);
				}
			}
		}
		App->scene->scene_octree.Boundaries(AdaptativeAABB);
		/**/
		//Insert AABBs to octree
		for (std::vector<GameObject*>::const_iterator item = vec->cbegin(); item != vec->cend(); ++item)
			if ((*item)->IsStatic())
				App->scene->scene_octree.Insert(*item);
	}

	ImGui::Checkbox("Show Octree Debug", &App->scene->octree_draw);

	//Debug text about time when checking candidates, collision tests...
	if (ImGui::DragInt("Max Items", &maxItems, 1.0f, 1, 10))
		App->scene->scene_octree.limits.octreeMaxItems = maxItems;

	if (ImGui::DragFloat("Minimun Size", &minSize, 0.1f, 1.0f, 15.0f, "%.2f"))
		App->scene->scene_octree.limits.octreeMinSize = minSize;
	ImGui::Text("Advice: If you want to see octree debug\ninformation, go to camera component information");
	ImGui::End();
}

void ModuleEngineUI::ImGuiResources()
{
	ImGui::Begin("Resources", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar);
	//ImGui::Begin("Resources", false);

	ImGui::BeginChild("Resources Created", ImVec2(0, 200), true, ImGuiWindowFlags_HorizontalScrollbar);
	const std::map<uint, Resource*>* resources = App->resources->GetResourcesMap();
	for (std::map<uint, Resource*>::const_iterator item = resources->cbegin(); item != resources->cend(); ++item)
	{
		switch (item->second->GetType())
		{
		case Resource::Type::mesh: ImGui::Text(((ResourceMesh*)item->second)->SimpleMeshDataStruct.Mesh_File.c_str()); break;
		case Resource::Type::texture: ImGui::Text(((ResourceTexture*)item->second)->TextureDataStruct.texture_name_UI.c_str()); break;
		case Resource::Type::null: break;
		}
		if (ImGui::IsItemClicked())
			ResouceSelected = item._Ptr->_Myval.second;
	}
	ImGui::EndChild();
	ImGui::BeginChild("Resources Options", ImVec2(0, 200), true, ImGuiWindowFlags_HorizontalScrollbar);
	//ImGui::Text("Here will be import settings of clicked\nresource above.");
	if (ResouceSelected != nullptr)
		ResouceSelected->DrawResourceOptionsAndData();
	ImGui::EndChild();
	
	ImGui::End();
}

void ModuleEngineUI::PushNewConsoleLabel(std::string* newlabel)
{
	ScrollDownConsole = true;
	console_logs.push_back(*newlabel);
}

void ModuleEngineUI::GetEvent(SDL_Event* event)
{
	if(event != nullptr)
		ImGui_ImplSdlGL3_ProcessEvent(event);
}

bool ModuleEngineUI::IsActive()
{
	return active;
}

bool ModuleEngineUI::PlotsFreezed() const
{
	return freezeplots;
}

bool ModuleEngineUI::SaveConf(JSON_Object * conf) const
{
	App->parsonjson->SetBool(conf, "startAsGame", startAsGame);
	return true;
}

bool ModuleEngineUI::LoadConf(JSON_Object * conf)
{
	startAsGame = App->parsonjson->GetBool(conf, "startAsGame", false);
	if (startAsGame)
		showMenuBar = showHierarchyWindow = showModuleVariablesWindow = showInspectorWindow = showConsoleWindow = showConfigurationWindow = showProfilerWindow = showTimeManager = false;
	else
		showMenuBar = showHierarchyWindow = showModuleVariablesWindow = showInspectorWindow = showConsoleWindow = showTimeManager = true;
	return true;
}

void ModuleEngineUI::PushFPSandMSPlot(uint fps, uint ms, uint mem)
{
	if (freezeplots)
		return;

	static uint count = 0;

	if (count >= FPS_AND_MS_PLOT_DATA_LENGTH)
		for (int i = 0; i < FPS_AND_MS_PLOT_DATA_LENGTH - 1; i++)
		{
			fpsPlotData[i] = fpsPlotData[i + 1];
			msPlotData[i] = msPlotData[i + 1];
			memPlotData[i] = memPlotData[i + 1];
		}
	else
		count++;

	fpsPlotData[count - 1] = fps;
	msPlotData[count - 1] = ms;
	memPlotData[count - 1] = mem;
}

void ModuleEngineUI::SetSelectedInspectorGO(GameObject* selected)
{
	GameObjectSelected = selected;
}

const GameObject* ModuleEngineUI::GetSelectedGameObject() const
{
	return GameObjectSelected;
}