#include "ComponentCamera.h"

ComponentCamera::ComponentCamera(bool Active) : Component(Active, 0, ComponentType::Camera_Component)
{

}

ComponentCamera::~ComponentCamera()
{
	if (DebugDrawFrustrum_vertices != nullptr)
	{
		glDeleteBuffers(1, &DebugDrawFrustrum_id_vertices);
		RELEASE_ARRAY(DebugDrawFrustrum_vertices);
	}
	if (DebugDrawFrustrum_indices != nullptr)
	{
		glDeleteBuffers(1, &DebugDrawFrustrum_id_indices);
		RELEASE_ARRAY(DebugDrawFrustrum_indices);
	}
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
	if (DebugDrawFrustrum && (DebugDrawFrustrum_id_vertices != 0))
	{
		glColor3f(1.0f, 1.0f, 0.0f);
		glLineWidth(2.0f);
		//glPushMatrix();
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, DebugDrawFrustrum_id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DebugDrawFrustrum_id_indices);
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
		ImGui::InputFloat("NearPlaneDistance", &NearPlaneDistance, 3, ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputFloat("FarPlaneDistance", &FarPlaneDistance, 3, ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputFloat("FOVHoritzontal", &FOVHoritzontal, 3, ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputFloat3("Position", &Pos[0], 3, ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputFloat3("Up", &Up[0], 3, ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputFloat3("Front", &Front[0], 3, ImGuiInputTextFlags_CharsDecimal);

		ImGui::Checkbox("Debug Draw Frustrum", &DebugDrawFrustrum);

		//Update frustrum
		frustrum.SetViewPlaneDistances(NearPlaneDistance, FarPlaneDistance);
		frustrum.SetVerticalFovAndAspectRatio(FOVHoritzontal, AspectRatio);
		frustrum.SetPos(Pos);
		frustrum.SetUp(Up);
		frustrum.SetFront(Front);
	}
}