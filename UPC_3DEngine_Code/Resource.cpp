#include "Resource.h"
#include "Application.h"
#include "ImporterMaterial.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"

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
	return exported_file;
}

bool Resource::LoadResource()
{
	if (loaded > 0) loaded++;
	else loaded = LoadResourceToMemory();
	return loaded > 0;
}

bool Resource::ReLoadResource()
{
	if (loaded > 0)
	{
		//Clean loaded resource
		//TODO
		//Load resource
		LoadResourceToMemory();
	}
}

bool Resource::UnLoad()
{
	if (loaded > 0) loaded--;
	else return false;
	if (loaded == 0) App->resources->ResToUnload = true;
	return true;
}

void Resource::DrawResourceOptionsAndData()
{

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
		App->importer->LoadSimpleMesh(&this->exported_file, ((ResourceMesh*)this)->SimpleMeshDataStruct);
		ret = true;
		break;
	case Resource::Type::texture:
		//Clean resource if you are reimporting this
		//Call texture importer with this ppinter as reference
		((ResourceTexture*)this)->TextureDataStruct.id_texture = App->importer->MaterialImporter->LoadImageFromFile(((ResourceTexture*)this)->TextureDataStruct, &this->exported_file);
		ret = true;
		break;
	case Resource::Type::null:
		break;
	}
	return ret;
}