#include "Glew\include\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>

#include "Primitive.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "p2Log.h"

//#include "glut/glut.h"
//#pragma comment (lib, "glut/glut32.lib")

// ------------------------------------------------------------
Primitive::Primitive() : transform(IdentityMatrix), color(White), axis(false), type(PrimitiveTypes::Primitive_Point)
{}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitive::Render() const
{
	glPushMatrix();
	glMultMatrixf(transform.M);

	if (axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}

	glColor3f(color.r, color.g, color.b);
	
	
	if (App->renderer3D->GL_Wireframe_Active())
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (App->renderer3D->GL_Point_Active())
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	InnerRender();

	if (App->renderer3D->GL_Wireframe_Active() | App->renderer3D->GL_Point_Active())
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glColor3f(1.0f, 1.0f, 1.0f);

	glPopMatrix();
}

// ------------------------------------------------------------
void Primitive::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

void Primitive::GeneratePrimitiveWithNewData()
{

}

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const vec3 &u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void Primitive::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

// CUBE ============================================
P2Cube::P2Cube() : Primitive(), size(1.0f, 1.0f, 1.0f)
{
	type = PrimitiveTypes::Primitive_Cube;
}

P2Cube::P2Cube(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
{
	type = PrimitiveTypes::Primitive_Cube;
}

P2Cube::~P2Cube()
{
	glDeleteBuffers(1, &GeometryStruct.id_vertices);
	RELEASE_ARRAY(GeometryStruct.vertices);

	glDeleteBuffers(1, &GeometryStruct.id_indices);
	RELEASE_ARRAY(GeometryStruct.indices);

	glDeleteBuffers(1, &GeometryStruct.id_texture_coords);
	RELEASE_ARRAY(GeometryStruct.texture_coords);
}

void P2Cube::InnerRender() const
{	
	if (buffersCreated)
		App->renderer3D->Draw(&GeometryStruct);
	else
	{
		//I do this here because if i put this in the GeneratePrimitiveWithNewData i get an error, i don't know why, so by now this stays here.

		// Buffer for vertices
		glGenBuffers(1, (GLuint*) &(GeometryStruct.id_vertices));
		glBindBuffer(GL_ARRAY_BUFFER, GeometryStruct.id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * GeometryStruct.num_vertices * 3, GeometryStruct.vertices, GL_STATIC_DRAW);

		// Buffer for indices
		glGenBuffers(1, (GLuint*) &(GeometryStruct.id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GeometryStruct.id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * GeometryStruct.num_indices, GeometryStruct.indices, GL_STATIC_DRAW);

		// Buffer for texture coords
		glGenBuffers(1, (GLuint*) &(GeometryStruct.id_texture_coords));
		glBindBuffer(GL_ARRAY_BUFFER, GeometryStruct.id_texture_coords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * GeometryStruct.num_indices * 2, GeometryStruct.texture_coords, GL_STATIC_DRAW);

		//I don't like it and this can be dangerous, but i want to preserve InnerRender const, so by now i take the risk.
		const_cast<bool&>(buffersCreated) = true;
	}
}

void P2Cube::GeneratePrimitiveWithNewData()
{
	if(buffersCreated)
	{
		glDeleteBuffers(1, &GeometryStruct.id_vertices);
		RELEASE_ARRAY(GeometryStruct.vertices);

		glDeleteBuffers(1, &GeometryStruct.id_indices);
		RELEASE_ARRAY(GeometryStruct.indices);

		glDeleteBuffers(1, &GeometryStruct.id_texture_coords);
		RELEASE_ARRAY(GeometryStruct.texture_coords);
	}

	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;

	GeometryStruct.num_vertices = 16;
	GeometryStruct.vertices = new float[GeometryStruct.num_vertices * 3];
	float vertices[] =
	{
		sx,  sy,  sz,
		-sx,  sy,  sz,
		-sx,  sy, -sz,
		sx,  sy, -sz,

		sx, -sy,  sz,
		-sx, -sy,  sz,
		-sx, -sy, -sz,
		sx, -sy, -sz,

		sx,  sy,  sz,
		sx, -sy,  sz,
		sx, -sy, -sz,
		sx,  sy, -sz,

		-sx,  sy,  sz,
		-sx, -sy,  sz,
		-sx, -sy, -sz,
		-sx,  sy, -sz
	};
	memcpy(GeometryStruct.vertices, vertices, sizeof(float) * GeometryStruct.num_vertices * 3);

	GeometryStruct.num_indices = 36;
	GeometryStruct.indices = new uint[GeometryStruct.num_indices];
	uint indices[] =
	{
		2, 7, 6,
		2, 3, 7,
		11, 9, 10,
		11, 8, 9,
		1, 4, 0,
		1, 5, 4,
		15, 13, 12,
		15, 14, 13,
		1, 3, 2,
		1, 0, 3,
		7, 5, 6,
		7, 4, 5
	};
	memcpy(GeometryStruct.indices, indices, sizeof(uint) * GeometryStruct.num_indices);

	//Texture
	GeometryStruct.texture_coords = new float[GeometryStruct.num_indices * 3];
	float texture_coords[] =
	{
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	memcpy(GeometryStruct.texture_coords, texture_coords, sizeof(float) * GeometryStruct.num_indices * 3);

	GeometryStruct.BoundBox = AABB(vec(-sx, -sy, -sz), vec(sx, sy, sz));
}

// SPHERE ============================================
P2Sphere::P2Sphere() : Primitive(), radius(1.0f)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

P2Sphere::P2Sphere(float radius) : Primitive(), radius(radius)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

P2Sphere::~P2Sphere()
{

}

void P2Sphere::InnerRender() const
{
	GLuint sphere_id = 0;
	GLfloat magic_vertices[344 * 3];

	glGenBuffers(1, (GLuint*)&sphere_id);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_id);
	glBufferData(GL_ARRAY_BUFFER, vertex_array.size() * sizeof(float), &vertex_array[0], GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, vertex_array.size() / 3);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void P2Sphere::GeneratePrimitiveWithNewData()
{
	for (int t = 0; t < stacks; t++)
	{
		float theta1 = ((float)t / stacks) * 1 * pi;
		float theta2 = ((float)(t + 1) / stacks) * 1 * pi;

		for (int p = 0; p < slices; p++)
		{
			float phi1 = ((float)p / slices) * 2 * pi;
			float phi2 = ((float)(p + 1) / slices) * 2 * pi;

			vec3 vertex1;
			vec3 vertex2;
			vec3 vertex3;
			vec3 vertex4;

			vertex1.z = radius * Sin(theta1) * Cos(phi1);
			vertex1.x = radius * Sin(theta1) * Sin(phi1);
			vertex1.y = radius * Cos(theta1);

			vertex2.z = radius * Sin(theta1) * Cos(phi2);
			vertex2.x = radius * Sin(theta1) * Sin(phi2);
			vertex2.y = radius * Cos(theta1);

			vertex3.z = radius * Sin(theta2) * Cos(phi2);
			vertex3.x = radius * Sin(theta2) * Sin(phi2);
			vertex3.y = radius * Cos(theta2);

			vertex4.z = radius * Sin(theta2) * Cos(phi1);
			vertex4.x = radius * Sin(theta2) * Sin(phi1);
			vertex4.y = radius * Cos(theta2);

			if (t == 0)
			{
				vertex_array.push_back(vertex1.x);
				vertex_array.push_back(vertex1.y);
				vertex_array.push_back(vertex1.z);

				vertex_array.push_back(vertex4.x);
				vertex_array.push_back(vertex4.y);
				vertex_array.push_back(vertex4.z);

				vertex_array.push_back(vertex3.x);
				vertex_array.push_back(vertex3.y);
				vertex_array.push_back(vertex3.z);
			}
			else if (t + 1 == stacks)
			{
				vertex_array.push_back(vertex1.x);
				vertex_array.push_back(vertex1.y);
				vertex_array.push_back(vertex1.z);

				vertex_array.push_back(vertex3.x);
				vertex_array.push_back(vertex3.y);
				vertex_array.push_back(vertex3.z);

				vertex_array.push_back(vertex2.x);
				vertex_array.push_back(vertex2.y);
				vertex_array.push_back(vertex2.z);
			}
			else
			{
				vertex_array.push_back(vertex1.x);
				vertex_array.push_back(vertex1.y);
				vertex_array.push_back(vertex1.z);

				vertex_array.push_back(vertex4.x);
				vertex_array.push_back(vertex4.y);
				vertex_array.push_back(vertex4.z);

				vertex_array.push_back(vertex2.x);
				vertex_array.push_back(vertex2.y);
				vertex_array.push_back(vertex2.z);

				vertex_array.push_back(vertex2.x);
				vertex_array.push_back(vertex2.y);
				vertex_array.push_back(vertex2.z);

				vertex_array.push_back(vertex4.x);
				vertex_array.push_back(vertex4.y);
				vertex_array.push_back(vertex4.z);

				vertex_array.push_back(vertex3.x);
				vertex_array.push_back(vertex3.y);
				vertex_array.push_back(vertex3.z);
			}
		}
	}

	GeometryStruct.BoundBox = AABB(vec(-radius, -radius, -radius), vec(radius, radius, radius));

}

// CYLINDER ============================================
P2Cylinder::P2Cylinder() : Primitive(), radius(1.0f), height(1.0f)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

P2Cylinder::P2Cylinder(float radius, float height) : Primitive(), radius(radius), height(height)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

void P2Cylinder::InnerRender() const
{
	int n = 30;

	// Cylinder Bottom
	glBegin(GL_POLYGON);
	
	for(int i = 360; i >= 0; i -= (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for(int i = 0; i <= 360; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < 480; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians

		glVertex3f(height*0.5f,  radius * cos(a), radius * sin(a) );
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a) );
	}
	glEnd();
}

// LINE ==================================================
P2Line::P2Line() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Line;
}

P2Line::P2Line(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitiveTypes::Primitive_Line;
}

void P2Line::InnerRender() const
{
	glLineWidth(thickness);
	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();
	glLineWidth(1.0f);
}

// RAY ==================================================
P2Ray::P2Ray() : Primitive()
{
	type = PrimitiveTypes::Primitive_Ray;
	SetOrigin(0.0f, 0.0f, 0.0f);
	SetDirection(0.0f, 1.0f, 0.0f);
}

const Ray* P2Ray::GetRay() const
{
	return &GeolibRay;
}

void P2Ray::SetOrigin(float x, float y, float z)
{
	GeolibRay.pos = vec(x, y, z);
}

void P2Ray::SetDirection(float x, float y, float z)
{
	vec direction;
	direction.x = x;
	direction.y = y;
	direction.z = z;
	direction.Normalize();
	GeolibRay.dir = direction;
}

void P2Ray::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(GeolibRay.pos.x, GeolibRay.pos.y, GeolibRay.pos.z);
	vec dir;
	dir.x = GeolibRay.pos.x + GeolibRay.dir.x;
	dir.y = GeolibRay.pos.y + GeolibRay.dir.y;
	dir.z = GeolibRay.pos.z + GeolibRay.dir.z;
	dir.Normalize();
	glVertex3f(dir.x * length, dir.y * length, dir.z * length);

	glEnd();

	glLineWidth(1.0f);
}

// PLANE ==================================================
P2Plane::P2Plane() : Primitive(), normal(0, 1, 0), constant(1)
{
	type = PrimitiveTypes::Primitive_Plane;
}

P2Plane::P2Plane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	type = PrimitiveTypes::Primitive_Plane;
}

void P2Plane::InnerRender() const
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float d = 200.0f;

	for(float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}