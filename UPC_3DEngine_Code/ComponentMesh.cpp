#include "ComponentMesh.h"

ComponentMesh::ComponentMesh(bool Active, uint replica_limit, ComponentType type) : Component(Active, replica_limit, type)
{

}

ComponentMesh::~ComponentMesh()
{}

bool ComponentMesh::Enable()
{
	return true;
}

bool ComponentMesh::PreUpdate(float dt)
{
	return true;
}

bool ComponentMesh::Update(float dt)
{
	return true;
}

bool ComponentMesh::PostUpdate(float dt)
{
	return true;
}

bool ComponentMesh::Disable()
{
	return true;
}

bool ComponentMesh::CleanUp()
{
	return true;
}

void ComponentMesh::DrawComponentImGui()
{

}