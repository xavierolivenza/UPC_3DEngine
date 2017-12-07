#include <experimental\filesystem>

#include "ComponentParticleSystem.h"
#include "ParticleSystem.h"
#include "imgui-1.51\imgui.h"
#include "ResourceMesh.h"
#include "imgui-1.51\ImGuizmo.h"
#include "ModuleCamera3D.h"
#include "MathGeoLib\Geometry\Frustum.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ModuleResources.h"
#include "ResourceTexture.h"

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
	if (PopUpOpen) ImGuiDrawPopUp();
	PartSystem->MouseLeftClick.Reset();
	switch(App->input->GetMouseButton(SDL_BUTTON_LEFT))
	{
		case KEY_STATE::KEY_IDLE: PartSystem->MouseLeftClick.Idle = true; break;
		case KEY_STATE::KEY_UP: PartSystem->MouseLeftClick.Up = true; break;
		case KEY_STATE::KEY_REPEAT: PartSystem->MouseLeftClick.Repeat = true; break;
		case KEY_STATE::KEY_DOWN: PartSystem->MouseLeftClick.Down = true; break;
	}
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
	{
		ImGui::Checkbox("Show Particle Editor", &PartSystem->EditorWindowOpen);
		ImGui::Checkbox("Edit Bounding Box", &EditBoundBox);
		ImGui::Button("Save Particles Resource", ImVec2(170, 30));
		ImGui::SameLine();
		if (ImGui::Button("Load Particles Resource", ImVec2(180, 30)))
		{
			FileToLoadType = Particle_Resource;
			PopUpOpen = true;
		}
		ImGui::Button("Save Emitter Resource", ImVec2(170, 30));
		ImGui::SameLine();
		if (ImGui::Button("Load Emitter Resource", ImVec2(180, 30)))
		{
			FileToLoadType = Emitter_Resource;
			PopUpOpen = true;
		}
		if (ImGui::Button("Load Texture", ImVec2(120, 30)))
		{
			FileToLoadType = Texture_Resource;
			PopUpOpen = true;
		}
	}
}

void ComponentParticleSystem::SetMeshResource(uint uuid)
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

void ComponentParticleSystem::SetTextureResource(uint uuid)
{
	TextureResource = (ResourceTexture*)App->resources->Get(uuid);
	if (TextureResource != nullptr)
		PartSystem->SetTextureResource(TextureResource->TextureDataStruct.id_texture, TextureResource->TextureDataStruct.texture_w, TextureResource->TextureDataStruct.texture_h);
}

void ComponentParticleSystem::SetParticleResource(uint uuid)
{
	//ParticleState InitialState;
	//PartSystem->SetInitialStateResource(InitialState);
	//ParticleState FinalState;
	//PartSystem->SetFinalStateResource(FinalState);
	//SetTextureResource();
}

void ComponentParticleSystem::SetEmitterResource(uint uuid)
{
	//ParticleEmitter Emitter;
	//PartSystem->SetEmitterResource(Emitter);
}

bool ComponentParticleSystem::SaveComponent(JSON_Object* conf) const
{
	return true;
}

bool ComponentParticleSystem::LoadComponent(JSON_Object* conf)
{
	return true;
}

