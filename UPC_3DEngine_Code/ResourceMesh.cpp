#include "ResourceMesh.h"
#include "Application.h"
#include "ModuleParson.h"

ResourceMesh::ResourceMesh() : Resource(Resource::Type::mesh)
{

}

ResourceMesh::~ResourceMesh()
{

}

void ResourceMesh::Save(JSON_Object* conf) const
{
	/*
	App->parsonjson-


		uint uuid = 0;
	std::string file;
	std::string exported_file;
	Type type = Type::null;
	*/
}

void ResourceMesh::Load(JSON_Object* conf)
{

}