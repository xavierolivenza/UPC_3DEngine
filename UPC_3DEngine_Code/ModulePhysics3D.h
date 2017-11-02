#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#include "Bullet/include/btBulletDynamicsCommon.h"

// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

class DebugDrawer;
struct PhysBody3D;
struct VehicleInfo;

class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(Application* app, bool start_enabled = true);
	~ModulePhysics3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool SaveConf(JSON_Object* conf) const;
	bool LoadConf(JSON_Object* conf);

	void ImGuiModuleVariables();

	PhysBody3D* AddBody(const P2Sphere& sphere, float mass = 1.0f);
	PhysBody3D* AddBody(const P2Cube& cube, float mass = 1.0f);
	PhysBody3D* AddBody(const P2Cylinder& cylinder, float mass = 1.0f);

	btHingeConstraint* ModulePhysics3D::Add_Hinge_Constraint(btRigidBody& rbA, btRigidBody& rbB, const btVector3& pivotInA, const btVector3& pivotInB, btVector3& axisInA, btVector3& axisInB, bool disablecollision);
	void AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const float3& anchorA, const float3& anchorB);
	void AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const float3& anchorA, const float3& anchorB, const float3& axisS, const float3& axisB, bool disable_collision = false);

private:

	bool debug;

	btDefaultCollisionConfiguration*	collision_conf;
	btCollisionDispatcher*				dispatcher;
	btBroadphaseInterface*				broad_phase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld*			world;
	btDefaultVehicleRaycaster*			vehicle_raycaster;
	DebugDrawer*						debug_draw;

	std::list<btCollisionShape*> shapes;
	std::list<PhysBody3D*> bodies;
	std::list<btDefaultMotionState*> motions;
	std::list<btTypedConstraint*> constraints;
};

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer() : line(0,0,0)
	{}

	void drawGrid(int gridWH);
	void drawAxis(float axislen);

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	P2Line line;
	Primitive point;

	bool DrawGrid = true;
	bool DrawAxis = true;
	uint GridSize = 20;
	float AxisLength = 2.0f;
	float LineThickness = 2.0f;
};