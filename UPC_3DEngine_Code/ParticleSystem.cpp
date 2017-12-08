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
	float3 p0 = shape.CornerPoint(0);
	float3 p1 = shape.CornerPoint(1);
	float3 p2 = shape.CornerPoint(2);
	float3 p3 = shape.CornerPoint(3);
	float3 p4 = shape.CornerPoint(4);
	float3 p5 = shape.CornerPoint(5);
	float3 p6 = shape.CornerPoint(6);
	float3 p7 = shape.CornerPoint(7);

	glLineWidth(4.0f);
	glColor3f(1.0f, 1.0f, 0.0f);

	glBegin(GL_LINES);

	glVertex3f(p0.x, p0.y, p0.z);
	glVertex3f(p1.x, p1.y, p1.z);

	glVertex3f(p0.x, p0.y, p0.z);
	glVertex3f(p2.x, p2.y, p2.z);

	glVertex3f(p0.x, p0.y, p0.z);
	glVertex3f(p4.x, p4.y, p4.z);

	glVertex3f(p7.x, p7.y, p7.z);
	glVertex3f(p6.x, p6.y, p6.z);

	glVertex3f(p7.x, p7.y, p7.z);
	glVertex3f(p3.x, p3.y, p3.z);

	glVertex3f(p7.x, p7.y, p7.z);
	glVertex3f(p5.x, p5.y, p5.z);

	glVertex3f(p5.x, p5.y, p5.z);
	glVertex3f(p1.x, p1.y, p1.z);

	glVertex3f(p5.x, p5.y, p5.z);
	glVertex3f(p4.x, p4.y, p4.z);

	glVertex3f(p2.x, p2.y, p2.z);
	glVertex3f(p3.x, p3.y, p3.z);

	glVertex3f(p2.x, p2.y, p2.z);
	glVertex3f(p6.x, p6.y, p6.z);

	glVertex3f(p6.x, p6.y, p6.z);
	glVertex3f(p4.x, p4.y, p4.z);

	glVertex3f(p3.x, p3.y, p3.z);
	glVertex3f(p1.x, p1.y, p1.z);

	glEnd();

	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void ParticleEmitter::DrawCircle(const Circle& shape)
{

}

ParticleEmitter::EmitterShapeUnion::EmitterShapeUnion()
{

}

Particle::Particle(ParticleSystem* parent, ParticleMeshData* MeshResource) : ParentParticleSystem(parent)
{
	Properties.MeshResource = MeshResource;
}

Particle::~Particle()
{

}

bool Particle::PreUpdate(float dt)
{
	/*
	float4x4 ParticleMatrix = float4x4::FromTRS(Properties.Position, Properties.Rotation, Properties.Scale);
	float4x4::LookAt();
	*/
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
	DrawParticle();
	return true;
}

