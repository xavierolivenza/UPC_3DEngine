#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleSceneIntro::~ModuleSceneIntro()
{
	
}

bool ModuleSceneIntro::Init()
{
	return true;
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOGP("Loading Intro assets");
	bool ret = true;

	/*
	vec shere1_centre = { 0,0,0 };
	Sphere* shere1 = new Sphere(shere1_centre, 2.0f);
	vec shere2_centre = { 1,0,0 };
	Sphere* shere2 = new Sphere(shere2_centre, 2.0f);
	bool intersection = shere1->Intersects(*shere2);
	if(intersection)
		LOGP("Spheres intersect");
	else
		LOGP("Spheres don't intersect");
	*/

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	//--------------------------------------------//
	//Start
	cube1.size.x = 15;
	cube1.size.y = 1;
	cube1.size.z = 15;
	cube1.color = Chocolate;
	cube1.SetPos(0, 0, 0);
	cube1body = App->physics->AddBody(cube1, 0);
	//Arc
	cube1_1.size.x = 1;
	cube1_1.size.y = 10;
	cube1_1.size.z = 1;
	cube1_1.color = Red;
	cube1_1.SetPos(7, 5.5, 5.5);
	cube1_1body = App->physics->AddBody(cube1_1, 0);
	cube1_2.size.x = 13;
	cube1_2.size.y = 1;
	cube1_2.size.z = 1;
	cube1_2.color = Red;
	cube1_2.SetPos(0, 10, 5.5);
	cube1_2body = App->physics->AddBody(cube1_2, 0);
	cube1_3.size.x = 1;
	cube1_3.size.y = 10;
	cube1_3.size.z = 1;
	cube1_3.color = Red;
	cube1_3.SetPos(-7, 5.5, 5.5);
	cube1_3body = App->physics->AddBody(cube1_3, 0);
	//--------------------------------------------//
	return ret;
}

update_status ModuleSceneIntro::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	/*
	LCG rand_test;
	LOGP("Random generator: %f", rand_test.Float(0.0f, 1.0f));
	LOGP("Random generator: %i", rand_test.Int(0, 100));
	*/

	//--------------------------------------------//
	//Start
	cube1.Render();
	//Arc
	cube1_1.Render();
	cube1_2.Render();
	cube1_3.Render();
	//--------------------------------------------//
	
	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOGP("Unloading Intro scene");
	return true;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	
}