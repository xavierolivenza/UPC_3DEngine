#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

GameObject::GameObject(const char* name, bool active, bool static_game_object): name(name), Active(active), Static(static_game_object)
{
	TransformComponent = CreateTransformComponent(true);
	LCG UUIDGen;
	UUID = UUIDGen.Int();
}

GameObject::~GameObject()
{}

bool GameObject::PreUpdate(float dt)
{
	if (Active)
	{
		for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
			if ((*item)->IsActive())
				(*item)->PreUpdate(dt);
		for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
			if ((*item)->IsActive())
				(*item)->PreUpdate(dt);
	}
	return true;
}

bool GameObject::Update(float dt)
{
	if (Active)
	{
		for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
			if ((*item)->IsActive())
				(*item)->Update(dt);
		for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
			if ((*item)->IsActive())
				(*item)->Update(dt);
	}
	return true;
}

bool GameObject::PostUpdate(float dt)
{
	if (Active)
	{
		for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
			if ((*item)->IsActive())
				(*item)->PostUpdate(dt);
		for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
			if ((*item)->IsActive())
				(*item)->PostUpdate(dt);
	}
	return true;
}

bool GameObject::CleanUp()
{
	for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
		(*item)->CleanUp();
	for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
		(*item)->CleanUp();
	//Release memory here
	//Cleanup loop need improvement iterate all the tree and from leaf nodes iterate backwards to clenaup memory
	return true;
}

bool GameObject::SaveGameObject(JSON_Object* conf) const
{
	App->parsonjson->SetString(conf, "name", name.c_str());
	App->parsonjson->SetUInt(conf, "UUID", UUID);
	if (this->GetParent() != nullptr)
		App->parsonjson->SetUInt(conf, "UUID_Parent", this->GetParent()->UUID);
	else
		App->parsonjson->SetUInt(conf, "UUID_Parent", 0);
	App->parsonjson->SetBool(conf, "Active", Active);
	App->parsonjson->SetBool(conf, "Static", Static);

	/*
	for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
		if ((*item)->IsActive())
			(*item)->SaveComponent(conf);
	*/
	/**/
	for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
		if ((*item)->IsActive())
			(*item)->SaveGameObject(conf);
	/**/

	//JSON_Value* va = json_value_init_array();
	//JSON_Array* array = json_value_get_array(va);
	//json_object_set_value(conf, /*array_name*/, va);
	//json_array_append_number(array, /*value*/);
	return true;
}

bool GameObject::LoadGameObject(JSON_Object* conf)
{
	return true;
}

void GameObject::SetActive(bool active)
{
	if (Active != active)
	{
		Active = active;
		for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
			(*item)->SetActive(Active);
		for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
			(*item)->SetActive(Active);
	}
}

bool GameObject::IsActive()
{
	return Active;
}

const std::vector<GameObject*>* GameObject::GetChildren() const
{
	return &children;
}

const std::vector<Component*>* GameObject::GetComponents() const
{
	return &components;
}

void GameObject::DrawGameObjectImGui()
{
	ImGui::Checkbox("Active", &Active);
	ImGui::Checkbox("Static", &Static);
}

void GameObject::DrawComponentImGui()
{
	for (std::vector<Component*>::iterator item = components.begin(); item != components.cend(); ++item)
		(*item)->DrawComponentImGui();
}

void GameObject::AddChild(GameObject* child)
{
	child->parent = this;
	children.push_back(child);
}

bool GameObject::RemoveChild(GameObject* child)
{
	for (std::vector<GameObject*>::iterator item = children.begin(); item != children.cend(); ++item)
		if ((*item) == child)
		{
			(*item)->CleanUp();
			RELEASE(*item);
			return true;
		}
	return false;
}

ComponentTransform* GameObject::CreateTransformComponent(bool active)
{
	ComponentTransform* transform = new ComponentTransform(this, active);
	components.push_back((Component*)transform);
	return transform;
}

ComponentMesh* GameObject::CreateMeshComponent(bool active)
{
	ComponentMesh* mesh = new ComponentMesh(this, active);
	components.push_back((Component*)mesh);
	return mesh;
}

ComponentMaterial* GameObject::CreateMaterialComponent(bool active)
{
	ComponentMaterial* material = new ComponentMaterial(this, active);
	components.push_back((Component*)material);
	return material;
}

ComponentCamera* GameObject::CreateCameraComponent(bool active)
{
	ComponentCamera* camera = new ComponentCamera(this, active);
	components.push_back((Component*)camera);
	return camera;
}

bool GameObject::RemoveComponent(Component* component)
{
	for (std::vector<Component*>::iterator item = components.begin(); item != components.cend(); ++item)
		if ((*item) == component)
		{
			(*item)->CleanUp();
			RELEASE(*item);
			return true;
		}
	return false;
}

const Component* GameObject::FindComponentFirst(ComponentType type) const
{
	Component* ret = nullptr;
	for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
		if (type == (*item)->GetType())
		{
			ret = (*item);
			break;
		}
	return ret;
}

const Component* GameObject::FindComponentLast(ComponentType type) const
{
	Component* ret = nullptr;
	for (std::vector<Component*>::const_reverse_iterator item = components.crbegin(); item != components.crend(); ++item)
		if (type == (*item)->GetType())
		{
			ret = (*item);
			break;
		}
	return ret;
}

void GameObject::FindComponentVec(std::vector<Component*>& vec, ComponentType type) const
{
	for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
		if (type == (*item)->GetType())
			vec.push_back(*item);
}

ComponentTransform* GameObject::GetTransform() const
{
	return TransformComponent;
}

const GameObject* GameObject::GetParent() const
{
	return parent;
}