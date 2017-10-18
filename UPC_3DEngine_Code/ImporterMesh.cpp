#include "ImporterMesh.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "Assimp/include/Logger.hpp"
#include "Assimp/include/LogStream.hpp"
#include "Assimp/include/DefaultLogger.hpp"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ImporterMesh::ImporterMesh()
{
	
}

ImporterMesh::~ImporterMesh()
{
	
}

bool ImporterMesh::Init()
{
	// Create a logger instance 
	Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
	// Now I am ready for logging my stuff
	Assimp::DefaultLogger::get()->info("this is my info-call");
	// Select the kinds of messages you want to receive on this log stream
	const unsigned int severity = Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Err | Assimp::Logger::Warn;
	// Attaching it to the default logger
	Assimp::DefaultLogger::get()->attachStream(new Assimp::myStream(), severity);
	return true;
}

bool ImporterMesh::CleanUp()
{
	// Kill it after the work is done
	Assimp::DefaultLogger::kill();
	return true;
}

bool ImporterMesh::Import(std::string* file_to_import, std::string& output_file)
{
	if((file_to_import == nullptr) || file_to_import->empty())
		return false;

	//TODO Generate ComponentMesh here, send to Save

	return false;
}

bool ImporterMesh::Save(const Component* component, std::string* file_to_save)
{
	if ((component == nullptr) || (file_to_save == nullptr) || file_to_save->empty())
		return false;

	ComponentMesh* meshComponent = (ComponentMesh*)component;

	//Serialize ComponentMesh to file

	return false;
}

Component* ImporterMesh::Load(std::string* file_to_load)
{
	return nullptr;
}