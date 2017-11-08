#include "Resource.h"
#include "Application.h"
#include "ImporterMaterial.h"

Resource::Resource(Resource::Type type) : type(type)
{
	LCG UUIDGen;
	uuid = UUIDGen.Int();
}

Resource::~Resource()
{

}

uint Resource::GetUID() const
{
	return uuid;
}

Resource::Type Resource::GetType() const
{
	return type;
}

const char* Resource::GetTypeStr() const
{
	switch (type)
	{
		case Resource::Type::mesh: return "Resource Mesh"; break;
		case Resource::Type::texture: return "Resource Texture"; break;
		case Resource::Type::null: return "Resource Null"; break;
	}
	return nullptr;
}

const std::string& Resource::GetFile() const
{
	return file;
}

bool Resource::LoadResource()
{
	if (loaded > 0) loaded++;
	else loaded = LoadResourceToMemory();
	return loaded > 0;
}

void Resource::Save(JSON_Object* conf) const
{

}

void Resource::Load(JSON_Object* conf)
{

}

bool Resource::LoadResourceToMemory()
{
	bool ret = false;
	switch (type)
	{
	case Resource::Type::mesh:
		//Clean resource if you are reimporting this
		//Call mesh importer with this ppinter as reference
		//App->importer->LoadSimpleMesh(&this->exported_file, this->DataMesh);
		break;
	case Resource::Type::texture:
		//Clean resource if you are reimporting this
		//Call texture importer with this ppinter as reference
		//this->DataMaterial.id_texture = App->importer->MaterialImporter->LoadImageFromFile(this->DataMaterial, &this->exported_file);
		break;
	case Resource::Type::null:
		break;
	}
	return ret;
}