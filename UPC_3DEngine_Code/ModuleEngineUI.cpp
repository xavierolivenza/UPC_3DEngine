#include "Globals.h"
#include "Application.h"
#include "ModuleEngineUI.h"

ModuleEngineUI::ModuleEngineUI(Application* app, bool start_enabled) : Module(app, start_enabled),
	fpsPlotData(FPS_AND_MS_PLOT_DATA_LENGTH), msPlotData(FPS_AND_MS_PLOT_DATA_LENGTH)
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
			App->WantToClose();
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
	const PerformanceStruct* PerformanceData = App->GetPerformanceStruct();
	PushFPSandMSPlot(PerformanceData->frames_last_second, PerformanceData->miliseconds_per_frame);
	ImGui::Begin("Configuration", false, ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize);
	if (ImGui::CollapsingHeader("Application"))
	{
		//App name
		static char appnamestr[128] = "";
		if (ImGui::InputText("App name", appnamestr, 128, ImGuiInputTextFlags_EnterReturnsTrue))
			App->window->SetTitle(appnamestr);

		//Max FPS Slider
		static int MaxFPSValue = 0;
		ImGui::SliderInt("Max FPS", &MaxFPSValue, 0, 144);

		//Some useful variables
		uint title_size = 50;
		char title[50];
		
		//Framerate PlotHistogram
		sprintf_s(title, title_size, "Framerate: %i", PerformanceData->frames_last_second);
		ImGui::PlotHistogram("##Framerate", &fpsPlotData[0], fpsPlotData.size(), 0, title, 0.0f, 150.0f, ImVec2(310, 100));

		//Miliseconds PlotHistogram
		sprintf_s(title, title_size, "Frame Miliseconds: %i", PerformanceData->miliseconds_per_frame);
		ImGui::PlotHistogram("##Frame Miliseconds", &msPlotData[0], msPlotData.size(), 0, title, 0.0f, 50.0f, ImVec2(310, 100));

		//Memory Consumption PlotHistogram


		//Memory data
		sMStats MemoryStats = m_getMemoryStatistics();
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
		static int ScreenW = 0;
		ImGui::SliderInt("Screen Width", &ScreenW, 0, 1920);
		static int ScreenH = 0;
		ImGui::SliderInt("Screen Height", &ScreenH, 0, 1080);
		static bool fullscreen = false;
		ImGui::Checkbox("Fullscreen", &fullscreen);
		static bool resizable = false;
		ImGui::SameLine();
		ImGui::Checkbox("Resizable", &resizable);
		static bool borderless = false;
		ImGui::Checkbox("Borderless", &borderless);

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

	//------------------------------------------//
	//-----------------Console------------------//
	//------------------------------------------//
	ImGui::Begin("Console", false, ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize);
	//ImGui::Begin("Console", false, ImGuiWindowFlags_NoResize);
	for (std::list<std::string>::iterator item = console_logs.begin(); item != console_logs.cend(); ++item)
		ImGui::Text(item._Ptr->_Myval.c_str());
	ImGui::SetScrollHere();
	ImGui::Separator();
	static char str0[128] = "";
	if (ImGui::InputText("input text", str0, 128, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		LOGP(str0);
		strcpy(str0, "");
	}
	ImGui::End();

	//------------------------------------------//
	//-----------------Rand Gen-----------------//
	//------------------------------------------//
	ImGui::Begin("Random generator", false, ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize);
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

void ModuleEngineUI::PushFPSandMSPlot(uint fps, uint ms)
{
	static uint count = 0;

	if (count >= FPS_AND_MS_PLOT_DATA_LENGTH)
		for (int i = 0; i < FPS_AND_MS_PLOT_DATA_LENGTH - 1; i++)
		{
			fpsPlotData[i] = fpsPlotData[i + 1];
			msPlotData[i] = msPlotData[i + 1];
		}
	else
		count++;

	fpsPlotData[count - 1] = fps;
	msPlotData[count - 1] = ms;
}