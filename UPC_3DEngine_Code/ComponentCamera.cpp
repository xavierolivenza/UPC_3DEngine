#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"

ComponentCamera::ComponentCamera(GameObject* parent, bool Active) : Component(parent, Active, 1, ComponentType::Camera_Component)
{
	if (Active) Enable();
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.nearPlaneDistance = NearPlaneDistance;
	frustum.farPlaneDistance = FarPlaneDistance;
	AspectRatio = App->window->GetAspectRatio();
	frustum.verticalFov = FOVVertical * DEGTORAD;
	frustum.horizontalFov = Atan(AspectRatio * Tan(frustum.verticalFov * 0.5f)) * 2.0f;
	frustum.pos = Pos;
	frustum.up = Up;
	frustum.front = Front;
}

ComponentCamera::~ComponentCamera()
{

}

bool ComponentCamera::Enable()
{
	return true;
}

bool ComponentCamera::PreUpdate(float dt)
{
	//Apply Frustum Culling
	if (FrustumCulling)
	{
		//Get vector of candidates from octree
		//Set DrawMesh to true
		std::list<ComponentMesh*> nodes;
		tests = App->scene->scene_octree.CollectIntersections(nodes, frustum);
		for (std::list<ComponentMesh*>::iterator item = nodes.begin(); item != nodes.cend(); ++item)
			(*item)->parent->DrawMesh = true;

		const std::vector<GameObject*>* SceneGameObjects;
		SceneGameObjects = App->scene->GetAllSceneGameObjects();
		for (std::vector<GameObject*>::const_iterator item = SceneGameObjects->begin(); item != SceneGameObjects->cend(); ++item)
		{
			if (!(*item)->IsStatic())
			{
				ComponentMesh* mesh = (ComponentMesh*)(*item)->FindComponentFirst(ComponentType::Mesh_Component);
				if (mesh != nullptr)
				{
					AABB Box;
					mesh->GetTransformedAABB(Box);

					if (frustum.Contains(Box))
						(*item)->DrawMesh = true;
				}
			}
		}
		//Test code, brute force
		/*
		const std::vector<GameObject*>* SceneGameObjects;
		SceneGameObjects = App->scene->GetAllSceneGameObjects();
		for (std::vector<GameObject*>::const_iterator item = SceneGameObjects->begin(); item != SceneGameObjects->cend(); ++item)
		{
		ComponentMesh* mesh = (ComponentMesh*)(*item)->FindComponentFirst(ComponentType::Mesh_Component);
		if (mesh != nullptr)
		{
		AABB Box;
		mesh->GetTransformedAABB(Box);

		if (frustum.Contains(Box))
		(*item)->DrawMesh = true;

		//if (frustum.ContainsAaBox(Box))
		//	(*item)->DrawMesh = true;
		}
		}
		*/
	}
	return true;
}

bool ComponentCamera::Update(float dt)
{
	if (parent != nullptr)
	{
		ComponentTransform* transform = parent->GetTransform();
		if (transform != nullptr)
		{
			float4x4 matrix = transform->GetMatrix();
			frustum.pos = matrix.Row3(3);
			frustum.front = matrix.Row3(2);
			frustum.up = matrix.Row3(1);
		}
	}

	if (DebugDrawFrustum)
		App->renderer3D->DrawDebugBox(frustum.CornerPoint(0), frustum.CornerPoint(1), frustum.CornerPoint(2), frustum.CornerPoint(3), frustum.CornerPoint(4), frustum.CornerPoint(5), frustum.CornerPoint(6), frustum.CornerPoint(7));
	return true;
}

bool ComponentCamera::PostUpdate(float dt)
{
	return true;
}

bool ComponentCamera::Disable()
{
	return true;
}

bool ComponentCamera::CleanUp()
{
	return true;
}

