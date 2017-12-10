#include "ParticleSystem.h"
#include "imgui-1.51\imgui.h"
#include "Glew\include\glew.h"

ParticleEmitter::ParticleEmitter()
{
	BoundingBox.minPoint = float3(-0.5f, -0.5f, -0.5f);
	BoundingBox.maxPoint = float3( 0.5f,  0.5f,  0.5f);
	ResetEmitterValues();
}

ParticleEmitter::~ParticleEmitter()
{

}

void ParticleEmitter::DebugDrawEmitter()
{
	glPushMatrix();
	glMultMatrixf(Transform.ptr());
	switch (Type)
	{
	case EmitterType_Sphere: DrawSphere(EmitterShape.Sphere_Shape); break;
	case EmitterType_SemiSphere: DrawSemiSphere(EmitterShape.Sphere_Shape); break;
	case EmitterType_Cone: DrawCone(EmitterShape.ConeTrunk_Shape); break;
	case EmitterType_Box: DrawBox(EmitterShape.Box_Shape); break;
	case EmitterType_Circle: DrawCircle(EmitterShape.Circle_Shape); break;
	}
	glPopMatrix();
}

void ParticleEmitter::DebugDrawEmitterAABB()
{
	glPushMatrix();
	glMultMatrixf(Transform.ptr());
	DrawBox(BoundingBox);
	glPopMatrix();
}

void ParticleEmitter::ResetEmitterValues()
{
	switch (Type)
	{
	case EmitterType_Sphere:
	case EmitterType_SemiSphere:
		EmitterShape.Sphere_Shape.r = 1.0f;
		break;
	case EmitterType_Cone:
		EmitterShape.ConeTrunk_Shape.Upper_Circle.r = 1.5f;
		EmitterShape.ConeTrunk_Shape.Bottom_Circle.r = 1.0f;
		EmitterShape.ConeTrunk_Shape.heigth = 1.0f;
		break;
	case EmitterType_Box:
		EmitterShape.Box_Shape.maxPoint = float3(-0.5f, -0.5f, -0.5f);
		EmitterShape.Box_Shape.minPoint = float3(0.5f, 0.5f, 0.5f);
		break;
	case EmitterType_Circle:
		EmitterShape.Circle_Shape.r = 1.0f;
		break;
	}
}

void ParticleEmitter::SetTransform(const float4x4& transform)
{
	Transform = transform;
}

