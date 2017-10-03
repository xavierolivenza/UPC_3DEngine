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
	Primitive_Cylinder
};

struct Geometry
{
	uint id_vertices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;
	uint id_indices = 0; // id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;
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
	Geometry GeometryStruct;
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
	float radius;
private:
	math::Sphere geo_sphere;

	float3*	vertex3 = nullptr;
	float2* vertex3_uv = nullptr;

	int stacks = 0;
	int slices = 0;

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