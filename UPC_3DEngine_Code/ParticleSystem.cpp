#include "ParticleSystem.h"
#include "imgui-1.51\imgui.h"
#include "Glew\include\glew.h"

ParticleEmitter::ParticleEmitter()
{
	BoundingBox.SetNegativeInfinity();
	BoundingBox.minPoint = float3(-0.5f, -0.5f, -0.5f);
	BoundingBox.maxPoint = float3( 0.5f,  0.5f,  0.5f);
}

ParticleEmitter::~ParticleEmitter()
{

}

void ParticleEmitter::DebugDrawEmitter()
{
	switch (Type)
	{
	case EmitterType_Sphere: DrawSphere(EmitterShape.Sphere_Shape); break;
	case EmitterType_SemiSphere: DrawSemiSphere(EmitterShape.Sphere_Shape); break;
	case EmitterType_Cone: DrawCone(EmitterShape.ConeTrunk_Shape); break;
	case EmitterType_Box: DrawBox(EmitterShape.Box_Shape); break;
	case EmitterType_Circle: DrawCircle(EmitterShape.Circle_Shape); break;
	}
}

void ParticleEmitter::DebugDrawEmitterAABB()
{
	DrawBox(BoundingBox);
}

void ParticleEmitter::DrawSphere(const Sphere& shape)
{

}

void ParticleEmitter::DrawSemiSphere(const Sphere& shape)
{

}

void ParticleEmitter::DrawCone(const ConeTrunk& shape)
{

}

void ParticleEmitter::DrawBox(const AABB& shape)
{

}

void ParticleEmitter::DrawCircle(const Circle& shape)
{

}

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
	//Particle Position
	/*
	float dt2half = dt * dt * 0.5f;
	float alpha = atan(Properties.EmissionDirection.y / Properties.EmissionDirection.x);
	float beta = atan(Properties.EmissionDirection.x / Properties.EmissionDirection.z);
	float gamma = atan(Properties.ExternalForce.y / Properties.ExternalForce.x);
	float delta = atan(Properties.ExternalForce.x / Properties.ExternalForce.z);
	Properties.Position.x = Properties.OriginalPosition.x + Properties.Speed * cos(alpha) * cos(beta) * dt + Properties.ExternalForce.Length() * cos(gamma) * cos(delta) * dt2half;
	Properties.Position.y = Properties.OriginalPosition.y + Properties.Speed * sin(alpha) * dt + Properties.ExternalForce.Length() * sin(gamma) * dt2half;
	Properties.Position.z = Properties.OriginalPosition.z + Properties.Speed * cos(alpha) * sin(beta) * dt + Properties.ExternalForce.Length() * cos(gamma) * sin(delta) * dt2half;
	*/
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