void ParticleEmitter::DrawSphere(const Sphere& shape)
{
	glLineWidth(DEBUG_THICKNESS);
	glColor3f(DEBUG_COLOR_R, DEBUG_COLOR_G, DEBUG_COLOR_B);
	float radius = shape.r;
	float degInRad = 360.0f / CIRCLES_SEGMENTS;
	degInRad *= DEGTORAD;
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCLES_SEGMENTS; i++)
		glVertex3f(cos(degInRad * i) * radius, 0.0f, sin(degInRad * i) * radius);
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCLES_SEGMENTS; i++)
		glVertex3f(cos(degInRad * i) * radius, sin(degInRad * i) * radius, 0.0f);
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCLES_SEGMENTS; i++)
		glVertex3f(0.0f, sin(degInRad * i) * radius, cos(degInRad * i) * radius);
	glEnd();
	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void ParticleEmitter::DrawSemiSphere(const Sphere& shape)
{
	glLineWidth(DEBUG_THICKNESS);
	glColor3f(DEBUG_COLOR_R, DEBUG_COLOR_G, DEBUG_COLOR_B);
	float radius = shape.r;
	float degInRad = 360.0f / CIRCLES_SEGMENTS;
	degInRad *= DEGTORAD;
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCLES_SEGMENTS; i++)
		glVertex3f(cos(degInRad * i) * radius, 0.0f, sin(degInRad * i) * radius);
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i <= CIRCLES_SEGMENTS * 0.5f; i++)
		glVertex3f(cos(degInRad * i) * radius, sin(degInRad * i) * radius, 0.0f);
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i <= CIRCLES_SEGMENTS * 0.5f; i++)
		glVertex3f(0.0f, sin(degInRad * i) * radius, cos(degInRad * i) * radius);
	glEnd();
	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void ParticleEmitter::DrawCone(const ConeTrunk& shape)
{
	glLineWidth(DEBUG_THICKNESS);
	glColor3f(DEBUG_COLOR_R, DEBUG_COLOR_G, DEBUG_COLOR_B);
	float Height = shape.heigth;
	float radius = shape.Bottom_Circle.r;
	float radius_top = shape.Upper_Circle.r;
	float degInRad = 360.0f / CIRCLES_SEGMENTS;
	degInRad *= DEGTORAD;
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCLES_SEGMENTS; i++)
		glVertex3f(cos(degInRad * i) * radius, 0.0f, sin(degInRad * i) * radius);
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i <= CIRCLES_SEGMENTS; i++)
		glVertex3f(cos(degInRad * i) * radius_top, Height, sin(degInRad * i) * radius_top);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(radius, 0.0f, 0.0f);
	glVertex3f(radius_top, Height, 0.0f);
	glVertex3f(-radius_top, Height, 0.0f);
	glVertex3f(-radius, 0.0f, 0.0f);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0f, 0.0f, -radius);
	glVertex3f(0.0f, Height, -radius_top);
	glVertex3f(0.0f, Height, radius_top);
	glVertex3f(0.0f, 0.0f, radius);
	glEnd();
	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
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

	glLineWidth(DEBUG_THICKNESS);
	glColor3f(DEBUG_COLOR_R, DEBUG_COLOR_G, DEBUG_COLOR_B);

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
	glLineWidth(DEBUG_THICKNESS);
	glColor3f(DEBUG_COLOR_R, DEBUG_COLOR_G, DEBUG_COLOR_B);
	float radius = shape.r;
	float degInRad = 360.0f / (float)CIRCLES_SEGMENTS;
	degInRad *= DEGTORAD;
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCLES_SEGMENTS; i++)
		glVertex3f(cos(degInRad * i) * radius, 0.0f, sin(degInRad * i) * radius);
	glEnd();
	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
}

ParticleEmitter::EmitterShapeUnion::EmitterShapeUnion()
{

}

Particle::Particle(ParticleSystem* parent) : ParentParticleSystem(parent)
{

}

Particle::~Particle()
{

}

bool Particle::PreUpdate(float dt)
{
	//glDisable(GL_CULL_FACE);
	switch (ParentParticleSystem->Emitter.ParticleFacingOptions)
	{
	case 0: //Null
	{
		break;
	}
	case 1: //Billboard
	{
		float3 Direction = ParentParticleSystem->CameraPosition - Properties.Position;
		Properties.Rotation = Quat::LookAt(float3(0.0f, 0.0f, 1.0f), Direction, float3(0.0f, 1.0f, 0.0f), float3(0.0f, 1.0f, 0.0f));
		break;
	}
	case 2: //VerticalBillboard
	{
		float3 Direction = ParentParticleSystem->CameraPosition - Properties.Position;
		Direction.y = Properties.Position.y;
		Properties.Rotation = Quat::LookAt(float3(0.0f, 0.0f, 1.0f), Direction, float3(0.0f, 1.0f, 0.0f), float3(0.0f, 1.0f, 0.0f));
		break;
	}
	case 3: //HorizontalBillboard
	{
		float3 Direction = ParentParticleSystem->CameraPosition - Properties.Position;
		Direction.x = Properties.Position.x;
		Properties.Rotation = Quat::LookAt(float3(0.0f, 0.0f, 1.0f), Direction, float3(0.0f, 1.0f, 0.0f), float3(0.0f, 1.0f, 0.0f));
		break;
	}
	}
	
	return true;
}

