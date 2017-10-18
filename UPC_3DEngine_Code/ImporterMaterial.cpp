#include "ImporterMaterial.h"

#include "DevIL\include\il.h"
#include "DevIL\include\ilu.h"
#include "DevIL\include\ilut.h"
#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")

ImporterMaterial::ImporterMaterial()
{
	
}

ImporterMaterial::~ImporterMaterial()
{
	
}

bool ImporterMaterial::Init()
{
	bool ret = true;
	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilClearColour(255, 255, 255, 000);
	ILenum ilError = ilGetError();
	if (ilError != IL_NO_ERROR)
		ret = false;
	return ret;
}

bool ImporterMaterial::CleanUp()
{
	ilShutDown();
	return true;
}

bool ImporterMaterial::Save(const MaterialData* DataMaterial, std::string* file_to_save)
{
	if ((DataMaterial == nullptr) || (file_to_save == nullptr) || file_to_save->empty())
		return false;

	//Serialize MaterialData to file

	return false;
}

bool ImporterMaterial::Load(MaterialData& DataMaterial, std::string* file_to_load)
{
	return false;
}