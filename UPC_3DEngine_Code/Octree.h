#pragma once
#include "MathGeoLib\Geometry\AABB.h"
#include <list>
#include <vector>

#define Octree_MAX_ITEMS 2
#define Octree_MIN_SIZE 10.0f

class GameObject;

class OctreeNode
{
public:
	OctreeNode(const AABB& box);
	virtual ~OctreeNode();

	bool isLeaf() const;

	void Insert(GameObject* obj);
	void Remove(GameObject* obj);

	void DebugDraw();

	void CreateChilds();


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
	void Clear();
	void Remove(GameObject* obj);

	void DebugDraw();

	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive) const;

public:
	OctreeNode* root_node = nullptr;
};

template<typename TYPE>
inline void Octree::CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive) const
{
	if (root_node != nullptr)
		root_node->CollectIntersections(objects, primitive);
}