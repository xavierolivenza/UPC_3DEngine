#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "camera3d";
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{

}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOGP("Setting up the camera");
	bool ret = true;
	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOGP("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	//TODO
	//When load geometry, auto resize cam(all geometry inside fov)

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		Cam_move = !Cam_move;
		Cam_move_LOG = true;
	}
	if (Cam_move)
	{
		if (Cam_move_LOG)
		{
			LOGP("Camera movement: Active");
			Cam_move_LOG = false;
		}
	}
	else
	{
		if (Cam_move_LOG)
		{
			LOGP("Camera movement: Disabled");
			Cam_move_LOG = false;
		}
		return UPDATE_CONTINUE;
	}
		
	vec3 newPos(0,0,0);
	float speed = 50.0f * dt;
	
	//Speed
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 100.0f * dt;

	//FPS like movement
	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;
	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	//Elevator/lift like movement
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y -= speed;

	bool zoomed = false;
	if (App->input->GetMouseZ() != 0)
	{
		newPos += App->input->GetMouseZ() * Z * speed;
		zoomed = true;
	}

	//Orbit the geometry in scene
	Position += newPos;
	if(!zoomed)
		Reference += newPos;

	if((App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) && (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT))
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if(Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		RecentreCameraToGeometry();

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

void ModuleCamera3D::CenterCameraToGeometry(const AABB* meshAABB)
{
	if (meshAABB == nullptr)
		Reference = vec3(0.0f, 0.0f, 0.0f);
	else
	{
		vec centre = meshAABB->CenterPoint();
		Reference = vec3(centre.x, centre.y, centre.z);
		LastCentreGeometry = meshAABB;
		
		//Same as LookAt, but here we don't recalculate viewmatrix, this is not necessary now
		Z = normalize(Position - Reference);
		//For the math here, X,Y is not necesary
		//X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
		//Y = cross(Z, X);

		//Set camerapos to see the hole geometry in view
		/*
		vec difference = meshAABB->maxPoint - meshAABB->minPoint;
		float wide = MAX(MAX(difference.x, difference.y), difference.z); //Get max of the 3 sides of the AABB
		*/

		vec difference = meshAABB->maxPoint - meshAABB->minPoint;
		float wide = difference.Length() + 2.0f; //This magic number is just to have some frame around geometry
		float FOVdistance = (wide * 0.5f) / tan(60.0f * 0.5f * DEGTORAD); //FOV = 60.0f, by now this remains as magic number, will change when we update camera

		/*
		vec3 distance_CamToCentreAabb_vec3 = Position - Reference;
		vec distance_CamToCentreAabb_vec(distance_CamToCentreAabb_vec3.x, distance_CamToCentreAabb_vec3.y, distance_CamToCentreAabb_vec3.z);
		float distance_CamToCentreAabb_magnitude = distance_CamToCentreAabb_vec.Length();
		
		float distance_diference = distance_CamToCentreAabb_magnitude - FOVdistance;
		//float distance_diference = FOVdistance - distance_CamToCentreAabb_magnitude;
		*/

		Position = Z * FOVdistance;

		// Recalculate matrix (CalculateViewMatrix is called inside LookAt)-------------
		LookAt(Reference);
		/**/
	}
}

void ModuleCamera3D::RecentreCameraToGeometry()
{
	CenterCameraToGeometry(LastCentreGeometry);
}