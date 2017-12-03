#include "ParticleSystem.h"
#include "imgui-1.51\imgui.h"

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
	ParticleMesh.indices = new uint[ParticleMesh.num_indices];
	uint indices[] =
	{
		0, 1, 2,
		1, 3, 2
	};
	memcpy(ParticleMesh.indices, indices, sizeof(float) * ParticleMesh.num_indices);
	// Indices Buffer
	glGenBuffers(1, (GLuint*) &ParticleMesh.id_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ParticleMesh.id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * ParticleMesh.num_indices, ParticleMesh.indices, GL_STATIC_DRAW);
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