#include <map>

#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "camera3d";

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(2.0f, 2.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);

}

ModuleCamera3D::~ModuleCamera3D()
{

}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOGP("Setting up the camera");
	CameraComp = new ComponentCamera(nullptr, true);
	CameraComp->SetFrame(Position, -Z, Y);
	bool ret = true;
	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOGP("Cleaning camera");
	RELEASE(CameraComp);
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

	float3 newPos(0, 0, 0);
	float speed = 50.0f * dt;

	//Speed
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 100.0f * dt;

	//FPS like movement
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

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
	if (!zoomed)
		Reference += newPos;

	if ((App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) && (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT))
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;
			float3x3 rotate = float3x3::RotateAxisAngle(float3(0.0f, 1.0f, 0.0f), DeltaX * DEGTORAD);

			X = rotate * X;
			Y = rotate * Y;
			Z = rotate * Z;
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;
			float3x3 rotate = float3x3::RotateAxisAngle(X, DeltaY * DEGTORAD);

			Y = rotate * Y;
			Z = rotate * Z;

			if (Y.y < 0.0f)
			{
				Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = Z.Cross(X);
			}
		}

		Position = Reference + Z * Position.Length();
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		const GameObject* selected = App->engineUI->GetSelectedGameObject();
		if (selected != nullptr)
		{
			ComponentMesh* Mesh = (ComponentMesh*)selected->FindComponentFirst(ComponentType::Mesh_Component);
			if (Mesh != nullptr)
			{
				AABB Box;
				Mesh->GetTransformedAABB(Box);
				CenterCameraToGeometry(&Box);
			}
		}
	}

	CameraComp->SetFrame(Position, -Z, Y);

	//Mouse Picking
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		//Create Ray
		int w = 0;
		int h = 0;
		App->window->GetWindowSize(w, h);
		float MouseNormX = -(1.0f - (float(App->input->GetMouseX()) * 2.0f) / (float)w);
		float MouseNormY = 1.0f - (float(App->input->GetMouseY()) * 2.0f) / (float)h;
		MousePickRay = CameraComp->frustum.UnProjectLineSegment(MouseNormX, MouseNormY);

		//Check ray agains gameobjects Sphere - AABB (Optimaze with infrustrum gameobjects(quadtree/octree))
		std::vector<GameObject*> SceneGameObjects;
		std::map<float, const GameObject*> SceneGameObjectsHitted;
		App->scene->GetAllSceneGameObjects(SceneGameObjects);
		for (std::vector<GameObject*>::const_iterator item = SceneGameObjects.cbegin(); item != SceneGameObjects.cend(); ++item)
		{
			ComponentMesh* MeshComp = (ComponentMesh*)(*item)->FindComponentFirst(ComponentType::Mesh_Component);
			if (MeshComp != nullptr)
			{
				bool hit = false;
				float Neardistance = 0.0f;
				float Fardistance = 0.0f;

				AABB GameObjectAABB;
				MeshComp->GetTransformedAABB(GameObjectAABB);
				hit = MousePickRay.Intersects(GameObjectAABB, Neardistance, Fardistance);
				/*
				hit = MousePickRay.Intersects(MeshComp->MeshDataStruct.BoundSphere);
				if (hit)
					hit = MousePickRay.Intersects(MeshComp->MeshDataStruct.BoundBox);
				if (hit)
					hit = MousePickRay.Intersects(MeshComp->MeshDataStruct.BoundOBox);
				*/
				if (hit)
				{
					//LOGP("Hit");
					//Order hitted AABB by distance
					SceneGameObjectsHitted.insert(std::pair<float, const GameObject*>(Neardistance, *item));
				}
			}
		}
		/*
		if (SceneGameObjectsHitted.size() > 0)
			App->engineUI->SetSelectedInspectorGO((GameObject*)(*SceneGameObjectsHitted.begin()).second);
		*/
		/**/
		GameObject* BestCandidate = nullptr;
		float BestCandidateDistance = 500.0f; //Initialize with big number so any triangle hit distance is minor to this.
		for (std::map<float, const GameObject*>::const_iterator item = SceneGameObjectsHitted.cbegin(); item != SceneGameObjectsHitted.cend(); ++item)
		{
			//Transform Mouse to GameObject Local Space
			//Copy it so we don't affect the original one
			RayLocal = MousePickRay;
			float4x4 TransMatrix = ((*item).second)->GetTransform()->GetMatrix();
			float4x4 InvertedTransMatrix = TransMatrix.Inverted().Transposed();
			RayLocal.Transform(TransMatrix.Inverted().Transposed());

			//Iterate mesh triangles to chechk if hit is real (using order by distance)
			MeshData& Mesh = ((ComponentMesh*)(((*item).second)->FindComponentFirst(ComponentType::Mesh_Component)))->MeshDataStruct;

			//Each 3 indices we have a triangle
			for (uint i = 0; i < Mesh.num_indices; i += 3)
			{
				Triangle tri;
				tri.a.Set(&Mesh.vertices[Mesh.indices[i++] * 3]);
				tri.b.Set(&Mesh.vertices[Mesh.indices[i++] * 3]);
				tri.c.Set(&Mesh.vertices[Mesh.indices[i++] * 3]);
				float distance = 0.0f;
				float3 intersectionPoint = float3::zero;
				if (RayLocal.Intersects(tri, &distance, &intersectionPoint))
				{
					if (distance < BestCandidateDistance)
					{
						BestCandidate = (GameObject*)(*item).second;
						BestCandidateDistance = distance;
					}
				}
			}
		}
		//Assign BestCandidate to selected gameobject
		if (BestCandidate != nullptr)
		{
			//App->engineUI->SetSelectedInspectorGO(const_cast<GameObject*>((*item).second)); //Warning const cast
			App->engineUI->SetSelectedInspectorGO(BestCandidate);
		}
		/**/
	}

	/**/
	if (RayDebugDraw)
	{
		glLineWidth(5.0f);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
		glVertex3f(RayLocal.a.x, RayLocal.a.y, RayLocal.a.z);
		glVertex3f(RayLocal.b.x, RayLocal.b.y, RayLocal.b.z);
		glEnd();
		glColor3f(1.0f, 1.0f, 1.0f);
		glLineWidth(1.0f);

		glLineWidth(5.0f);
		glBegin(GL_LINES);
		glVertex3f(MousePickRay.a.x, MousePickRay.a.y, MousePickRay.a.z);
		glVertex3f(MousePickRay.b.x, MousePickRay.b.y, MousePickRay.b.z);
		glEnd();
		glLineWidth(1.0f);
	}
	/**/

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z).Normalized());
	Y = Z.Cross(X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const float3 &Spot)
{
	Reference = Spot;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z).Normalized());
	Y = Z.Cross(X);
}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3 &Movement)
{
	Position += Movement;
	Reference += Movement;
}

