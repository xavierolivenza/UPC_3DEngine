#include "ParticleSystem.h"
#include "imgui-1.51\imgui.h"
#include "Glew\include\glew.h"

ParticleEmitter::EmitterShapeUnion::EmitterShapeUnion()
{

}

Particle::Particle()
{

}

Particle::~Particle()
{

}

bool Particle::PreUpdate(float dt)
{
	return true;
}

bool Particle::Update(float dt)
{
	return true;
}

bool Particle::PostUpdate(float dt)
{
	return true;
}

void Particle::DrawParticle()
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
	indices = new unsigned int[num_indices];
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

ParticleSystem::ParticleSystem()
{

}

ParticleSystem::~ParticleSystem()
{

}

bool ParticleSystem::PreUpdate(float dt)
{
	bool ret = false;
	for (std::vector<Particle*>::iterator item = Particles.begin(); item != Particles.cend() && ret == true; ++item)
		ret = (*item)->PreUpdate(dt);
	return ret;
}

bool ParticleSystem::Update(float dt)
{
	bool ret = false;
	for (std::vector<Particle*>::iterator item = Particles.begin(); item != Particles.cend() && ret == true; ++item)
		ret = (*item)->Update(dt);
	return ret;
}

bool ParticleSystem::PostUpdate(float dt)
{
	bool ret = false;
	for (std::vector<Particle*>::iterator item = Particles.begin(); item != Particles.cend() && ret == true; ++item)
		ret = (*item)->PostUpdate(dt);
	return ret;
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
	ParticleMesh.Clean();
	ParticleMesh.num_faces = 2;
	// Vertices
	ParticleMesh.num_vertices = 4;
	ParticleMesh.vertices = new float[ParticleMesh.num_vertices * 3];
	float vertices[] =
	{
		-0.5f,  0.5f, 0,
		0.5f,  0.5f, 0,
		-0.5f, -0.5f, 0,
		0.5f, -0.5f, 0
	};
	memcpy(ParticleMesh.vertices, vertices, sizeof(float) * ParticleMesh.num_vertices * 3);
	// Vertices Buffer
	glGenBuffers(1, (GLuint*) &ParticleMesh.id_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, ParticleMesh.id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ParticleMesh.num_vertices * 3, ParticleMesh.vertices, GL_STATIC_DRAW);
	//Indices
	ParticleMesh.num_indices = 6;
	ParticleMesh.indices = new unsigned int[ParticleMesh.num_indices];
	unsigned int indices[] =
	{
		0, 1, 2,
		1, 3, 2
	};
	memcpy(ParticleMesh.indices, indices, sizeof(float) * ParticleMesh.num_indices);
	// Indices Buffer
	glGenBuffers(1, (GLuint*) &ParticleMesh.id_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ParticleMesh.id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ParticleMesh.num_indices, ParticleMesh.indices, GL_STATIC_DRAW);
	// Texture coords
	ParticleMesh.texture_coords = new float[ParticleMesh.num_vertices * 3];
	float texture_coords[] =
	{
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};
	memcpy(ParticleMesh.texture_coords, texture_coords, sizeof(float) * ParticleMesh.num_vertices * 3);
	// Texture coords Buffer
	glGenBuffers(1, (GLuint*) &ParticleMesh.id_texture_coords);
	glBindBuffer(GL_ARRAY_BUFFER, ParticleMesh.id_texture_coords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ParticleMesh.num_indices * 3, ParticleMesh.texture_coords, GL_STATIC_DRAW);
}

void ParticleSystem::DrawImGuiEditorWindow()
{
	if (EditorWindowOpen)
	{
		ImGui::Begin("Particle Editor", &EditorWindowOpen);

		ImVec2 WindowSize = ImGui::GetWindowSize();
		float ChildsWidth = (WindowSize.x - 28.0f) * 0.33f;
		float ChildsHeight = (WindowSize.y - 52.0f) * 0.5f;

		ImGui::BeginChild("Preview", ImVec2(ChildsWidth, ChildsHeight), true);
		DrawTexturePreview();
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Initial State", ImVec2(ChildsWidth, ChildsHeight), true);
		ParticleStateEnum = InitialState_Enum;
		DrawColorSelector();
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Final State", ImVec2(ChildsWidth, ChildsHeight), true);
		ParticleStateEnum = FinalState_Enum;
		DrawColorSelector();
		ImGui::EndChild();

		ImGui::BeginChild("EmitterGeneralOptions", ImVec2(0, ChildsHeight), true);
		DrawEmitterOptions();
		ImGui::EndChild();

		ImGui::End();
	}
}

