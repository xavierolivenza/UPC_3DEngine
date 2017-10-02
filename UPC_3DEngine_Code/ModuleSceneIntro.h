#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

private:
	Sphere* shere1;
	Sphere* shere2;
	Sphere* shere3;
	Sphere* shere4;
	Capsule* capsule1;
	Capsule* capsule2;
	Capsule* capsule3;
	Capsule* capsule4;

	P2Cube cube1;
	P2Sphere* sphere1 = nullptr;
};