bool Particle::Update(float dt)
{
	//Save external transform?
	switch (ParentParticleSystem->Emitter.EmissionType)
	{
	case 0: //LocalEmission

		break;
	case 1: //WorldEmission

		break;
	}
	Properties.LifetimeActual += (unsigned int)(dt * 1000.0f);
	CalculateStatesInterpolation();
	if (!MeshChanged) DrawParticle();
	return true;
}

bool Particle::PostUpdate(float dt)
{
	return true;
}

void Particle::DrawParticle()
{
	const ParticleMeshData& Mesh = ParentParticleSystem->GetParticleMeshData();

	glDisable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	if (ParentParticleSystem->TextureData.TextureID != 0)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, ParentParticleSystem->TextureData.TextureID);
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, Mesh.id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (Mesh.normals != nullptr)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, Mesh.id_normals);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}
	if (Mesh.texture_coords != nullptr) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, Mesh.id_texture_coords);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}

	glPushMatrix();
	float4x4 ParticleMatrix = float4x4::FromTRS(Properties.Position, Properties.Rotation, Properties.Scale).Transposed();
	glMultMatrixf(ParticleMatrix.ptr());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh.id_indices);
	glDrawElements(GL_TRIANGLES, Mesh.num_indices, GL_UNSIGNED_INT, NULL);

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Particle::CalculateStatesInterpolation()
{
	float LifetimeFloat = Properties.LifetimeActual * 0.001f;
	CalculatePosition(LifetimeFloat);
	CalculateSpeed(LifetimeFloat);
	CalculateExternalForce(LifetimeFloat);
	CalculateSize(LifetimeFloat);
	CalculateColor(LifetimeFloat);
}

void Particle::CalculatePosition(float LifetimeFloat)
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
}

void Particle::CalculateSpeed(float LifetimeFloat)
{
	Properties.Speed = LERP(InitialState.Speed, FinalState.Speed, LifetimeFloat);
}

void Particle::CalculateExternalForce(float LifetimeFloat)
{
	Properties.ExternalForce.x = LERP(InitialState.ExternalForce.x, FinalState.ExternalForce.x, LifetimeFloat);
	Properties.ExternalForce.y = LERP(InitialState.ExternalForce.y, FinalState.ExternalForce.y, LifetimeFloat);
	Properties.ExternalForce.z = LERP(InitialState.ExternalForce.z, FinalState.ExternalForce.z, LifetimeFloat);
}

void Particle::CalculateSize(float LifetimeFloat)
{
	Properties.Size = LERP(InitialState.Size, FinalState.Size, LifetimeFloat);
}