void ComponentCamera::DrawComponentImGui()
{
	if (ImGui::CollapsingHeader("Camera Component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Camera Component Active", &Active);
		if (ImGui::Checkbox("Main Camera", &MainCamera))
		{
			//If this camera is the MainCamera
			if (MainCamera)
			{
				const std::vector<GameObject*>* SceneGameObjects;
				SceneGameObjects = App->scene->GetAllSceneGameObjects();
				for (std::vector<GameObject*>::const_iterator item = SceneGameObjects->cbegin(); item != SceneGameObjects->cend(); ++item)
				{
					//Search other cameras
					ComponentCamera* CameraComp = (ComponentCamera*)(*item)->FindComponentFirst(ComponentType::Camera_Component);
					// And set them to false
					if ((CameraComp != nullptr) && (CameraComp != this))
						CameraComp->SetMainCamera(false);
				}
			}
		}
		ImGui::Checkbox("Frustum Culling", &FrustumCulling);

		if (ImGui::DragFloat("NearPlaneDistance", &NearPlaneDistance, 0.001f, 0.001f, 1000.0f))
			frustum.nearPlaneDistance = NearPlaneDistance;
		if (ImGui::DragFloat("FarPlaneDistance", &FarPlaneDistance, 1.0f, 0.001f, 1000.0f))
			frustum.farPlaneDistance = FarPlaneDistance;
		if (ImGui::DragFloat("FOV", &FOVVertical, 1.0f, 10.0f, 170.0f))
		{
			AspectRatio = App->window->GetAspectRatio();
			frustum.verticalFov = FOVVertical * DEGTORAD;
			frustum.horizontalFov = Atan(AspectRatio * Tan(frustum.verticalFov * 0.5f)) * 2.0f;
		}
		/*
		ImGui::DragFloat3("FPosition", &Pos[0], 3, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly);
		if (ImGui::DragFloat3("Up", &Up[0], 3, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
			frustum.up = Up;
		if (ImGui::DragFloat3("Front", &Front[0], 3, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly))
			frustum.front = Front;
		*/

		ImGui::Checkbox("Debug Draw Frustrum", &DebugDrawFrustum);
		ImGui::Text("Number of tests: %i", tests);
	}
}

bool ComponentCamera::SaveComponent(JSON_Object* conf) const
{
	App->parsonjson->SetUInt(conf, "UUID", UUID);
	App->parsonjson->SetUInt(conf, "UUID_Parent", parent->GetUUID());
	App->parsonjson->SetBool(conf, "Active", Active);
	App->parsonjson->SetUInt(conf, "Type", type);
	App->parsonjson->SetUInt(conf, "Frustum_Type", frustum.type);
	App->parsonjson->SetFloat(conf, "Near_Plane", frustum.nearPlaneDistance);
	App->parsonjson->SetFloat(conf, "Far_Plane", frustum.farPlaneDistance);
	App->parsonjson->SetFloat(conf, "FOV_Vertical", frustum.verticalFov);
	App->parsonjson->SetFloat3(conf, "Position", frustum.pos);
	App->parsonjson->SetFloat3(conf, "Up", frustum.up);
	App->parsonjson->SetFloat3(conf, "Front", frustum.front);
	App->parsonjson->SetBool(conf, "FrustumCulling", FrustumCulling);
	App->parsonjson->SetBool(conf, "MainCamera", MainCamera);
	App->parsonjson->SetBool(conf, "DebugDrawFrustum", DebugDrawFrustum);
	return true;
}

bool ComponentCamera::LoadComponent(JSON_Object* conf)
{
	UUID = App->parsonjson->GetUInt(conf, "UUID", 0);
	Active = App->parsonjson->GetBool(conf, "Active", true);
	frustum.type = (FrustumType)App->parsonjson->GetUInt(conf, "Frustum_Type", FrustumType::PerspectiveFrustum);
	frustum.nearPlaneDistance = App->parsonjson->GetFloat(conf, "Near_Plane", NearPlaneDistance);
	frustum.farPlaneDistance = App->parsonjson->GetFloat(conf, "Far_Plane", FarPlaneDistance);
	AspectRatio = App->window->GetAspectRatio();
	frustum.verticalFov = App->parsonjson->GetFloat(conf, "FOV_Vertical", FOVVertical);
	frustum.horizontalFov = Atan(AspectRatio * Tan(frustum.verticalFov * 0.5f)) * 2.0f;
	frustum.pos = App->parsonjson->GetFloat3(conf, "Position", float3::zero);
	frustum.up = App->parsonjson->GetFloat3(conf, "Up", float3::zero);
	frustum.front = App->parsonjson->GetFloat3(conf, "Front", float3::zero);
	FrustumCulling = App->parsonjson->GetBool(conf, "FrustumCulling", false);
	MainCamera = App->parsonjson->GetBool(conf, "MainCamera", false);
	DebugDrawFrustum = App->parsonjson->GetBool(conf, "DebugDrawFrustum", false);
	return true;
}

bool ComponentCamera::IsMainCamera() const
{
	return MainCamera;
}

void ComponentCamera::SetMainCamera(bool maincam)
{
	MainCamera = maincam;
}

bool ComponentCamera::IsCulling() const
{
	return FrustumCulling;
}

float3 ComponentCamera::GetPos() const
{
	return parent->GetTransform()->GetPos();
}

float4x4 ComponentCamera::GetViewMatrix() const
{
	math::float4x4 matrix = frustum.ViewMatrix();
	return matrix.Transposed();
}

float4x4 ComponentCamera::GetViewProjMatrix() const
{
	return frustum.ViewProjMatrix().Transposed();
}

void ComponentCamera::SetFrame(float3& pos, float3& front, float3& up)
{
	frustum.pos = pos;
	frustum.front = front;
	frustum.up = up;
}