void KeyInput::Reset()
{
	Idle = false;
	Up = false;
	Down = false;
	Repeat = false;
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

void ParticleSystem::SetTextureResource(unsigned int ID)
{
	TextureID = ID;
}

void ParticleSystem::SetInitialStateResource(ParticleState& state)
{

}

void ParticleSystem::SetFinalStateResource(ParticleState& state)
{

}

void ParticleSystem::SetEmitterResource(ParticleEmitter& emitter)
{

}

void ParticleSystem::DebugDrawEmitter()
{
	Emitter.DebugDrawEmitter();
}

void ParticleSystem::DebugDrawEmitterAABB()
{
	Emitter.DebugDrawEmitterAABB();
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

AABB& ParticleSystem::GetEmitterAABB()
{
	return Emitter.BoundingBox;
}

void ParticleSystem::DrawTexturePreview()
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	static ImVector<ImVec2> points;
	static bool adding_line = false;
	static float sz = 36.0f;
	float spacing = 8.0f;

	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();            // ImDrawList API uses screen coordinates!
	ImVec2 canvas_size = ImGui::GetContentRegionAvail();        // Resize canvas to what's available
	if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
	if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;
	draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(50, 50, 50), ImColor(50, 50, 60), ImColor(60, 60, 70), ImColor(50, 50, 60));
	draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(255, 255, 255));

	bool adding_preview = false;
	ImGui::InvisibleButton("canvas", canvas_size);
	ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - canvas_pos.x, ImGui::GetIO().MousePos.y - canvas_pos.y);
	if (adding_line)
	{
		adding_preview = true;
		points.push_back(mouse_pos_in_canvas);
		if (!ImGui::GetIO().MouseDown[0])
			adding_line = adding_preview = false;
	}
	if (ImGui::IsItemHovered())
	{
		if (!adding_line && ImGui::IsMouseClicked(0))
		{
			points.push_back(mouse_pos_in_canvas);
			adding_line = true;
		}
		if (ImGui::IsMouseClicked(1) && !points.empty())
		{
			adding_line = adding_preview = false;
			points.pop_back();
			points.pop_back();
		}
	}
	draw_list->PushClipRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y));      // clip lines within the canvas (if we resize it, etc.)
	for (int i = 0; i < points.Size - 1; i += 2)
		//draw_list->AddLine(ImVec2(canvas_pos.x + points[i].x, canvas_pos.y + points[i].y), ImVec2(canvas_pos.x + points[i + 1].x, canvas_pos.y + points[i + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
		draw_list->AddRect(ImVec2(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y), ImVec2(mouse_pos_in_canvas.x + sz, mouse_pos_in_canvas.y + sz), IM_COL32(255, 255, 0, 255), 0.0f, ~0); mouse_pos_in_canvas.x += sz + spacing;
		draw_list->PopClipRect();
	if (adding_preview)
		points.pop_back();
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
	ImGui::Columns(2, "##mycolumns3", false);  // 2-ways, no border
	ImGui::PushItemWidth(100);
	ImGui::SliderFloat("Preview Initial-Final", &Emitter.PreviewState, 0.0f, 1.0f);
	ImGui::PopItemWidth();
	ImGui::PushItemWidth(80);
	ImGui::SliderInt("+-##Lifetime", &Emitter.Lifetime, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::SliderInt("Lifetime+-Var##LifetimeVariation", &Emitter.LifetimeVariation, 0.0f, 100.0f);
	ImGui::DragInt("Emission Duration", &Emitter.EmissionDuration, 0.1f, 0.0f, 100.0f);
	ImGui::Checkbox("Loop", &Emitter.Loop);
	ImGui::DragInt("Particle Num", &Emitter.ParticleNumber, 0.1f, 0.0f, 1000.0f);
	ImGui::SliderFloat("+-##Speed", &Emitter.Speed, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::SliderFloat("Speed+-Var##SpeedVariation", &Emitter.SpeedVariation, 0.0f, 100.0f);
	/*
	ImGui::SliderFloat("+-##ExternalForceX", &Emitter.ExternalForce.x, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::SliderFloat("ExternalForceX+-Var##ExternalForceVariationX", &Emitter.ExternalForceVariation.x, 0.0f, 100.0f);
	ImGui::SliderFloat("+-##ExternalForceY", &Emitter.ExternalForce.y, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::SliderFloat("ExternalForceY+-Var##ExternalForceVariationY", &Emitter.ExternalForceVariation.y, 0.0f, 100.0f);
	ImGui::SliderFloat("+-##ExternalForceZ", &Emitter.ExternalForce.z, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::SliderFloat("ExternalForceZ+-Var##ExternalForceVariationZ", &Emitter.ExternalForceVariation.z, 0.0f, 100.0f);
	*/
	ImGui::PopItemWidth();
	ImGui::NextColumn();
	ImGui::PushItemWidth(120);
	ImGui::Combo("Emyssion Type", (int*)&Emitter.EmissionType, "Local\0World\0");
	ImGui::Combo("Emyssion Shape", (int*)&Emitter.Type, "Sphere\0SemiSphere\0Cone\0Box\0Circle\0Edge");
	ImGui::PopItemWidth();
	//Curve editor to interpolate initial-final
	ImGui::PushItemWidth(240);
	ImGui::InputFloat3("AABB Max", &Emitter.BoundingBox.maxPoint[0], 3);
	ImGui::InputFloat3("AABB Min", &Emitter.BoundingBox.minPoint[0], 3);
	ImGui::PopItemWidth();
}

bool ParticleSystem::CreateParticle()
{
	return true;
}