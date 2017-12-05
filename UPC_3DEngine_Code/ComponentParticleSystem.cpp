#include "ComponentParticleSystem.h"
#include "ParticleSystem.h"
#include "imgui-1.51\imgui.h"
#include "ResourceMesh.h"

ComponentParticleSystem::ComponentParticleSystem(GameObject* parent, bool Active) : Component(parent, Active, 0, ComponentType::ParticleSystem_Component)
{
	PartSystem = new ParticleSystem();
	if (Active) Enable();
}

ComponentParticleSystem::~ComponentParticleSystem()
{
	RELEASE(PartSystem);
}

bool ComponentParticleSystem::Enable()
{
	return true;
}

bool ComponentParticleSystem::PreUpdate(float dt)
{
	PartSystem->PreUpdate(dt);
	return true;
}

bool ComponentParticleSystem::Update(float dt)
{
	PartSystem->Update(dt);
	if (PartSystem->EditorWindowOpen) PartSystem->DrawImGuiEditorWindow();
	return true;
}

bool ComponentParticleSystem::PostUpdate(float dt)
{
	PartSystem->PostUpdate(dt);
	return true;
}

bool ComponentParticleSystem::Disable()
{
	return true;
}

bool ComponentParticleSystem::CleanUp()
{
	PartSystem->CleanUp();
	return true;
}

void ComponentParticleSystem::DrawComponentImGui()
{
	if (ImGui::CollapsingHeader("Particle System Component", ImGuiTreeNodeFlags_DefaultOpen))
		ImGui::Checkbox("Show Particle Editor", &PartSystem->EditorWindowOpen);
}

void ComponentParticleSystem::SetResource(uint uuid)
{
	ResourceMesh* resourceMesh = (ResourceMesh*)App->resources->Get(uuid);
	if (resourceMesh != nullptr)
	{
		ParticleMeshData ParticleMesh;
		ParticleMesh.num_faces = resourceMesh->SimpleMeshDataStruct.num_faces;
		ParticleMesh.id_vertices = resourceMesh->SimpleMeshDataStruct.id_vertices;
		ParticleMesh.num_vertices = resourceMesh->SimpleMeshDataStruct.num_vertices;
		ParticleMesh.vertices = new float[ParticleMesh.num_vertices * 3];
		memcpy(ParticleMesh.vertices, resourceMesh->SimpleMeshDataStruct.vertices, sizeof(float) * ParticleMesh.num_vertices * 3);
		ParticleMesh.id_indices = resourceMesh->SimpleMeshDataStruct.id_indices;
		ParticleMesh.num_indices = resourceMesh->SimpleMeshDataStruct.num_indices;
		ParticleMesh.indices = new uint[ParticleMesh.num_indices];
		memcpy(ParticleMesh.indices, resourceMesh->SimpleMeshDataStruct.indices, sizeof(float) * ParticleMesh.num_indices);
		ParticleMesh.id_normals = resourceMesh->SimpleMeshDataStruct.id_normals;
		ParticleMesh.normals = new float[ParticleMesh.num_vertices * 3];
		memcpy(ParticleMesh.normals, resourceMesh->SimpleMeshDataStruct.normals, sizeof(float) * ParticleMesh.num_vertices * 3);
		ParticleMesh.id_texture_coords = resourceMesh->SimpleMeshDataStruct.id_texture_coords;
		ParticleMesh.texture_coords = new float[ParticleMesh.num_vertices * 3];
		memcpy(ParticleMesh.texture_coords, resourceMesh->SimpleMeshDataStruct.texture_coords, sizeof(float) * ParticleMesh.num_vertices * 3);
		PartSystem->SetMeshResource(ParticleMesh);
	}
}

bool ComponentParticleSystem::SaveComponent(JSON_Object* conf) const
{
	return true;
}

bool ComponentParticleSystem::LoadComponent(JSON_Object* conf)
{
	return true;
}