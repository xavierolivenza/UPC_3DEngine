#include "ComponentParticleSystem.h"
#include "ParticleSystem.h"
#include "imgui-1.51\imgui.h"
#include "ResourceMesh.h"
#include "imgui-1.51\ImGuizmo.h"
#include "ModuleCamera3D.h"
#include "MathGeoLib\Geometry\Frustum.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"

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
	if (EditBoundBox)
	{
		ImGuizmo::Enable(true);
		if ((parent != nullptr) && (App->engineUI->GetSelectedGameObject() == parent) && !(App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT))
		{
			ImGuiIO& io = ImGui::GetIO();
			ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

			float4x4 viewmatrix = App->camera->GetCameraComp()->frustum.ViewMatrix();
			float4x4 projectionmatrix = App->camera->GetCameraComp()->frustum.ProjectionMatrix();
			viewmatrix.Transpose();
			projectionmatrix.Transpose();

			AABB Box = PartSystem->GetEmitterAABB();

			float4x4 MaxMatrix = float4x4::FromTRS(Box.maxPoint, Quat::identity, float3::one);
			MaxMatrix.Transpose();
			float4x4 MinMatrix = float4x4::FromTRS(Box.minPoint, Quat::identity, float3::one);
			MinMatrix.Transpose();

			for (uint i = 0; i < 2; i++)
			{
				if (i == 0) ImGuizmo::Manipulate(viewmatrix.ptr(), projectionmatrix.ptr(), ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, MaxMatrix.ptr());
				else ImGuizmo::Manipulate(viewmatrix.ptr(), projectionmatrix.ptr(), ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, MinMatrix.ptr());

				if (ImGuizmo::IsUsing())
				{
					//matrix.Transpose();
					//if ((parent != nullptr) && (parent->GetParent() != nullptr))
					//matrix = parent->GetParent()->GetTransform()->GetMatrix().Transposed().Inverted() * matrix;
					//float3 position = float3::zero;
					//float3 scale = float3::zero;
					//Quat rotation = Quat::identity;
					//matrix.Decompose(position, rotation, scale);
					//SetPos(position);
					//SetRot(rotation);
					//SetScale(scale);
				}
			}
		}

	}
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
	ImGui::Checkbox("Edit Bounding Box", &EditBoundBox);
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