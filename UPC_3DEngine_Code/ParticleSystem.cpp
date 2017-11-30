#include "ParticleSystem.h"
#include "imgui-1.51\imgui.h"

ParticleSystem::ParticleSystem()
{

}

ParticleSystem::~ParticleSystem()
{

}

bool ParticleSystem::PreUpdate(float dt)
{
	return true;
}

bool ParticleSystem::Update(float dt)
{
	return true;
}

bool ParticleSystem::PostUpdate(float dt)
{
	return true;
}

bool ParticleSystem::CleanUp()
{
	return true;
}

void ParticleSystem::DrawImGuiEditorWindow()
{
	if (EditorWindowOpen)
	{
		ImGui::Begin("Particle Editor", &EditorWindowOpen);

		ImVec2 WindowSize = ImGui::GetWindowSize();
		float ChildsWidth = (WindowSize.x - 28.0f) / 3.0f;
		float ChildsHeight = (WindowSize.y - 52.0f) / 2.0f;

		ImGui::BeginChild("Preview", ImVec2(ChildsWidth, ChildsHeight), true);

		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Initial State", ImVec2(ChildsWidth, ChildsHeight), true);
		DrawColorSelector();
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Final State", ImVec2(ChildsWidth, ChildsHeight), true);
		DrawColorSelector();
		ImGui::EndChild();

		ImGui::BeginChild("EmitterGeneralOptions", ImVec2(0, ChildsHeight), true);

		ImGui::EndChild();

		ImGui::End();
	}
}

void ParticleSystem::DrawColorSelector()
{
	//imgui_demo.cpp line 835
}

bool ParticleSystem::CreateParticle()
{
	return true;
}

ParticleEmitter::EmitterShapeUnion::EmitterShapeUnion()
{

}