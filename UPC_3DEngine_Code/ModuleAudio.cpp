#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleAudio::ModuleAudio(Application* app, bool start_enabled) : Module(app, start_enabled), music(NULL)
{
	name = "audio";
}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before render is available
bool ModuleAudio::Init()
{
	LOGP("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOGP("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// load support for the OGG format
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOGP("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		ret = false;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOGP("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		ret = false;
	}

	return true;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	LOGP("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	std::list<Mix_Chunk*>::iterator item;

	for(item = fx.begin(); item != fx.end(); item = item++)
	{
		Mix_FreeChunk(item._Ptr->_Myval);
	}

	fx.clear();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	return true;
}

// Play a music file
bool ModuleAudio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;
	
	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic((int) (fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOGP("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				LOGP("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOGP("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOGP("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int ModuleAudio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOGP("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}

// Play WAV
bool ModuleAudio::PlayFx(unsigned int id, int repeat)
{
	/*
	bool ret = false;

	Mix_Chunk* chunk = NULL;
	
	if(fx.at(id-1, chunk) == true)
	{
		Mix_PlayChannel(-1, chunk, repeat);
		ret = true;
	}

	return ret;
	*/

	bool ret = false;

	if (id > 0 && id <= fx.size())
	{
		int n_mus = 0;
		std::list<Mix_Chunk*>::iterator item = fx.begin();
		for (; item != fx.cend() && n_mus<id - 1; ++item, ++n_mus) {

		}

		Mix_PlayChannel(-1, (*item), repeat);
	}

	return ret;
}

void ModuleAudio::VolumeMusic(int volume)
{
	if ((music != NULL) && (volume >= 0) && (volume <= 100))
	{
		LOGP("volume was    : %d\n", Mix_VolumeMusic(MIX_MAX_VOLUME / 2));
		Mix_VolumeMusic(volume);
		LOGP("volume is now : %d\n", Mix_VolumeMusic(-1));
	}
}

bool ModuleAudio::SaveConf(JSON_Object * conf) const
{
	App->parsonjson->SetInt(conf, "Volume", volume);
	return true;
}

bool ModuleAudio::LoadConf(JSON_Object * conf)
{
	volume = App->parsonjson->GetInt(conf, "Volume", 100);
	return true;
}

void ModuleAudio::ImGuiModuleVariables()
{
	ImGui::SliderInt("Volume", &volume, 0, 100);
}
