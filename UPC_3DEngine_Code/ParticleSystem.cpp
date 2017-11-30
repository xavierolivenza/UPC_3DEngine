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

void ParticleSystem::SetMeshResource(ParticleMeshData& MeshData)
{
	ParticleMesh.Copy(MeshData);
}

void ParticleSystem::SetMeshResourcePlane()
{
	//TODO Load plane to ParticleMesh
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

ParticleMeshData::ParticleMeshData()
{

}

ParticleMeshData::~ParticleMeshData()
{
	Clean();
}

void ParticleMeshData::Copy(ParticleMeshData& Other)
{
	Clean();
	num_faces = Other.num_faces;
	id_vertices = Other.id_vertices;
	num_vertices = Other.num_vertices;
	vertices = new float[num_vertices * 3];
	memcpy(vertices, Other.vertices, sizeof(float) * num_vertices * 3);
	id_indices = Other.id_indices;
	num_indices = Other.num_indices;
	indices = new uint[num_indices];
	memcpy(indices, Other.indices, sizeof(float) * num_indices);
	id_normals = Other.id_normals;
	normals = new float[num_vertices * 3];
	memcpy(normals, Other.normals, sizeof(float) * num_vertices * 3);
	id_texture_coords = Other.id_texture_coords;
	texture_coords = new float[num_vertices * 3];
	memcpy(texture_coords, Other.texture_coords, sizeof(float) * num_vertices * 3);
}

void ParticleMeshData::Clean()
{
	if (vertices != nullptr)
	{
		if (id_vertices > 0)
			glDeleteBuffers(1, &id_vertices);
		RELEASE_ARRAY(vertices);
	}
	if (indices != nullptr)
	{
		if (id_indices > 0)
			glDeleteBuffers(1, &id_indices);
		RELEASE_ARRAY(indices);
	}
	if (normals != nullptr)
	{
		if (id_normals > 0)
			glDeleteBuffers(1, &id_normals);
		RELEASE_ARRAY(normals);
	}
	if (texture_coords != nullptr)
	{
		if (id_texture_coords > 0)
			glDeleteBuffers(1, &id_texture_coords);
		RELEASE_ARRAY(texture_coords);
	}
}