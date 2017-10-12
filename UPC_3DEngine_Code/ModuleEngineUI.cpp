#include "Globals.h"
#include "Application.h"
#include "ModuleEngineUI.h"

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

	if (SDL_Has3DNow()) Modes3D += std::string("3D Now Avalible \n");
	else Modes3D += std::string("3D Now Unavalible \n");
	if (SDL_HasAVX()) Modes3D += std::string("AVX Avalible \n");
	else Modes3D += std::string("AVX Unavalible \n");
	//if (SDL_HasAVX2()) Modes3D += std::string("AVX2 Avalible \n");
	//else Modes3D += std::string("AVX2 Unavalible \n");
	if (SDL_HasAltiVec()) Modes3D += std::string("AltiVec Avalible \n");
	else Modes3D += std::string("AltiVec Unavalible \n");
	if (SDL_HasMMX()) Modes3D += std::string("MMX Avalible \n");
	else Modes3D += std::string("MMX Unavalible \n");
	if (SDL_HasRDTSC()) Modes3D += std::string("RDTSC Avalible \n");
	else Modes3D += std::string("RDTSC Unavalible \n");
	if (SDL_HasSSE()) Modes3D += std::string("SSE Avalible \n");
	else Modes3D += std::string("SSE Unavalible \n");
	if (SDL_HasSSE2()) Modes3D += std::string("SSE2 Avalible \n");
	else Modes3D += std::string("SSE2 Unavalible \n");
	if (SDL_HasSSE3()) Modes3D += std::string("SSE3 Avalible \n");
	else Modes3D += std::string("SSE3 Unavalible \n");
	if (SDL_HasSSE41()) Modes3D += std::string("SSE41 Avalible \n");
	else Modes3D += std::string("SSE41 Unavalible \n");
	if (SDL_HasSSE42()) Modes3D += std::string("SSE42 Avalible \n");
	else Modes3D += std::string("SSE42 Unavalible \n");

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

	style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.412f, 0.412f, 0.412f, 0.70f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.70f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.85f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.80f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.098f, 0.098f, 0.439f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.098f, 0.098f, 0.439f, 0.70f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.416f, 0.353f, 0.804f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.416f, 0.353f, 0.804f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.8f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.098f, 0.098f, 0.439f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.416f, 0.353f, 0.804f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.098f, 0.098f, 0.450f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.098f, 0.098f, 0.439f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.000f, 0.000f, 0.502f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.000f, 0.000f, 0.502f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.80f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.0f, 0.549f, 0.0f, 0.85f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 1.0f, 0.0f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.000f, 0.549f, 0.000f, 0.85f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.098f, 0.098f, 0.439f, 0.80f);
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
	//------------------------------------------//
	//-----------------Menu Bar-----------------//
	//------------------------------------------//
	ImGuiDrawMenuBar();
	if ((App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) showProfilerWindow = !showProfilerWindow;
	if ((App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) showModuleVariablesWindow = !showModuleVariablesWindow;
	if ((App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) showConfigurationWindow = !showConfigurationWindow;
	if ((App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) showConsoleWindow = !showConsoleWindow;
	if ((App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) showPropertiesWindow = !showPropertiesWindow;
	
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
	//------------Properties Window-------------//
	//------------------------------------------//
	if (showPropertiesWindow)
		ImGuiPropertiesWindow();

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
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View"))
	{
		ImGui::MenuItem("Profiler", "LALT + 1", &showProfilerWindow);
		ImGui::MenuItem("Module Variables", "LALT + 2", &showModuleVariablesWindow);
		ImGui::MenuItem("Configuration", "LALT + 3", &showConfigurationWindow);
		ImGui::MenuItem("Console", "LALT + 4", &showConsoleWindow);
		ImGui::MenuItem("Properties", "LALT + 5", &showPropertiesWindow);
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
				ImGui::Text("SDL 2.0.3");
				ImGui::Text("ImGui 1.51");
				ImGui::Text("MathGeoLib 1.5");
				ImGui::Text("mmgr");
				ImGui::Text("parson");
				ImGui::Text("Assimp 3.3.1");
				ImGui::Text("DevIl 1.8.0");
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
	ImGui::Begin("Configuration", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar);
	//ImGui::Begin("Configuration", false);
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
		char title[50];
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
		uint title_size = 50;
		char title[50];
		SDL_version SDLVersion;

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

		sprintf_s(title, title_size, "CPU Cache Line Size: %i", SDL_GetCPUCacheLineSize());
		ImGui::Text(title);
		sprintf_s(title, title_size, "CPU Cores: %i", SDL_GetCPUCount());
		ImGui::Text(title);

		ImGui::Text(Modes3D.c_str());

		ImGui::Separator();

		sprintf_s(title, title_size, "RAM: %i", SDL_GetSystemRAM());
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
	}
	ImGui::End();
}

void  ModuleEngineUI::ImGuiProfierWindow()
{
	ImGui::Begin("Profiler", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	//ImGui::Begin("Profiler", false);
	uint title_size = 50;
	char title[50];
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
	ImGui::Begin("Console", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	//ImGui::Begin("Console", false);
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
	ImGui::End();
}

void ModuleEngineUI::ImGuiPropertiesWindow()
{
	ImGui::Begin("Properties", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar);
	//ImGui::Begin("Properties", false);
	ImGui::Text("If ther is geometry loaded, here will appear\nPosition/Rotation/Scale of the loaded meshes.");
	ImGui::Text("If one of the IDs/Paths below is zero/null,\nit means that it does not have\nthat characteristic.");
	
	char data[100] = "";
	uint data_size = sizeof data;

	for (std::vector<GeometryData>::const_iterator item = App->loadmesh->GetGeometryStructVector()->cbegin(); item != App->loadmesh->GetGeometryStructVector()->cend(); ++item)
	{
		if (ImGui::CollapsingHeader(item._Ptr->name.c_str()))
		{
			sprintf_s(data, data_size, "Transformation##%s", item._Ptr->name.c_str());
			if (ImGui::CollapsingHeader(data))
			{
				//This all are input text, ready to be edited in future updates, by now, read only.
				//sprintf_s(data, data_size, "Position##%s", item._Ptr->name.c_str());
				ImGui::Text("Position");
				sprintf_s(data, data_size, "%f", item._Ptr->pos.x);
				if (ImGui::InputText("PosX", data, data_size, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
				{

				}
				sprintf_s(data, data_size, "%f", item._Ptr->pos.y);
				if (ImGui::InputText("PosY", data, data_size, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
				{

				}
				sprintf_s(data, data_size, "%f", item._Ptr->pos.z);
				if (ImGui::InputText("PosZ", data, data_size, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
				{

				}
				//sprintf_s(data, data_size, "Rotation##%s", item._Ptr->name.c_str());
				ImGui::Text("Rotation");
				float3 rot = item._Ptr->rot.ToEulerXYZ();
				rot *= RADTODEG;
				sprintf_s(data, data_size, "%f", rot.x);
				if (ImGui::InputText("RotX", data, data_size, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
				{

				}
				sprintf_s(data, data_size, "%f", rot.y);
				if (ImGui::InputText("RotY", data, data_size, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
				{

				}
				sprintf_s(data, data_size, "%f", rot.z);
				if (ImGui::InputText("RotZ", data, data_size, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
				{

				}
				//sprintf_s(data, data_size, "Scale##%s", item._Ptr->name.c_str());
				ImGui::Text("Scale");
				sprintf_s(data, data_size, "%f", item._Ptr->scale.x);
				if (ImGui::InputText("ScaX", data, data_size, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
				{

				}
				sprintf_s(data, data_size, "%f", item._Ptr->scale.y);
				if (ImGui::InputText("ScaY", data, data_size, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
				{

				}
				sprintf_s(data, data_size, "%f", item._Ptr->scale.z);
				if (ImGui::InputText("ScaZ", data, data_size, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
				{

				}
			}
			sprintf_s(data, data_size, "Geometry##%s", item._Ptr->name.c_str());
			if (ImGui::CollapsingHeader(data))
			{
				sprintf_s(data, data_size, "Num Faces: %i", item._Ptr->num_faces);
				ImGui::Text(data);
				sprintf_s(data, data_size, "Num Vertices: %i", item._Ptr->num_vertices);
				ImGui::Text(data);
				sprintf_s(data, data_size, "Num Indices: %i", item._Ptr->num_indices);
				ImGui::Text(data);
				sprintf_s(data, data_size, "ID Vertices: %i", item._Ptr->id_vertices);
				ImGui::Text(data);
				sprintf_s(data, data_size, "ID Indices: %i", item._Ptr->id_indices);
				ImGui::Text(data);
				sprintf_s(data, data_size, "ID Normals: %i", item._Ptr->id_normals);
				ImGui::Text(data);
				sprintf_s(data, data_size, "ID Colors: %i", item._Ptr->id_colors);
				ImGui::Text(data);
				sprintf_s(data, data_size, "ID Texture Coords: %i", item._Ptr->id_texture_coords);
				ImGui::Text(data);
			}
			sprintf_s(data, data_size, "Texture##%s", item._Ptr->name.c_str());
			if (ImGui::CollapsingHeader(data))
			{
				std::string path;
				ImGui::Separator();
				path = "Texture Path: ";
				path += item._Ptr->texture_name;
				ImGui::Text(path.c_str());
				sprintf_s(data, data_size, "Texture ID: %i", item._Ptr->id_texture);
				ImGui::Text(data);
				sprintf_s(data, data_size, "Texture Width: %i", item._Ptr->texture_w);
				ImGui::Text(data);
				sprintf_s(data, data_size, "Texture Height: %i", item._Ptr->texture_h);
				ImGui::Text(data);
			}
		}
	}

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