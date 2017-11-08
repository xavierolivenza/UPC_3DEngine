#include "Resource.h"

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

void Resource::Save(JSON_Object* conf) const
{

}

void Resource::Load(JSON_Object* conf)
{

}