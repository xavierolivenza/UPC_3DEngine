#include <fstream>

#include "ImporterMaterial.h"
#include "ResourceTexture.h"

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

bool ImporterMaterial::Save(const std::string* texture_name, std::string& loaded_file, bool Reimporting)
{
	//Check if the file exists
	size_t bar_pos = texture_name->rfind("\\");
	std::string tex_path = *App->importer->Get_Library_material_path() + texture_name->substr(bar_pos, texture_name->rfind(".") - bar_pos);
	tex_path += ".dds";

	LOGP("Importing dds process start, to file: %s", tex_path.c_str());

	if (!Reimporting)
	{
		FILE* file = fopen(tex_path.c_str(), "r");
		if (file != nullptr)
		{
			fclose(file);
			LOGP("DDS file already exists: %s", tex_path.c_str());
			loaded_file = tex_path;
			return true;
		}
	}
	
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
				{
					outfile.write((char*)data, size);
					loaded_file = tex_name;
				}
				else
					LOGP("Failed to write the file %s", tex_path.c_str());
				outfile.close();
			}
			RELEASE_ARRAY(data);
		}
		ilDeleteImages(1, &imgID);
	}
	else
		return false;

	return true;
}

bool ImporterMaterial::Load(MaterialData& DataMaterial, std::string* file_to_load)
{
	int tex_id = LoadImageFromFile(DataMaterial, file_to_load);
	if(tex_id > 0)
		DataMaterial.id_texture = tex_id;
	return false;
}

// Function load a image, turn it into a texture, and return the texture ID as a GLuint for use
int ImporterMaterial::LoadImageFromFile(MaterialData& DataMaterial, std::string* file_to_load)
{
	// Create an image ID as a ULuint
	ILuint imageID;
	// Create a texture ID as a GLuint
	GLuint textureID;
	// Create a flag to keep track of success/failure
	ILboolean success;
	// Create a flag to keep track of the IL error state
	ILenum error;
	// Generate the image ID
	ilGenImages(1, &imageID);
	// Bind the image
	ilBindImage(imageID);
	// Load the image file
	TCHAR directory[MAX_PATH + 1] = "";
	DWORD len = GetCurrentDirectory(MAX_PATH, directory);
	std::string texture_path = directory;

	size_t bar_pos = texture_path.rfind("\\") + 1;
	texture_path = texture_path.substr(0, bar_pos);

	LOGP("Starting loading texture directory: %s", texture_path.c_str());

	texture_path += file_to_load->substr(2, file_to_load->length());
	success = ilLoadImage(texture_path.c_str());
	DataMaterial.texture_name = texture_path;

	LOGP("Starting loading texture file_to_load: %s", file_to_load->c_str());
	LOGP("Starting loading texture: %s", texture_path.c_str());

	// If we managed to load the image, then we can start to do things with it...
	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		DataMaterial.texture_w = ImageInfo.Width;
		DataMaterial.texture_h = ImageInfo.Height;
		DataMaterial.texture_d = ImageInfo.Depth;

		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			LOGP("Image conversion failed - IL reports error: %i - %s", error, iluErrorString(error));
			return -1;
		}

		// Generate a new texture
		glGenTextures(1, &textureID);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// Set texture interpolation method to use linear interpolation (no MIPMAPS)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
			0,				// Pyramid level (for mip-mapping) - 0 is the top level
			ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
			ilGetInteger(IL_IMAGE_WIDTH),	// Image width
			ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
			0,				// Border width in pixels (can either be 1 or 0)
			ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
			GL_UNSIGNED_BYTE,		// Image data type
			ilGetData());			// The actual image data itself

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else // If we failed to open the image file in the first place...
	{
		error = ilGetError();
		LOGP("Image load failed - IL reports error: %i - %s", error, iluErrorString(error));
		return -1;
	}

	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

	LOGP("Texture creation successful.");

	return textureID; // Return the GLuint to the texture so you can use it!
}

bool ImporterMaterial::DevilCanLoad(const char* file)
{
	//Serialize MaterialData to file
	uint imgID = 0;
	ilGenImages(1, &imgID);
	ilBindImage(imgID);
	ILboolean success = ilLoadImage(file);
	if (success == IL_TRUE)
	{
		ilDeleteImages(1, &imgID);
		return true;
	}
	return false;
}