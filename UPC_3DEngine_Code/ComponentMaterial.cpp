#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(bool Active) : Component(Active, 1, ComponentType::Material_Component)
{

}

ComponentMaterial::~ComponentMaterial()
{}

bool ComponentMaterial::Enable()
{
	return true;
}

bool ComponentMaterial::PreUpdate(float dt)
{
	return true;
}

bool ComponentMaterial::Update(float dt)
{
	return true;
}

bool ComponentMaterial::PostUpdate(float dt)
{
	return true;
}

bool ComponentMaterial::Disable()
{
	return true;
}

bool ComponentMaterial::CleanUp()
{
	return true;
}

void ComponentMaterial::DrawComponentImGui()
{

}