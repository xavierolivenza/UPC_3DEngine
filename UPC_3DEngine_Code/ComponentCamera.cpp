#include "ComponentCamera.h"
#include "ComponentTransform.h"

ComponentCamera::ComponentCamera(GameObject* parent, bool Active) : Component(parent, Active, 1, ComponentType::Camera_Component)
{
	if (Active) Enable();
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.nearPlaneDistance = NearPlaneDistance;
	frustum.farPlaneDistance = FarPlaneDistance;
	AspectRatio = App->window->GetAspectRatio();
	frustum.verticalFov = FOVHoritzontal * DEGTORAD;
	frustum.horizontalFov = Atan(AspectRatio * Tan(frustum.verticalFov * 0.5f)) * 2.0f;
	frustum.pos = Pos;
	frustum.up = Up;
	frustum.front = Front;
}

ComponentCamera::~ComponentCamera()
{
	CleanFrustumDraw();
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
	if (first_time)
	{
		GenerateFrustumDraw();
		first_time = false;
	}

	if (parent != nullptr)
	{
		/*
		ComponentTransform* transform = (ComponentTransform*)parent->FindComponentFirst(ComponentType::Camera_Component);
		if (transform != nullptr)
		{
			bool frustum_changed = false;
			float3 prev_pos = frustum.pos;
			frustum.pos = transform->GetPos();
			if ((prev_pos.x != frustum.pos.x) || (prev_pos.y != frustum.pos.y) || (prev_pos.z != frustum.pos.z))
				frustum_changed = true;

			float3 prev_front = frustum.front;
			frustum.front = transform->GetMatrix()->Row3(2).Normalized();
			if ((prev_front.x != frustum.front.x) || (prev_front.y != frustum.front.y) || (prev_front.z != frustum.front.z))
				frustum_changed = true;

			float3 prev_up = frustum.up;
			frustum.up = transform->GetMatrix()->Row3(1).Normalized();
			if ((prev_up.x != frustum.up.x) || (prev_up.y != frustum.up.y) || (prev_up.z != frustum.up.z))
				frustum_changed = true;

			if (frustum_changed)
				GenerateFrustumDraw();
		}
		*/
	}
	
	if (DebugDrawFrustum && (DebugDrawFrustum_id_vertices != 0))
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glLineWidth(2.0f);
		//glPushMatrix();
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, DebugDrawFrustum_id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DebugDrawFrustum_id_indices);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, NULL);
		//glPopMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glLineWidth(1.0f);
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
		float temp_NearPlaneDistance = NearPlaneDistance;
		float temp_FarPlaneDistance = FarPlaneDistance;
		float temp_FOVHoritzontal = FOVHoritzontal;
		float3 temp_Pos = Pos;
		float3 temp_Up = Up;
		float3 temp_Front = Front;

		ImGui::DragFloat("NearPlaneDistance", &NearPlaneDistance, 3, ImGuiInputTextFlags_CharsDecimal);
		ImGui::DragFloat("FarPlaneDistance", &FarPlaneDistance, 3, ImGuiInputTextFlags_CharsDecimal);
		ImGui::DragFloat("FOVHoritzontal", &FOVHoritzontal, 3, ImGuiInputTextFlags_CharsDecimal);
		ImGui::DragFloat3("FPosition", &Pos[0], 3, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly);
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

		//Update frustrum
		bool frustum_modified = false;
		if (temp_NearPlaneDistance != NearPlaneDistance)
		{
			frustum.nearPlaneDistance = NearPlaneDistance;
			frustum_modified = true;
		}
		if (temp_FarPlaneDistance != FarPlaneDistance)
		{
			frustum.farPlaneDistance = FarPlaneDistance;
			frustum_modified = true;
		}
		if (temp_FOVHoritzontal != FOVHoritzontal)
		{
			AspectRatio = App->window->GetAspectRatio();
			frustum.verticalFov = FOVHoritzontal * DEGTORAD;
			frustum.horizontalFov = Atan(AspectRatio * Tan(frustum.verticalFov / 2.0f)) * 2.0f;
			frustum_modified = true;
		}
		if ((temp_Pos.x != Pos.x) || (temp_Pos.y != Pos.y) || (temp_Pos.z != Pos.z))
		{
			frustum.pos = Pos;
			frustum_modified = true;
		}
		if ((temp_Up.x != Up.x) || (temp_Up.y != Up.y) || (temp_Up.z != Up.z))
		{
			frustum.up = Up = Up.Normalized();
			frustum_modified = true;
		}
		if ((temp_Front.x != Front.x) || (temp_Front.y != Front.y) || (temp_Front.z != Front.z))
		{
			frustum.front = Front = Front.Normalized();
			frustum_modified = true;
		}
		if(frustum_modified)
			GenerateFrustumDraw();
	}
}

void ComponentCamera::GenerateFrustumDraw()
{
	//Clean if there is another frustum allocated
	CleanFrustumDraw();

	//Box math
	DebugDrawFrustum_vertices = new float[24];
	float vertices[] =
	{
		frustum.CornerPoint(0).x,frustum.CornerPoint(0).y,frustum.CornerPoint(0).z,
		frustum.CornerPoint(1).x,frustum.CornerPoint(1).y,frustum.CornerPoint(1).z,
		frustum.CornerPoint(2).x,frustum.CornerPoint(2).y,frustum.CornerPoint(2).z,
		frustum.CornerPoint(3).x,frustum.CornerPoint(3).y,frustum.CornerPoint(3).z,
		frustum.CornerPoint(4).x,frustum.CornerPoint(4).y,frustum.CornerPoint(4).z,
		frustum.CornerPoint(5).x,frustum.CornerPoint(5).y,frustum.CornerPoint(5).z,
		frustum.CornerPoint(6).x,frustum.CornerPoint(6).y,frustum.CornerPoint(6).z,
		frustum.CornerPoint(7).x,frustum.CornerPoint(7).y,frustum.CornerPoint(7).z
	};
	memcpy(DebugDrawFrustum_vertices, vertices, sizeof(float) * 24);

	DebugDrawFrustum_indices = new uint[24];
	uint indices[] =
	{
		0,2,
		0,4,
		0,1,
		7,6,
		7,3,
		7,5,
		5,1,
		5,4,
		2,3,
		2,6,
		6,4,
		3,1
	};
	memcpy(DebugDrawFrustum_indices, indices, sizeof(uint) * 24);

	//Buffer for vertex
	glGenBuffers(1, (GLuint*)&DebugDrawFrustum_id_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, DebugDrawFrustum_id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, DebugDrawFrustum_vertices, GL_STATIC_DRAW);

	// Buffer for indices
	glGenBuffers(1, (GLuint*)&DebugDrawFrustum_id_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DebugDrawFrustum_id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 24, DebugDrawFrustum_indices, GL_STATIC_DRAW);
}

void ComponentCamera::CleanFrustumDraw()
{
	if (DebugDrawFrustum_vertices != nullptr)
	{
		glDeleteBuffers(1, &DebugDrawFrustum_id_vertices);
		RELEASE_ARRAY(DebugDrawFrustum_vertices);
	}
	if (DebugDrawFrustum_indices != nullptr)
	{
		glDeleteBuffers(1, &DebugDrawFrustum_id_indices);
		RELEASE_ARRAY(DebugDrawFrustum_indices);
	}
}