void Particle::DrawParticle()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, Properties.MeshResource->id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (Properties.MeshResource->normals != nullptr)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, Properties.MeshResource->id_normals);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}
	if (Properties.MeshResource->texture_coords != nullptr) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, Properties.MeshResource->id_texture_coords);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Properties.MeshResource->id_indices);
	glDrawElements(GL_TRIANGLES, Properties.MeshResource->num_indices, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
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

void ParticleTextureData::Set(unsigned int ID, unsigned int width, unsigned int heigth)
{
	TextureID = ID;
	TextureW = width;
	TextureH = heigth;
}

ParticleSystem::ParticleSystem()
{
	SetMeshResourcePlane();
	CreateParticle();
}

ParticleSystem::~ParticleSystem()
{

}

bool ParticleSystem::PreUpdate(float dt)
{
	if (ToDeleteBool_FirstTimeTest)
	{
		GenerateMeshResourceBuffers();
		ToDeleteBool_FirstTimeTest = false;
	}

	bool ret = true;
	for (std::vector<Particle*>::iterator item = Particles.begin(); item != Particles.cend() && ret == true; ++item)
		ret = (*item)->PreUpdate(dt);
	return ret;
}

bool ParticleSystem::Update(float dt)
{
	bool ret = true;
	for (std::vector<Particle*>::iterator item = Particles.begin(); item != Particles.cend() && ret == true; ++item)
		ret = (*item)->Update(dt);
	return ret;
}

bool ParticleSystem::PostUpdate(float dt)
{
	bool ret = true;
	for (std::vector<Particle*>::iterator item = Particles.begin(); item != Particles.cend() && ret == true; ++item)
		ret = (*item)->PostUpdate(dt);
	return ret;
}

bool ParticleSystem::CleanUp()
{
	for (std::vector<Particle*>::iterator item = Particles.begin(); item != Particles.cend(); ++item)
		RELEASE(*item);
	Particles.clear();
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
	//Indices
	ParticleMesh.num_indices = 6;
	ParticleMesh.indices = new unsigned int[ParticleMesh.num_indices];
	unsigned int indices[] =
	{
		2, 1, 0,
		2, 3, 1
	};
	memcpy(ParticleMesh.indices, indices, sizeof(float) * ParticleMesh.num_indices);
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
}

void ParticleSystem::SetTextureResource(unsigned int ID, unsigned int width, unsigned int heigth)
{
	TextureData.Set(ID, width, heigth);
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

void ParticleSystem::SetCameraToFaceBillboards(float3 position, float3 up)
{
	CameraPosition = position;
}

void ParticleSystem::GenerateMeshResourceBuffers()
{
	// Vertices Buffer
	glGenBuffers(1, (GLuint*)&ParticleMesh.id_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, ParticleMesh.id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ParticleMesh.num_vertices * 3, ParticleMesh.vertices, GL_STATIC_DRAW);
	// Indices Buffer
	glGenBuffers(1, (GLuint*)&ParticleMesh.id_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ParticleMesh.id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ParticleMesh.num_indices, ParticleMesh.indices, GL_STATIC_DRAW);
	// Texture coords Buffer
	glGenBuffers(1, (GLuint*)&ParticleMesh.id_texture_coords);
	glBindBuffer(GL_ARRAY_BUFFER, ParticleMesh.id_texture_coords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ParticleMesh.num_indices * 3, ParticleMesh.texture_coords, GL_STATIC_DRAW);
}

void ParticleSystem::DrawTexturePreview()
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	static float texSize = 1.0f;
	static ImVec2 InitialPos = ImVec2(0.0f, 0.0f);
	static ImVec2 FinalPos = ImVec2(0.0f, 0.0f);
	ImGui::SliderFloat("Image Preview Size", &texSize, 0.0f, 3.0f, "%.2f");

	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();            // ImDrawList API uses screen coordinates!
	ImVec2 canvas_size = ImGui::GetContentRegionAvail();        // Resize canvas to what's available
	if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
	if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;
	draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(50, 50, 50), ImColor(50, 50, 60), ImColor(60, 60, 70), ImColor(50, 50, 60));
	bool adding_preview = false;
	ImGui::InvisibleButton("canvas", canvas_size);
	ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
	draw_list->PushClipRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y));      // clip lines within the canvas (if we resize it, etc.)
	draw_list->AddImage((void*)TextureData.TextureID, canvas_pos, ImVec2(canvas_pos.x + TextureData.TextureW * texSize, canvas_pos.y + TextureData.TextureH * texSize), ImVec2(0, 1), ImVec2(1, 0));
	draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(255, 255, 255));
	if (mouse_pos_in_canvas.x > canvas_pos.x && mouse_pos_in_canvas.y > canvas_pos.y && mouse_pos_in_canvas.x < canvas_pos.x + canvas_size.x && mouse_pos_in_canvas.y < canvas_pos.y + canvas_size.y)
	{
		if (MouseLeftClick.State == KeyInput::Down)
			InitialPos = ImVec2(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y);
		if (MouseLeftClick.State == KeyInput::Repeat)
			FinalPos = ImVec2(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y);
	}
	draw_list->AddRect(ImVec2(InitialPos.x, InitialPos.y), ImVec2(FinalPos.x, FinalPos.y), IM_COL32(255, 255, 0, 255), 0.0f, ~0);
	draw_list->PopClipRect();

	//ImGui::SameLine();
	//ImGui::VSliderFloat("##SliverV", );
	//ImGui::SliderFloat("##SliverH", );

	ImGui::DragFloat2("Corner1 UV", corner1UV.ptr(), 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat2("Corner2 UV", corner2UV.ptr(), 0.001f, 0.0f, 1.0f);
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
	Particle* NewParticle = new Particle(this, &ParticleMesh);
	Particles.push_back(NewParticle);
	return true;
}