#pragma once
#include <list>
#include <vector>
#include "MathGeoLib\Geometry\AABB.h"
#include "Globals.h"
//#define Octree_MAX_ITEMS 2
//#define Octree_MIN_SIZE 10.0f

class GameObject;
class ComponentMesh;

struct OctreeLimits
{
	uint octreeMaxItems = 2;
	float octreeMinSize = 10.0f;
};

class OctreeNode
{
public:
	OctreeNode(const AABB& box);
	~OctreeNode();

	bool isLeaf() const;

	void Insert(ComponentMesh* obj, OctreeLimits& limits);
	void Remove(ComponentMesh* obj);

	void DebugDraw();

	void CreateChilds();

	//template<typename TYPE>
	int CollectIntersections(std::list<ComponentMesh*>& nodes, const Frustum& frustum) const;

public:
	AABB box;
	std::list<ComponentMesh*> objects;
	OctreeNode* parent = nullptr;
	OctreeNode* childs[8];

};

class Octree
{
public:
	Octree();
	~Octree();

	void Boundaries(AABB limits);
	void Clear(bool fullclear = true);
	void Remove(ComponentMesh* mesh);
	void Insert(GameObject* obj);
	void Insert(ComponentMesh* mesh);

	void DebugDraw();

	//template<typename TYPE>
	int CollectIntersections(std::list<ComponentMesh*>& nodes, const Frustum& frustum) const;

public:
	
	OctreeLimits limits;

private:
	OctreeNode* root_node = nullptr;
};