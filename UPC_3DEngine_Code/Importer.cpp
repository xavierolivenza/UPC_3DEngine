#include "Importer.h"

Importer::Importer()
{

}

Importer::~Importer()
{

}

bool Importer::Init()
{
	return true;
}

bool Importer::CleanUp()
{
	return true;
}

bool Importer::Import(std::string* file_to_import, std::string& output_file)
{
	return false;
}

bool Importer::Save(const Component* component, std::string* file_to_load)
{
	return false;
}

Component* Importer::Load(std::string* file_to_load)
{
	return nullptr;
}