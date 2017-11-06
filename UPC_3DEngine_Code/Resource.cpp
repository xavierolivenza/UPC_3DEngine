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

void Resource::Save(JSON_Object* conf) const
{

}

void Resource::Load(JSON_Object* conf)
{

}