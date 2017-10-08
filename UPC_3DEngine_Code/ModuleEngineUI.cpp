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
	ImGuiDrawMenuBar();
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) showProfilerWindow = !showProfilerWindow;
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) showModuleVariablesWindow = !showModuleVariablesWindow;
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) showConfigurationWindow = !showConfigurationWindow;
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) showConsoleWindow = !showConsoleWindow;
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) showPropertiesWindow = !showPropertiesWindow;

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
		ImGui::MenuItem("Profiler", "1", &showProfilerWindow);
		ImGui::MenuItem("Module Variables", "2", &showModuleVariablesWindow);
		ImGui::MenuItem("Configuration", "3", &showConfigurationWindow);
		ImGui::MenuItem("Console", "4", &showConsoleWindow);
		ImGui::MenuItem("Properties", "5", &showPropertiesWindow);
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
	ImGui::Begin("Configuration", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	//ImGui::Begin("Configuration", false);
	if (ImGui::CollapsingHeader("Application"))
	{
		//App name
		if (ImGui::InputText("App name", appnamestr, 128, ImGuiInputTextFlags_EnterReturnsTrue))
			App->window->SetTitle(appnamestr);

		//Max FPS Slider
		ImGui::SliderInt("Max FPS", &App->window->MaxFPSValue, 0, 144);

		ImGui::Separator();

		//Some useful variables
		uint title_size = 50;
		char title[50];

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
		sprintf_s(title, title_size, "SDL Minor Version: %i", SDLVersion.minor);
		ImGui::Text(title);
		sprintf_s(title, title_size, "SDL Major Version: %i", SDLVersion.major);
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
		if (SDL_Has3DNow()) ImGui::Text("3D Now Avalible");
		else ImGui::Text("3D Now Unavalible");
		if (SDL_HasAVX()) ImGui::Text("AVX Avalible");
		else ImGui::Text("AVX Unavalible");
		//if (SDL_HasAVX2()) ImGui::Text("AVX2 Avalible");
		//else ImGui::Text("AVX2 Unavalible");
		if (SDL_HasAltiVec()) ImGui::Text("AltiVec Avalible");
		else ImGui::Text("AltiVec Unavalible");
		if (SDL_HasMMX()) ImGui::Text("MMX Avalible");
		else ImGui::Text("MMX Unavalible");
		if (SDL_HasRDTSC()) ImGui::Text("RDTSC Avalible");
		else ImGui::Text("RDTSC Unavalible");
		if (SDL_HasSSE()) ImGui::Text("SSE Avalible");
		else ImGui::Text("SSE Unavalible");
		if (SDL_HasSSE2()) ImGui::Text("SSE2 Avalible");
		else ImGui::Text("SSE2 Unavalible");
		if (SDL_HasSSE3()) ImGui::Text("SSE3 Avalible");
		else ImGui::Text("SSE3 Unavalible");
		if (SDL_HasSSE41()) ImGui::Text("SSE41 Avalible");
		else ImGui::Text("SSE41 Unavalible");
		if (SDL_HasSSE42()) ImGui::Text("SSE42 Avalible");
		else ImGui::Text("SSE42 Unavalible");

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
		sprintf_s(title, title_size, "GL_Versions: %s", GPUData);
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
	uint title_size = 50;
	char title[50];

	for (std::list<Module*>::const_iterator item = App->GetModuleList()->begin(); item != App->GetModuleList()->cend(); ++item)
	{
		sprintf_s(title, title_size, "Module: %s", item._Ptr->_Myval->name.c_str());
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
	//float MaxScrollY = ImGui::GetScrollMaxX();
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
	ImGui::Begin("Properties", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	//ImGui::Begin("Properties", false);
	if (ImGui::CollapsingHeader("Transformation"))
	{
		char* data = nullptr;

		ImGui::Text("Position");
		data = "315434";
		if (ImGui::InputText("PosX", data, 50, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
		{

		}
		data = "315434";
		if (ImGui::InputText("PosY", data, 50, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
		{

		}
		data = "315434";
		if (ImGui::InputText("PosZ", data, 50, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
		{

		}

		ImGui::Text("Rotation");
		data = "315434";
		if (ImGui::InputText("RotX", data, 50, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
		{

		}
		data = "315434";
		if (ImGui::InputText("RotY", data, 50, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
		{

		}
		data = "315434";
		if (ImGui::InputText("RotZ", data, 50, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
		{

		}

		ImGui::Text("Scale");
		data = "315434";
		if (ImGui::InputText("ScaX", data, 50, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
		{

		}
		data = "315434";
		if (ImGui::InputText("ScaY", data, 50, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
		{

		}
		data = "315434";
		if (ImGui::InputText("ScaZ", data, 50, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
		{

		}

		/*
		static char str0[50] = "";
		static char str1[25] = "";

		strcpy(str0, itoa(w_width, str1, 10));
		if (ImGui::InputText("w_width", str0, 50, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal))
			w_width = atoi(str0);
		*/
	}
	if (ImGui::CollapsingHeader("Geometry"))
	{

	}
	if (ImGui::CollapsingHeader("Texture"))
	{

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