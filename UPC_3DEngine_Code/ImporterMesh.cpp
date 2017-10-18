#include "ImporterMesh.h"

#include "Assimp/include/Logger.hpp"
#include "Assimp/include/LogStream.hpp"
#include "Assimp/include/DefaultLogger.hpp"

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

bool ImporterMesh::Save(const MeshData* DataMesh, std::string* file_to_save)
{
	if ((DataMesh == nullptr) || (file_to_save == nullptr) || file_to_save->empty())
		return false;

	//Serialize MeshData to file

	return false;
}

bool ImporterMesh::Load(MeshData& DataMesh, std::string* file_to_load)
{
	return false;
}