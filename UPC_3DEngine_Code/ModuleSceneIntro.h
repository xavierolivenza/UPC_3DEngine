#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"


#include "MathGeoLib\MathGeoLib.h"
#include "MathGeoLib\MathBuildConfig.h"

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
	P2Cube		 cube1;
	PhysBody3D*  cube1body;
	//--------------------------------------------//
	//Arc
	P2Cube		 cube1_1;
	PhysBody3D*  cube1_1body;
	P2Cube		 cube1_2;
	PhysBody3D*  cube1_2body;
	P2Cube		 cube1_3;
	PhysBody3D*  cube1_3body;
	//--------------------------------------------//
	
};
