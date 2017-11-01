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

bool GameObject::SaveGameObject(JSON_Array* array) const
{
	JSON_Value* GameObjectArray_value = json_value_init_object();
	JSON_Object* GameObjectArray_object = json_value_get_object(GameObjectArray_value);

	App->parsonjson->SetString(GameObjectArray_object, "name", name.c_str());
	App->parsonjson->SetUInt(GameObjectArray_object, "UUID", UUID);
	if (this->GetParent() != nullptr)
		App->parsonjson->SetUInt(GameObjectArray_object, "UUID_Parent", this->GetParent()->UUID);
	else
		App->parsonjson->SetUInt(GameObjectArray_object, "UUID_Parent", 0);
	App->parsonjson->SetBool(GameObjectArray_object, "Active", Active);
	App->parsonjson->SetBool(GameObjectArray_object, "Static", Static);

	/**/
	JSON_Value* comp_array_value = json_value_init_array();
	JSON_Array* comp_array = json_value_get_array(comp_array_value);
	if (json_object_set_value(GameObjectArray_object, "Components", comp_array_value) == JSONFailure)
		return false;
	for (std::vector<Component*>::const_iterator item = components.cbegin(); item != components.cend(); ++item)
	{
		JSON_Value* GameObjectArray_value = json_value_init_object();
		JSON_Object* GameObjectArray_object = json_value_get_object(GameObjectArray_value);
		(*item)->SaveComponent(GameObjectArray_object);
		if (json_array_append_value(comp_array, GameObjectArray_value) == JSONFailure)
			return false;
	}
	//json_value_free(array_value);
	/**/
	if (json_array_append_value(array, GameObjectArray_value) == JSONFailure)
		return false;
	/**/
	for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
		(*item)->SaveGameObject(array);
	/**/
	return true;
}

bool GameObject::LoadGameObject(JSON_Object* conf)
{
	name = App->parsonjson->GetString(conf, "name", "Error Name GameObject");
	UUID = App->parsonjson->GetUInt(conf, "UUID", 0);
	Active = App->parsonjson->GetBool(conf, "Active", true);
	Static = App->parsonjson->GetBool(conf, "Static", true);

	JSON_Array* array = json_object_get_array(conf, "Components");
	if (array != nullptr)
	{
		uint array_count = json_array_get_count(array);
		if (array_count < 0)
		{
			LOGP("Empty Components array when trying to load GameObject.");
			return false;
		}
		for (uint i = 0; i < array_count; i++)
		{
			JSON_Object* array_object = json_array_get_object(array, i);
			ComponentType componentType = (ComponentType)App->parsonjson->GetUInt(array_object, "Type", ComponentType::Null_ComponentType);
			switch (componentType)
			{
			case ComponentType::Transform_Component:
			{
				TransformComponent->LoadComponent(array_object);
				break;
			}
			case ComponentType::Mesh_Component:
			{
				ComponentMesh* newMesh = CreateMeshComponent(true);
				newMesh->LoadComponent(array_object);
				break;
			}
			case ComponentType::Material_Component:
			{
				ComponentMaterial* newMaterial = CreateMaterialComponent(true);
				newMaterial->LoadComponent(array_object);
				break;
			}
			case ComponentType::Camera_Component:
			{
				ComponentCamera* newCamera = CreateCameraComponent(true);
				newCamera->LoadComponent(array_object);
				break;
			}
			case ComponentType::Null_ComponentType:
			{
				LOGP("Error trying to load component. Null type.");
				break;
			}
			}
			/*
			
			uint parentUUID = GetUInt(array_object, "UUID_Parent", 0);
			//If parent UUID is = 0, means that this has no parent, so just create the game object and add to root
			if (parentUUID == 0)
			{
				GameObject* newGameObject = App->scene->CreateGameObject("Default name", true, true);
				newGameObject->LoadGameObject(array_object);
				App->scene->AddChildToRoot(newGameObject);
			}
			else
			{
				GameObject* ParentGameObject = App->scene->FindGameObjectWithUUID(parentUUID);
				if (ParentGameObject == nullptr)
				{
					LOGP("Error finding parent game object with UUID: %i", parentUUID);
					continue;
				}
				GameObject* newGameObject = App->scene->CreateGameObject("Default name", true, true);
				newGameObject->LoadGameObject(array_object);
				ParentGameObject->AddChild(newGameObject);
			}
			*/
		}
	}
	else
		LOGP("No array Components Found when trying to load GameObject.");
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

u32 GameObject::GetUUID() const
{
	return UUID;
}

GameObject* GameObject::FindGameObjectWithUUID(u32 UUID_ToSearch)
{
	if (UUID == UUID_ToSearch)
		return this;
	else
	{
		for (std::vector<GameObject*>::const_iterator item = children.cbegin(); item != children.cend(); ++item)
		{
			GameObject* gameobject = (*item)->FindGameObjectWithUUID(UUID_ToSearch);
			if (gameobject != nullptr)
				return gameobject;
		}
	}
	return nullptr;
}