void ParticleSystem::DrawTexturePreview()
{

}

void ParticleSystem::DrawColorSelector()
{
	ParticleState* state = &InitialState;
	if (ParticleStateEnum == InitialState_Enum)
		ImGui::Text("Initial State");
	else
	{
		ImGui::Text("Final State");
		state = &FinalState;
	}
	ImGui::Separator();
	int misc_flags = ((state->alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (state->alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (state->options_menu ? 0 : ImGuiColorEditFlags_NoOptions));
	ImGui::Combo("Inputs Mode", &state->inputs_mode, "All Inputs\0No Inputs\0RGB Input\0HSV Input\0HEX Input\0");
	ImGui::Combo("Picker Mode", &state->picker_mode, "Auto\0Square selector\0Triangle wheel\0");
	ImGuiColorEditFlags flags = misc_flags;
	if (!state->alpha) flags |= ImGuiColorEditFlags_NoAlpha; // This is by default if you call ColorPicker3() instead of ColorPicker4()
	if (state->alpha_bar) flags |= ImGuiColorEditFlags_AlphaBar;
	if (state->side_preview) flags |= ImGuiColorEditFlags_NoSidePreview;
	if (state->picker_mode == 1) flags |= ImGuiColorEditFlags_PickerHueBar;
	if (state->picker_mode == 2) flags |= ImGuiColorEditFlags_PickerHueWheel;
	if (state->inputs_mode == 1) flags |= ImGuiColorEditFlags_NoInputs;
	if (state->inputs_mode == 2) flags |= ImGuiColorEditFlags_RGB;
	if (state->inputs_mode == 3) flags |= ImGuiColorEditFlags_HSV;
	if (state->inputs_mode == 4) flags |= ImGuiColorEditFlags_HEX;
	ImGui::ColorPicker4("Color##Color", (float*)&state->RGBATint, flags);
	ImGui::PushItemWidth(80);
	ImGui::SliderFloat("+-##Size", &state->Size, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::SliderFloat("Size+-Var##SizeVariation", &state->SizeVariation, 0.0f, 100.0f);
	ImGui::PopItemWidth();
	ImGui::DragFloat4("Color Var##ColorVariation", (float*)&state->RGBATintVariation, 0.1f, 1.0f, 255.0f);
}

void ParticleSystem::DrawEmitterOptions()
{
	//imgui_demo.cpp line 1482
	ParticleEmitter emitter;

	ImGui::PushItemWidth(80);
	ImGui::SliderInt("+-##Lifetime", &emitter.Lifetime, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::SliderInt("Lifetime+-Var##LifetimeVariation", &emitter.Lifetime, 0.0f, 100.0f);
	ImGui::PopItemWidth();
	ImGui::PushItemWidth(80);
	ImGui::DragInt("Emission Duration", &emitter.EmissionDuration, 0.0f, 100.0f);
	ImGui::PopItemWidth();
	ImGui::Checkbox("Loop", &emitter.Loop);
	ImGui::PushItemWidth(80);
	ImGui::DragInt("Particle Num", &emitter.ParticleNumber, 0.0f, 100.0f);
	ImGui::PopItemWidth();
	ImGui::PushItemWidth(80);
	ImGui::SliderFloat("+-##Speed", &emitter.Speed, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::SliderFloat("Speed+-Var##SpeedVariation", &emitter.SpeedVariation, 0.0f, 100.0f);
	ImGui::PopItemWidth();
	ImGui::PushItemWidth(80);
	ImGui::SliderFloat("+-##ExternalForceX", &emitter.ExternalForce.x, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::SliderFloat("ExternalForceX+-Var##ExternalForceVariationX", &emitter.ExternalForceVariation.x, 0.0f, 100.0f);
	ImGui::PopItemWidth();
	ImGui::PushItemWidth(80);
	ImGui::SliderFloat("+-##ExternalForceY", &emitter.ExternalForce.y, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::SliderFloat("ExternalForceY+-Var##ExternalForceVariationY", &emitter.ExternalForceVariation.y, 0.0f, 100.0f);
	ImGui::PopItemWidth();
	ImGui::PushItemWidth(80);
	ImGui::SliderFloat("+-##ExternalForceZ", &emitter.ExternalForce.z, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::SliderFloat("ExternalForceZ+-Var##ExternalForceVariationZ", &emitter.ExternalForceVariation.z, 0.0f, 100.0f);
	ImGui::PopItemWidth();

}

bool ParticleSystem::CreateParticle()
{
	return true;
}