#pragma once

#include "MathGeoLib\Math.h"

struct MeshData;
class ComponentTransform;

class ImporterMesh
{
public:
	ImporterMesh();
	~ImporterMesh();

	bool Init();
	bool CleanUp();

	bool Save(const float3& pos, const float3& scale, const Quat& rot, const MeshData& DataMesh, std::string& loaded_file) const;
	bool Load(ComponentTransform& transform, MeshData& DataMesh, const std::string* file_to_load, bool settransform = true);

	bool AssimpCanLoad(const char * file);

};