void ComponentParticleSystem::ImGuiDrawPopUp()
{
	const char* Str = "Wrong Type";
	switch (FileToLoadType)
	{
	case Texture_Resource: Str = "Load Texture"; break;
	case Particle_Resource: Str = "Load Particle"; break;
	case Emitter_Resource: Str = "Load Emitter"; break;
	}

	ImGui::OpenPopup(Str);
	if (ImGui::BeginPopupModal(Str, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_HorizontalScrollbar))
	{
		ImGui::Text("Here are only shown files that are accepted\nextention files.");
		ImGui::BeginChild("File Browser##1", ImVec2(300, 300), true);
		switch (FileToLoadType)
		{
		case Texture_Resource: DrawDirectory(App->importer->Get_Assets_path()->c_str()); break;
		case Particle_Resource: DrawDirectory(App->importer->Get_ParticleSystem_Particles_path()->c_str()); break;
		case Emitter_Resource: DrawDirectory(App->importer->Get_ParticleSystem_Emitter_path()->c_str()); break;
		}
		ImGui::EndChild();
		char file_path[1000] = "";
		sprintf_s(file_path, 1000, "%s", FileToLoad.c_str());
		ImGui::InputText("##input text 1", file_path, 1000, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("Ok##1", ImVec2(50, 20)))
		{
			if (!FileToLoad.empty())
			{
				switch (FileToLoadType)
				{
				case Texture_Resource: ImGuiLoadTexturePopUp(); break;
				case Particle_Resource: ImGuiLoadParticlePopUp(); break;
				case Emitter_Resource: ImGuiLoadEmitterPopUp(); break;
				}
			}
			PopUpOpen = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel##1", ImVec2(50, 20)))
		{
			PopUpOpen = false;
		}
		ImGui::EndPopup();
	}
}

void ComponentParticleSystem::ImGuiLoadTexturePopUp()
{
	size_t bar_pos = FileToLoad.rfind("\\") + 1;
	size_t dot_pos = FileToLoad.rfind(".");
	FileToLoadName = FileToLoad.substr(bar_pos, dot_pos - bar_pos);
	uint Texuuid = App->resources->LoadResource((*App->importer->Get_Library_material_path() + "\\" + FileToLoadName + ".dds").c_str(), FileToLoad.c_str());
	SetTextureResource(Texuuid);
}

void ComponentParticleSystem::ImGuiLoadParticlePopUp()
{

}

void ComponentParticleSystem::ImGuiLoadEmitterPopUp()
{

}

void ComponentParticleSystem::DrawDirectory(const char * directory)
{
	for (std::experimental::filesystem::directory_iterator::value_type file_in_path : std::experimental::filesystem::directory_iterator(directory))
	{
		char title[1000] = "";
		if (std::experimental::filesystem::is_directory(file_in_path.path()) && (file_in_path.path().string().length() < 1000))
		{
			sprintf_s(title, 1000, "%S", file_in_path.path().filename().c_str());
			if (ImGui::TreeNodeEx(title, 0))
			{
				sprintf_s(title, 1000, "%S", file_in_path.path().c_str());
				DrawDirectory(title);
				ImGui::TreePop();
			}
		}
		if (std::experimental::filesystem::is_regular_file(file_in_path.path()) && (file_in_path.path().string().length() < 1000))
		{
			sprintf_s(title, 1000, "%S", file_in_path.path().extension().c_str());
			DirectoryTemporalStr = title;
			bool Valid = false;
			//Change Particle_Resource extention to .particle.json
			//Change Emitter_Resource extention to .emitter.json
			switch (FileToLoadType)
			{
			case Texture_Resource: if ((DirectoryTemporalStr == ".png") || (DirectoryTemporalStr == ".PNG") || (DirectoryTemporalStr == ".jpg") || (DirectoryTemporalStr == ".JPG") || (DirectoryTemporalStr == ".tga") || (DirectoryTemporalStr == ".TGA") || (DirectoryTemporalStr == ".dds") || (DirectoryTemporalStr == ".DDS")) Valid = true; break;
			case Particle_Resource: if (DirectoryTemporalStr == ".json") Valid = true; break;
			case Emitter_Resource: if (DirectoryTemporalStr == ".json") Valid = true; break;
			}
			if (Valid)
			{
				sprintf_s(title, 1000, "%S", file_in_path.path().filename().c_str());
				if (ImGui::TreeNodeEx(title, ImGuiTreeNodeFlags_Leaf))
				{
					if (ImGui::IsItemClicked())
					{
						char path[1000] = "";
						sprintf_s(path, 1000, "%S", file_in_path.path().c_str());
						FileToLoad = path;
					}
					ImGui::TreePop();
				}
			}
		}
	}
}
