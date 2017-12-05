#ifndef __ResourceEmitter_H__
#define __ResourceEmitter_H__

#include "Resource.h"

class ResourceEmitter : public Resource
{
public:
	ResourceEmitter();
	~ResourceEmitter();

	void CleanResource();

	void DrawResourceOptionsAndData();

	void Save(JSON_Object* conf) const;
	void Load(JSON_Object* conf);
};

#endif // __ResourceEmitter_H__