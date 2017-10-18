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

bool ImporterMaterial::Import(std::string* file_to_import, std::string& output_file)
{
	if ((file_to_import == nullptr) || file_to_import->empty())
		return false;

	//TODO Generate ComponentMesh here, send to Save

	return false;
}

bool ImporterMaterial::Save(const Component* component, std::string* file_to_save)
{
	if ((component == nullptr) || (file_to_save == nullptr) || file_to_save->empty())
		return false;

	ComponentMaterial* materialComponent = (ComponentMaterial*)component;

	//Serialize ComponentMaterial to file

	return false;
}

Component* ImporterMaterial::Load(std::string* file_to_load)
{
	return nullptr;
}