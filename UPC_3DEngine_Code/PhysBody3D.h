#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"
#include "p2Point.h"
#include "glmath.h"

class btRigidBody;
class Module;
class btTransform;

// =================================================
struct PhysBody3D
{

	friend class ModulePhysics3D;

public:

	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	const vec3 GetPos() const;
	void SetAsSensor(bool is_sensor);
	bool IsSensor() const;
	void SetAngularVelocity(float x, float y, float z);
	void SetLinearVelocity(float x, float y, float z);
	void Freeze(bool freezed);

private:

	btRigidBody* body = nullptr;
	vec3 vehicle_position;
	bool is_sensor = false;

public:

	p2List<Module*> collision_listeners;

};

#endif // __PhysBody3D_H__