// -----------------------------------------------------------------
const float* ModuleCamera3D::GetViewMatrix() const
{
	return CameraComp->GetViewMatrix();
}

const float* ModuleCamera3D::GetViewProjMatrix() const
{
	return CameraComp->GetViewProjMatrix();
}

void ModuleCamera3D::CenterCameraToGeometry(const AABB* meshAABB)
{
	if (meshAABB == nullptr)
		Reference = float3(0.0f, 0.0f, 0.0f);
	else
	{
		float3 centre = meshAABB->CenterPoint();
		Reference = float3(centre.x, centre.y, centre.z);

		//Same as LookAt, but here we don't recalculate viewmatrix, this is not necessary now
		Z = (Position - Reference).Normalized();
		//For the math here, X,Y is not necesary
		//X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
		//Y = cross(Z, X);

		//Set camerapos to see the hole geometry in view
		/*
		vec difference = meshAABB->maxPoint - meshAABB->minPoint;
		float wide = MAX(MAX(difference.x, difference.y), difference.z); //Get max of the 3 sides of the AABB
		*/

		float3 difference = meshAABB->maxPoint - meshAABB->minPoint;
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

void ModuleCamera3D::UpdateCamFov(int width, int height)
{
	//CameraComp->frustum.verticalFov = FOVVertical * DEGTORAD;
	CameraComp->frustum.horizontalFov = Atan(((float)width / (float)height) * Tan(CameraComp->frustum.verticalFov * 0.5f)) * 2.0f;
}

void ModuleCamera3D::ImGuiModuleVariables()
{
	ImGui::Checkbox("Ray Debug Draw Active", &RayDebugDraw);
	CameraComp->DrawComponentImGui();
}