void Particle::CalculateColor(float LifetimeFloat)
{
	Properties.RGBATint.x = LERP(InitialState.RGBATint.x, FinalState.RGBATint.x, LifetimeFloat);
	Properties.RGBATint.y = LERP(InitialState.RGBATint.y, FinalState.RGBATint.y, LifetimeFloat);
	Properties.RGBATint.z = LERP(InitialState.RGBATint.z, FinalState.RGBATint.z, LifetimeFloat);
	Properties.RGBATint.w = LERP(InitialState.RGBATint.w, FinalState.RGBATint.w, LifetimeFloat);
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
	num_faces = 0;
	if (vertices != nullptr)
	{
		if (id_vertices > 0)
			glDeleteBuffers(1, &id_vertices);
		id_vertices = 0;
		RELEASE_ARRAY(vertices);
	}
	num_indices = 0;
	if (indices != nullptr)
	{
		if (id_indices > 0)
			glDeleteBuffers(1, &id_indices);
		id_indices = 0;
		RELEASE_ARRAY(indices);
	}
	if (normals != nullptr)
	{
		if (id_normals > 0)
			glDeleteBuffers(1, &id_normals);
		id_normals = 0;
		RELEASE_ARRAY(normals);
	}
	if (texture_coords != nullptr)
	{
		if (id_texture_coords > 0)
			glDeleteBuffers(1, &id_texture_coords);
		id_texture_coords = 0;
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
	if (ShowEmitter) DebugDrawEmitter();
	if (ShowEmitterBoundBox) DebugDrawEmitterAABB();
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
	//ToDeleteBool_FirstTimeTest = true;
	//for (std::vector<Particle*>::iterator item = Particles.begin(); item != Particles.cend(); ++item) (*item)->MeshChanged = true;
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
	ToDeleteBool_FirstTimeTest = true;
	for (std::vector<Particle*>::iterator item = Particles.begin(); item != Particles.cend(); ++item) (*item)->MeshChanged = true;
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

void ParticleSystem::SetEmitterTransform(const float4x4 & transform)
{
	Emitter.SetTransform(transform);
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

const ParticleMeshData & ParticleSystem::GetParticleMeshData() const
{
	return ParticleMesh;
}

void ParticleSystem::SetCameraPosToFollow(float3 position)
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
	for (std::vector<Particle*>::iterator item = Particles.begin(); item != Particles.cend(); ++item) (*item)->MeshChanged = false;
}

void ParticleSystem::DrawTexturePreview()
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	//static float texSize = 1.0f;
	static ImVec2 InitialPos = ImVec2(0.0f, 0.0f);
	static ImVec2 FinalPos = ImVec2(0.0f, 0.0f);
	//ImGui::SliderFloat("Image Preview Size", &texSize, 0.0f, 3.0f, "%.2f");

	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();            // ImDrawList API uses screen coordinates!
	ImVec2 region_avaliable = ImGui::GetContentRegionAvail();
	float TexW = 250.0f;
	float TexH = 250.0f;
	float scale = 1.0f;

	if (TextureData.TextureW > 0.0f && TextureData.TextureH > 0.0f)
	{
		scale = region_avaliable.x / TextureData.TextureW;
		TexW = region_avaliable.x;
		TexH = TextureData.TextureH * scale;
	}

	ImVec2 canvas_size = ImVec2(TexW, TexH);        // Resize canvas to what's available
//	if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
//	if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;

	float tex_w = canvas_pos.x + TextureData.TextureW;
	float tex_h = canvas_pos.y + TextureData.TextureH;
	ImTextureID tex_id = (void*)TextureData.TextureID;

	draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(50, 50, 50), ImColor(50, 50, 60), ImColor(60, 60, 70), ImColor(50, 50, 60));
	bool adding_preview = false;
	ImGui::InvisibleButton("canvas", canvas_size);
	ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
	draw_list->PushClipRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y));      // clip lines within the canvas (if we resize it, etc.)
	draw_list->AddImage((void*)TextureData.TextureID, canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImVec2(0, 1), ImVec2(1, 0)); // * TextureData.TextureW & H * texSize if uncommented
	draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(255, 255, 255));
	if (mouse_pos_in_canvas.x > canvas_pos.x && mouse_pos_in_canvas.y > canvas_pos.y && mouse_pos_in_canvas.x < canvas_pos.x + canvas_size.x && mouse_pos_in_canvas.y < canvas_pos.y + canvas_size.y)
	{
		if (MouseLeftClick.State == KeyInput::Down)
			InitialPos = ImVec2(mouse_pos_in_canvas.x - canvas_pos.x, mouse_pos_in_canvas.y - canvas_pos.y);
		if (MouseLeftClick.State == KeyInput::Repeat)
			FinalPos = ImVec2(mouse_pos_in_canvas.x - canvas_pos.x, mouse_pos_in_canvas.y - canvas_pos.y);

		ImGui::BeginTooltip();
		float focus_sz = 50.0f;
		float focus_x = mouse_pos_in_canvas.x - canvas_pos.x - focus_sz * 0.5f; if (focus_x < 0.0f) focus_x = 0.0f; else if (focus_x > tex_w - focus_sz) focus_x = tex_w - focus_sz;
		float focus_y = mouse_pos_in_canvas.y - canvas_pos.y - focus_sz * 0.5f; if (focus_y < 0.0f) focus_y = 0.0f; else if (focus_y > tex_h - focus_sz) focus_y = tex_h - focus_sz;
		ImVec2 uv0 = ImVec2((focus_x) / canvas_size.x, (focus_y) / canvas_size.y);
		ImVec2 uv1 = ImVec2((focus_x + focus_sz) / canvas_size.x, (focus_y + focus_sz) / canvas_size.y);
		uv0 = ImVec2(uv0.x, 1.0f - uv0.y);
		uv1 = ImVec2(uv1.x, 1.0f - uv1.y);
		ImGui::Image(tex_id, ImVec2(128, 128), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
		ImGui::EndTooltip();
	}
	draw_list->AddRect(ImVec2(InitialPos.x + canvas_pos.x, InitialPos.y + canvas_pos.y), ImVec2(FinalPos.x + canvas_pos.x, FinalPos.y + canvas_pos.y), IM_COL32(255, 255, 0, 255), 0.0f, ~0);
	draw_list->PopClipRect();
	//ImGui::SameLine();
	//ImGui::VSliderFloat("##SliverV", );
	//ImGui::SliderFloat("##SliverH", );

	//With the inputs below we aim to have two inputs for the user, you can set the UV with texture coords and OpenGL 0.0f-1.0f coords
	//(if you set coords with one input type, the other is updated with same data)

	ImGui::DragFloat2("Corner1 UV OpenGL", corner1UV.ptr(), 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat2("Corner2 UV OpenGL", corner2UV.ptr(), 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat2("Corner1 UV PixelCoord", corner1UV.ptr(), 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat2("Corner2 UV PixelCoord", corner2UV.ptr(), 0.001f, 0.0f, 1.0f);
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
	ImGui::Columns(3, "##mycolumns3", false);  // 2-ways, no border
	ImGui::PushItemWidth(100);
	if (ImGui::Combo("Emitter Shape", (int*)&Emitter.Type, "Sphere\0SemiSphere\0Cone\0Box\0Circle")) Emitter.ResetEmitterValues();
	ImGui::PopItemWidth();
	switch (Emitter.Type)
	{
	case 0: //EmitterType_Sphere
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("Radius", (float*)&Emitter.EmitterShape.Sphere_Shape.r, 0.1f, 0.0f, 100.0f);
		//ImGui::PopItemWidth();
		//ImGui::PushItemWidth(200);
		//ImGui::DragFloat3("Center Point", (float*)&Emitter.EmitterShape.Sphere_Shape.pos, 0.1f, 0.0f, 100.0f);
		ImGui::PopItemWidth();
		break;
	case 1: //EmitterType_SemiSphere
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("Radius", (float*)&Emitter.EmitterShape.Sphere_Shape.r, 0.1f, 0.0f, 100.0f);
		//ImGui::PopItemWidth();
		//ImGui::PushItemWidth(200);
		//ImGui::DragFloat3("Center Point", (float*)&Emitter.EmitterShape.Sphere_Shape.pos, 0.1f, 0.0f, 100.0f);
		ImGui::PopItemWidth();
		break;
	case 2: //EmitterType_Cone
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("Aperture Radius", (float*)&Emitter.EmitterShape.ConeTrunk_Shape.Upper_Circle.r, 0.1f, 0.0f, 100.0f);
		ImGui::DragFloat("Base Radius", (float*)&Emitter.EmitterShape.ConeTrunk_Shape.Bottom_Circle.r, 0.1f, 0.0f, 100.0f);
		ImGui::DragFloat("Cone Heigth", (float*)&Emitter.EmitterShape.ConeTrunk_Shape.heigth, 0.1f, 0.0f, 100.0f);
		ImGui::PopItemWidth();
		break;
	case 3: //EmitterType_Box
		ImGui::PushItemWidth(200);
		ImGui::DragFloat3("Max Point", (float*)&Emitter.EmitterShape.Box_Shape.maxPoint, 0.1f, -100.0f, 100.0f);
		ImGui::DragFloat3("Min Point", (float*)&Emitter.EmitterShape.Box_Shape.minPoint, 0.1f, -100.0f, 100.0f);
		ImGui::PopItemWidth();
		break;
	case 4: //EmitterType_Circle
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("Radius", (float*)&Emitter.EmitterShape.Circle_Shape.r, 0.1f, 0.0f, 100.0f);
		//ImGui::PopItemWidth();
		//ImGui::PushItemWidth(200);
		//ImGui::DragFloat3("Center Point", (float*)&Emitter.EmitterShape.Circle_Shape.pos, 0.1f, 0.0f, 100.0f);
		ImGui::PopItemWidth();
		break;
	}
	ImGui::NextColumn();
	ImGui::PushItemWidth(100);
	ImGui::SliderFloat("Preview Initial-Final", &Emitter.PreviewState, 0.0f, 1.0f);
	ImGui::PopItemWidth();
	ImGui::PushItemWidth(80);
	ImGui::SliderInt("+-##Lifetime", &Emitter.Lifetime, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::SliderInt("Lifetime +- Var##LifetimeVariation", &Emitter.LifetimeVariation, 0.0f, 100.0f);
	ImGui::DragInt("Emission Duration", &Emitter.EmissionDuration, 0.1f, 0.0f, 100.0f);
	ImGui::Checkbox("Loop", &Emitter.Loop);
	ImGui::DragInt("Particle Num", &Emitter.ParticleNumber, 0.1f, 0.0f, 1000.0f);
	ImGui::SliderFloat("+-##Speed", &Emitter.Speed, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::SliderFloat("Speed +- Var##SpeedVariation", &Emitter.SpeedVariation, 0.0f, 100.0f);
	ImGui::SliderFloat("+-##ExternalForceX", &Emitter.ExternalForce.x, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::SliderFloat("ExternalForceX+-Var##ExternalForceVariationX", &Emitter.ExternalForceVariation.x, 0.0f, 100.0f);
	ImGui::SliderFloat("+-##ExternalForceY", &Emitter.ExternalForce.y, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::SliderFloat("ExternalForceY+-Var##ExternalForceVariationY", &Emitter.ExternalForceVariation.y, 0.0f, 100.0f);
	ImGui::SliderFloat("+-##ExternalForceZ", &Emitter.ExternalForce.z, 0.0f, 100.0f);
	ImGui::SameLine();
	ImGui::SliderFloat("ExternalForceZ+-Var##ExternalForceVariationZ", &Emitter.ExternalForceVariation.z, 0.0f, 100.0f);
	ImGui::PopItemWidth();
	ImGui::NextColumn();
	ImGui::PushItemWidth(175);
	ImGui::Combo("Emyssion Type", (int*)&Emitter.EmissionType, "Local\0World\0");
	ImGui::Combo("Particle Facing Options", (int*)&Emitter.ParticleFacingOptions, "Null\0Billboard\0Vertical Billboard\0Horizontal Billboard");
	ImGui::PopItemWidth();
	//TODO: Curve editor to interpolate initial-final
	ImGui::PushItemWidth(200);
	ImGui::Checkbox("Show Emitter", &ShowEmitter);
	ImGui::Checkbox("Show Emitter Bounding Box", &ShowEmitterBoundBox);
	ImGui::DragFloat3("AABB Max", &Emitter.BoundingBox.maxPoint[0], 0.01f, -100.0f, 100.0f);
	ImGui::DragFloat3("AABB Min", &Emitter.BoundingBox.minPoint[0], 0.01f, -100.0f, 100.0f);
	ImGui::PopItemWidth();
}

bool ParticleSystem::CreateParticle()
{
	Particle* NewParticle = new Particle(this);
	Particles.push_back(NewParticle);
	return true;
}