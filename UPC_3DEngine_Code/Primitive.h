
#pragma once
#include "glmath.h"
#include "Color.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
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
};

// ============================================
class P2Cube : public Primitive
{
public :
	P2Cube();
	P2Cube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	vec3 size;
};

// ============================================
class P2Sphere : public Primitive
{
public:
	P2Sphere();
	P2Sphere(float radius);
	void InnerRender() const;
public:
	float radius;
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