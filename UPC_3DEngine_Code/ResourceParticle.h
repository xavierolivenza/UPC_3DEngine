#ifndef __ResourceParticle_H__
#define __ResourceParticle_H__

#include "Resource.h"

class ResourceParticle : public Resource
{
public:
	ResourceParticle();
	~ResourceParticle();

	void CleanResource();

	void DrawResourceOptionsAndData();

	void Save(JSON_Object* conf) const;
	void Load(JSON_Object* conf);
};

#endif // __ResourceParticle_H__