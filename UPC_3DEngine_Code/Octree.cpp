#include "Octree.h"
#include "GameObject.h"
#include "ComponentMesh.h"


// Octree NODE -------------------------

OctreeNode::OctreeNode(const AABB& box) : box(box)
{
	parent = nullptr;

	for (uint i = 0; i < 8; i++)
		childs[i] = nullptr;
}

OctreeNode::~OctreeNode()
{
	for (uint i = 0; i < 8; i++)
		if (childs[i] != nullptr)
			RELEASE(childs[i]);
}

bool OctreeNode::isLeaf() const
{
	if (childs[0] == nullptr)
		return true;

	return false;
}

void OctreeNode::Insert(ComponentMesh* mesh)
{
	if (isLeaf() && (objects.size() < Octree_MAX_ITEMS ||
		(box.HalfSize().LengthSq() <= Octree_MIN_SIZE * Octree_MIN_SIZE)))
	{
		AABB otherBox;
		mesh->GetTransformedAABB(otherBox);
		if(box.Contains(otherBox))
			objects.push_back(mesh->parent);
	}

	else
	{
		if (isLeaf())
		{
			CreateChilds();
			for (uint i = 0; i < 8; i++)
				childs[i]->Insert(mesh);
		}
	}
}

void OctreeNode::Remove(GameObject* obj)
{
	std::list<GameObject*>::iterator it = std::find(objects.begin(), objects.end(), obj);

	if (it != objects.end())
		objects.erase(it);

	if (!isLeaf())
		for (uint i = 0; i < 8; i++)
			childs[i]->Remove(obj);
}

void OctreeNode::DebugDraw()
{
	App->renderer3D->DrawDebugBox(box.CornerPoint(0), box.CornerPoint(1), box.CornerPoint(2), box.CornerPoint(3), box.CornerPoint(4), box.CornerPoint(5), box.CornerPoint(6), box.CornerPoint(7));
	
	/*
	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(box.Edge(i).a.x, box.Edge(i).a.y, box.Edge(i).a.z);
		glVertex3f(box.Edge(i).b.x, box.Edge(i).b.y, box.Edge(i).b.z);
	}
	*/

	if (childs[0] != nullptr)
		for (uint i = 0; i < 8; i++)
			childs[i]->DebugDraw();
			
}

void OctreeNode::CreateChilds()
{
	// We divide the node into 8 equal parts
	float3 size(box.Size());
	float3 size_new(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);

	float3 center(box.CenterPoint());
	float3 center_new;

	AABB box_new;

	// -X / -Y / -Z
	center_new.Set(center.x - size_new.x * 0.5f, center.y - size_new.y * 0.5f, center.z - size_new.z * 0.5f);
	box_new.SetFromCenterAndSize(center_new, size_new);
	childs[0] = new OctreeNode(box_new);

	// +X / -Y / -Z
	center_new.Set(center.x + size_new.x * 0.5f, center.y - size_new.y * 0.5f, center.z - size_new.z * 0.5f);
	box_new.SetFromCenterAndSize(center_new, size_new);
	childs[1] = new OctreeNode(box_new);

	// -X / +Y / -Z
	center_new.Set(center.x + size_new.x * 0.5f, center.y + size_new.y * 0.5f, center.z + size_new.z * 0.5f);
	box_new.SetFromCenterAndSize(center_new, size_new);
	childs[2] = new OctreeNode(box_new);

	// -X / -Y / +Z
	center_new.Set(center.x - size_new.x * 0.5f, center.y - size_new.y * 0.5f, center.z + size_new.z * 0.5f);
	box_new.SetFromCenterAndSize(center_new, size_new);
	childs[3] = new OctreeNode(box_new);

	// +X / +Y / +Z
	center_new.Set(center.x + size_new.x * 0.5f, center.y + size_new.y * 0.5f, center.z + size_new.z * 0.5f);
	box_new.SetFromCenterAndSize(center_new, size_new);
	childs[4] = new OctreeNode(box_new);

	// +X / +Y / -Z
	center_new.Set(center.x + size_new.x * 0.5f, center.y + size_new.y * 0.5f, center.z - size_new.z * 0.5f);
	box_new.SetFromCenterAndSize(center_new, size_new);
	childs[5] = new OctreeNode(box_new);

	// +X / -Y / +Z
	center_new.Set(center.x + size_new.x * 0.5f, center.y - size_new.y * 0.5f, center.z + size_new.z * 0.5f);
	box_new.SetFromCenterAndSize(center_new, size_new);
	childs[6] = new OctreeNode(box_new);

	// -X / +Y / +Z
	center_new.Set(center.x - size_new.x * 0.5f, center.y + size_new.y * 0.5f, center.z + size_new.z * 0.5f);
	box_new.SetFromCenterAndSize(center_new, size_new);
	childs[7] = new OctreeNode(box_new);
}

// Octree ------------------------------

Octree::Octree()
{
}

Octree::~Octree()
{
}

void Octree::Boundaries(AABB limits)
{
	Clear();
	root_node = new OctreeNode(limits);
}

void Octree::Clear()
{
	RELEASE(root_node);
}

void Octree::Remove(GameObject* obj)
{
	if (root_node != nullptr)
		root_node->Remove(obj);
}

void Octree::Insert(GameObject* obj)
{
	ComponentMesh* mesh = (ComponentMesh*) obj->FindComponentFirst(ComponentType::Mesh_Component);
	if(mesh != nullptr)
		root_node->Insert(mesh);
}

void Octree::DebugDraw()
{
	glBegin(GL_LINES);
	glLineWidth(3.0f);
	glColor3f(1.00f, 0.761f, 0.00f);

	if (root_node != nullptr)
		root_node->DebugDraw();

	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
}

// --------------------------------------