#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

#define WIDTH 15
#define CHICKEN_Y_FLOOR 1.5f

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	//--------------------------------------------//
	//Start
	cube1.size.x = WIDTH;
	cube1.size.y = 1;
	cube1.size.z = 110;
	cube1.color = Chocolate;
	cube1.SetPos(0, 0, 50);
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

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
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

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	
}