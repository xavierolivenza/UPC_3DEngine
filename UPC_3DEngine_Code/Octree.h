#pragma once
#include "MathGeoLib\Geometry\AABB.h"
#include <list>
#include <vector>

#define Octree_MAX_ITEMS 2
#define Octree_MIN_SIZE 10.0f

class GameObject;
class ComponentMesh;

class OctreeNode
{
public:
	OctreeNode(const AABB& box);
	virtual ~OctreeNode();

	bool isLeaf() const;

	void Insert(ComponentMesh* obj);
	void Remove(GameObject* obj);

	void DebugDraw();

	void CreateChilds();

	template<typename TYPE>
	int CollectIntersections(std::list<GameObject*>& nodes, const TYPE& frustum) const;

public:
	AABB box;
	std::list<GameObject*> objects;
	OctreeNode* parent = nullptr;
	OctreeNode* childs[8];

};

class Octree
{
public:
	Octree();
	virtual ~Octree();

	void Boundaries(AABB limits);
	void Clear(bool fullclear = true);
	void Remove(GameObject* obj);
	void Insert(GameObject* obj);

	void DebugDraw();

	template<typename TYPE>
	int CollectIntersections(std::list<GameObject*>& nodes, const TYPE& frustum) const;

private:
	OctreeNode* root_node = nullptr;
};