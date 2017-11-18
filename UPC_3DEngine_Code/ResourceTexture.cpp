#include "ResourceTexture.h"
#include "Application.h"
#include "ModuleParson.h"

ResourceTexture::ResourceTexture() : Resource(Resource::Type::texture)
{

}

ResourceTexture::~ResourceTexture()
{

}

void ResourceTexture::CleanResource()
{
	TextureDataStruct.Clean();
}

void ResourceTexture::DrawResourceOptionsAndData()
{
	char title[50] = "";
	sprintf_s(title, 50, "UUID: %i", uuid);
	ImGui::Text(title);
	ImGui::Text(file.c_str());
	ImGui::Text(exported_file.c_str());
	ImGui::Text(file_date.c_str());
	sprintf_s(title, 50, "Type: %i", type);
	ImGui::Text(title);
	sprintf_s(title, 50, "Used: %i", loaded);
	ImGui::Text(title);

	static int WrappingMethodPick = 0;
	if (ImGui::Combo("Wrapping Method", &WrappingMethodPick, "GL_CLAMP\0GL_REPEAT\0GL_MIRRORED_REPEAT\0GL_CLAMP_TO_EDGE\0GL_CLAMP_TO_BORDER\0\0"))
	{
		switch (WrappingMethodPick)
		{
		case 0: TextureDataStruct.WrappingMethod = GL_CLAMP; break;
		case 1: TextureDataStruct.WrappingMethod = GL_REPEAT; break;
		case 2: TextureDataStruct.WrappingMethod = GL_MIRRORED_REPEAT; break;
		case 3: TextureDataStruct.WrappingMethod = GL_CLAMP_TO_EDGE; break;
		case 4: TextureDataStruct.WrappingMethod = GL_CLAMP_TO_BORDER; break;
		}
	}

	if (ImGui::Combo("Interpolation Method", &InterpolationMethodPick, "GL_NEAREST\0GL_LINEAR\0\0"))
	{
		switch (InterpolationMethodPick)
		{
		case 0: TextureDataStruct.InterpolationMethod = GL_NEAREST; break;
		case 1: TextureDataStruct.InterpolationMethod = GL_LINEAR; break;
		}
	}

	if (ImGui::Combo("Compressing Method", &CompressingMethodPick, "IL_DXT1\0IL_DXT2\0IL_DXT3\0IL_DXT4\0IL_DXT5\0\0"))
	{
		switch (CompressingMethodPick)
		{
		case 0: TextureDataStruct.CompressingMethod = IL_DXT1; break;
		case 1: TextureDataStruct.CompressingMethod = IL_DXT2; break;
		case 2: TextureDataStruct.CompressingMethod = IL_DXT3; break;
		case 3: TextureDataStruct.CompressingMethod = IL_DXT4; break;
		case 4: TextureDataStruct.CompressingMethod = IL_DXT5; break;
		}
	}

	if (ImGui::Button("Import"))
	{
		//Reimport with new variables
		App->resources->ReimportResource(*this, TextureDataStruct.CompressingMethod);
	}
}

void ResourceTexture::Save(JSON_Object* conf) const
{
	App->parsonjson->SetUInt(conf, "uuid", uuid);
	App->parsonjson->SetString(conf, "file_assets", file.c_str());
	App->parsonjson->SetString(conf, "file_library", exported_file.c_str());
	App->parsonjson->SetString(conf, "file_time", file_date.c_str());
	App->parsonjson->SetUInt(conf, "type", type);

	App->parsonjson->SetInt(conf, "WrappingMethod", TextureDataStruct.WrappingMethod);
	App->parsonjson->SetInt(conf, "InterpolationMethod", TextureDataStruct.InterpolationMethod);
	App->parsonjson->SetInt(conf, "CompressingMethod", TextureDataStruct.CompressingMethod);
}

void ResourceTexture::Load(JSON_Object* conf)
{
	uuid = App->parsonjson->GetUInt(conf, "uuid", 0);
	file = App->parsonjson->GetString(conf, "file_assets", "");
	exported_file = App->parsonjson->GetString(conf, "file_library", "");
	type = (Resource::Type)App->parsonjson->GetUInt(conf, "type", Resource::Type::null);
	file_date = App->parsonjson->GetString(conf, "file_time", "");

	TextureDataStruct.WrappingMethod = App->parsonjson->GetInt(conf, "WrappingMethod", GL_CLAMP);
	switch (TextureDataStruct.WrappingMethod)
	{
	case GL_CLAMP: InterpolationMethodPick = 0; break;
	case GL_REPEAT: InterpolationMethodPick = 1; break;
	case GL_MIRRORED_REPEAT: InterpolationMethodPick = 2; break;
	case GL_CLAMP_TO_EDGE: InterpolationMethodPick = 3; break;
	case GL_CLAMP_TO_BORDER: InterpolationMethodPick = 4; break;
	}
	TextureDataStruct.InterpolationMethod = App->parsonjson->GetInt(conf, "InterpolationMethod", GL_LINEAR);
	switch (TextureDataStruct.WrappingMethod)
	{
	case GL_NEAREST: InterpolationMethodPick = 0; break;
	case GL_LINEAR: InterpolationMethodPick = 1; break;
	}
	TextureDataStruct.CompressingMethod = App->parsonjson->GetInt(conf, "CompressingMethod", IL_DXT5);
	switch (TextureDataStruct.WrappingMethod)
	{
	case IL_DXT1: InterpolationMethodPick = 0; break;
	case IL_DXT2: InterpolationMethodPick = 1; break;
	case IL_DXT3: InterpolationMethodPick = 2; break;
	case IL_DXT4: InterpolationMethodPick = 3; break;
	case IL_DXT5: InterpolationMethodPick = 4; break;
	}
	//App->resources->ReimportResource(*this, TextureDataStruct.CompressingMethod);
}