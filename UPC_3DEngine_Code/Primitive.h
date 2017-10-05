#pragma once

#include <list>
#include <vector>

#include "Globals.h"
#include "glmath.h"
#include "Color.h"
#include "MathGeoLib\MathGeoLib.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder,
	Primitive_Ray
};

struct GeometryData
{
	AABB BoundBox;

	uint id_vertices = 0; // id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;
	uint id_indices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;
	uint id_colors = 0; // id in VRAM
	float* colors = nullptr;
	uint id_normals = 0; // id in VRAM
	float* normals = nullptr;
	uint id_texture_coords = 0; // id in VRAM
	float* texture_coords = nullptr;
	uint id_texture = 0; // id in VRAM
	std::string texture_name = "";
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	virtual void	GeneratePrimitiveWithNewData();
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	mat4x4 transform;
	bool axis;

protected:
	PrimitiveTypes type;
	GeometryData GeometryStruct;
};

// ============================================
class P2Cube : public Primitive
{
public :
	P2Cube();
	P2Cube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
	void GeneratePrimitiveWithNewData();
public:
	vec3 size;
private:
	bool buffersCreated = false;
	bool newVertexBufferCreated = false;
};

// ============================================
class P2Sphere : public Primitive
{
public:
	P2Sphere();
	P2Sphere(float radius);
	~P2Sphere();
	void InnerRender() const;
	void GeneratePrimitiveWithNewData();
public:
	float radius = 1.0f;
	uint stacks = 10;
	uint slices = 10;

private:
	math::Sphere geo_sphere;

	float3*	vertex3 = nullptr;
	float2* vertex3_uv = nullptr;

	std::list<vec3> mesh;
	std::vector<float> vertex_array;
};

// ============================================
class P2Cylinder : public Primitive
{
public:
	P2Cylinder();
	P2Cylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class P2Line : public Primitive
{
public:
	P2Line();
	P2Line(float x, float y, float z);
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class P2Ray : public Primitive
{
public:
	P2Ray();
	void InnerRender() const;
	const Ray* GetRay() const;
	void SetOrigin(float x, float y, float z);
	void SetDirection(float x, float y, float z);
private:
	Ray GeolibRay;
	uint length = 1000;
};

// ============================================
class P2Plane : public Primitive
{
public:
	P2Plane();
	P2Plane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	vec3 normal;
	float constant;
};