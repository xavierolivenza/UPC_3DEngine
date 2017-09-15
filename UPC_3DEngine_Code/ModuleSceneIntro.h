#pragma once
#include "Module.h"
#include "p2DynArray.h"
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
	//--------------------------------------------//
	//---------------Circuit parts----------------//
	//--------------------------------------------//
	//Start
	Cube		 cube1;
	PhysBody3D*  cube1body;
	//--------------------------------------------//
	//Arc
	Cube		 cube1_1;
	PhysBody3D*  cube1_1body;
	Cube		 cube1_2;
	PhysBody3D*  cube1_2body;
	Cube		 cube1_3;
	PhysBody3D*  cube1_3body;
	//--------------------------------------------//
	
};
