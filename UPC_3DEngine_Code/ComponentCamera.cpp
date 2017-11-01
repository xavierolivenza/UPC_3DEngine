#include "ComponentCamera.h"
#include "ComponentTransform.h"

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
	return true;
}

bool ComponentCamera::Update(float dt)
{
	if (parent != nullptr)
	{
		ComponentTransform* transform = parent->GetTransform();
		if (transform != nullptr)
		{
			float4x4 matrix = *transform->GetMatrix();
			frustum.pos = matrix.Row3(3);
			frustum.front = matrix.Row3(2);
			frustum.up = matrix.Row3(1);
		}
	}

	if (DebugDrawFrustum)
		App->renderer3D->DrawDebugBox(frustum.CornerPoint(0), frustum.CornerPoint(1), frustum.CornerPoint(2), frustum.CornerPoint(3), frustum.CornerPoint(4), frustum.CornerPoint(5), frustum.CornerPoint(6), frustum.CornerPoint(7));

	//Apply Frustum Culling
	if (FrustumCulling)
	{

	}

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
		ImGui::Checkbox("Main Camera", &MainCamera);
		ImGui::Checkbox("Frustum Culling", &FrustumCulling);

		ImGui::DragFloat("NearPlaneDistance", &NearPlaneDistance, 3, ImGuiInputTextFlags_CharsDecimal);
		ImGui::DragFloat("FarPlaneDistance", &FarPlaneDistance, 3, ImGuiInputTextFlags_CharsDecimal);
		ImGui::DragFloat("FOV", &FOVVertical, 3, ImGuiInputTextFlags_CharsDecimal);
		//ImGui::DragFloat3("FPosition", &Pos[0], 3, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly);
		ImGui::DragFloat3("Up", &Up[0], 3, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly);
		ImGui::DragFloat3("Front", &Front[0], 3, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly);

		/*
		ImGui::InputFloat("NearPlaneDistance", &NearPlaneDistance, 3, ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputFloat("FarPlaneDistance", &FarPlaneDistance, 3, ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputFloat("FOVHoritzontal", &FOVHoritzontal, 3, ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputFloat3("FPosition", &Pos[0], 3, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("Up", &Up[0], 3, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("Front", &Front[0], 3, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly);
		*/

		ImGui::Checkbox("Debug Draw Frustrum", &DebugDrawFrustum);
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
	frustum.verticalFov = App->parsonjson->GetFloat(conf, "FOV_Vertical", FOVVertical) * DEGTORAD;
	frustum.horizontalFov = Atan(AspectRatio * Tan(frustum.verticalFov * 0.5f)) * 2.0f;
	frustum.pos = App->parsonjson->GetFloat3(conf, "Position", float3::zero);
	frustum.up = App->parsonjson->GetFloat3(conf, "Up", float3::zero);
	frustum.front = App->parsonjson->GetFloat3(conf, "Front", float3::zero);
	return true;
}

bool ComponentCamera::IsMainCamera() const
{
	return MainCamera;
}

const float* ComponentCamera::GetViewProjMatrix() const
{
	return frustum.ViewProjMatrix().Transposed().ptr();
}