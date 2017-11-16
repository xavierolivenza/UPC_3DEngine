#pragma once

#include "Globals.h"
#include "ModuleParson.h"

class Resource
{
	friend class ModuleResources;
	friend class ModuleSceneImporter;
public:
	enum Type
	{
		mesh,
		texture,
		null
	};

public:
	Resource(Type type);
	virtual ~Resource();

	uint GetUID() const;
	Type GetType() const;
	const char* GetTypeStr() const;
	const std::string& GetFile() const;
	
	virtual void CleanResource();

	bool LoadResource();
	bool ReLoadResource();

	bool UnLoad();

	virtual void DrawResourceOptionsAndData();

	virtual void Save(JSON_Object* conf) const;
	virtual void Load(JSON_Object* conf);

protected:
	virtual bool LoadResourceToMemory();

protected:
	uint uuid = 0;
	std::string file;
	std::string exported_file;
	std::string file_date;
	Type type = Type::null;
	uint loaded = 0;
};