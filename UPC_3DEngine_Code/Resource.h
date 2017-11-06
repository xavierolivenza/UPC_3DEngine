#pragma once

#include "Globals.h"
#include "ModuleParson.h"

class Resource
{
public:
	enum Type
	{
		mesh,
		texture,
		null
	};

public:
	Resource(Type type);
	~Resource();

	uint GetUID() const;
	Type GetType() const;
	
	virtual void Save(JSON_Object* conf) const;
	virtual void Load(JSON_Object* conf);

protected:
	uint uuid = 0;
	Type type = Type::null;
	uint loaded = 0;
};