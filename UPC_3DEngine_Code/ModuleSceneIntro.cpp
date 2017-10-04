#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "sceneintro";
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

	LOGP("/-----------------------------------------/");

	/**/
	vec shere1_centre = { 0,0,0 };
	shere1 = new Sphere(shere1_centre, 2.0f);
	LOGP("Sphere 1 centre (0,0,0), radius 2.0f");
	vec shere2_centre = { 1,0,0 };
	shere2 = new Sphere(shere2_centre, 2.0f);
	LOGP("Sphere 2 centre (1,0,0), radius 2.0f");
	bool intersection1 = shere1->Intersects(*shere2);
	if(intersection1)
		LOGP("Spheres 1-2 intersect");
	else
		LOGP("Spheres 1-2 don't intersect");
	/**/

	/**/
	vec shere3_centre = { 0,0,0 };
	shere3 = new Sphere(shere3_centre, 2.0f);
	LOGP("Sphere 3 centre (0,0,0), radius 2.0f");
	vec shere4_centre = { 5,0,0 };
	shere4 = new Sphere(shere4_centre, 2.0f);
	LOGP("Sphere 4 centre (5,0,0), radius 2.0f");
	bool intersection2 = shere3->Intersects(*shere4);
	if (intersection2)
		LOGP("Spheres 3-4 intersect");
	else
		LOGP("Spheres 3-4 don't intersect");
	/**/

	LOGP("/-----------------------------------------/");

	/**/
	vec capsule1_bot = { 0,0,0 };
	vec capsule1_top = { 0,1,0 };
	capsule1 = new Capsule(capsule1_bot, capsule1_top,2.0f);
	LOGP("Capsule 1 bottom (0,0,0), top (0,1,0), radius 2.0f");
	vec capsule2_bot = { 0,1,0 };
	vec capsule2_top = { 0,2,0 };
	capsule2 = new Capsule(capsule2_bot, capsule2_top, 2.0f);
	LOGP("Capsule 2 bottom (0,1,0), top (0,2,0), radius 2.0f");
	bool intersection3 = capsule1->Intersects(*capsule2);
	if (intersection3)
		LOGP("Capsule 1-2 intersect");
	else
		LOGP("Capsule 1-2 don't intersect");
	/**/

	/**/
	vec capsule3_bot = { 0,0,0 };
	vec capsule3_top = { 0,1,0 };
	capsule3 = new Capsule(capsule3_bot, capsule3_top, 2.0f);
	LOGP("Capsule 3 bottom (0,0,0), top (0,1,0), radius 2.0f");
	vec capsule4_bot = { 0,10,0 };
	vec capsule4_top = { 0,11,0 };
	capsule4 = new Capsule(capsule4_bot, capsule4_top, 2.0f);
	LOGP("Capsule 4 bottom (0,10,0), top (0,11,0), radius 2.0f");
	bool intersection4 = capsule3->Intersects(*capsule4);
	if (intersection4)
		LOGP("Capsule 3-4 intersect");
	else
		LOGP("Capsule 3-4 don't intersect");
	/**/

	LOGP("/-----------------------------------------/");
	/**/
	for (int i = 0; i < 10; i++)
	{
		LCG rand_test;
		LOGP("Random generator Flo: %f", rand_test.Float(0.0f, 1.0f));
		LOGP("Random generator Int: %i", rand_test.Int(0, 100));
	}
	/**/

	LOGP("/-----------------------------------------/");
	
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	
	/*
	cube1.size.x = 1;
	cube1.size.y = 1;
	cube1.size.z = 1;
	cube1.color = Chocolate;
	cube1.SetPos(3, 0.5f, 0);
	cube1.GeneratePrimitiveWithNewData();

	sphere1.radius = 1.0f;
	sphere1.slices = 15;
	sphere1.stacks = 15;
	sphere1.SetPos(-3, 1, 0);
	sphere1.GeneratePrimitiveWithNewData();
	*/
	return ret;
}

update_status ModuleSceneIntro::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	//cube1.Render();
	//sphere1.Render();
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
	RELEASE(shere1);
	RELEASE(shere2);
	RELEASE(shere3);
	RELEASE(shere4);
	RELEASE(capsule1);
	RELEASE(capsule2);
	RELEASE(capsule3);
	RELEASE(capsule4);
	
	return true;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	
}