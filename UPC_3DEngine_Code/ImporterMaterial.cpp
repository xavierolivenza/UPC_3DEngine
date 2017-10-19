#include <fstream>

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

bool ImporterMaterial::Save(const std::string* texture_name, std::string* file_to_save)
{
	if ((texture_name == nullptr) || (file_to_save == nullptr) || file_to_save->empty())
		return false;

	//Serialize MaterialData to file

	uint imgID = 0;
	ilGenImages(1, &imgID);
	ilBindImage(imgID);
	ILboolean success = ilLoadImage(texture_name->c_str());
	if (success == IL_TRUE)
	{
		ILuint size;
		ILubyte *data;
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // To pick a specific DXT compression use
		size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
		if (size > 0)
		{
			data = new ILubyte[size]; // allocate buffer
			if (ilSaveL(IL_DDS, data, size) > 0)// Save to buffer
			{
				size_t bar_pos = texture_name->rfind("\\");
				std::string tex_name = texture_name->substr(bar_pos, texture_name->rfind(".") - bar_pos);
				tex_name += ".dds";
				std::string tex_path = *App->importer->Get_Library_material_path() + tex_name;

				std::ofstream outfile(tex_path, std::ofstream::binary);
				if (outfile.good()) //write file
					outfile.write((char*)data, size);
				else
					LOGP("Failed to write the file %s", tex_path.c_str());
				outfile.close();
			}
			RELEASE_ARRAY(data);
		}
		ilDeleteImages(1, &imgID);
	}

	return false;
}

bool ImporterMaterial::Load(MaterialData& DataMaterial, std::string* file_to_load)
{
	return false;
}