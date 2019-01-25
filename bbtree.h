/* bbtree.h
*
* Interaccion y Visualizacion de la Informacion.
*
* Trabajo 1c.
* Bounding Boxes y Median cut.
*
* Hernan Indibil de la Cruz Calvo.
* Escuela Superior de Ingenieria Informatica de Albacete.
*/

#ifndef _BBTREE_H_
#define _BBTREE_H_

#include "glm/vec3.hpp" // glm::vec3

#include <algorithm>
#include "limits.h"
#include "object.h"
#include "objectlist.h"


/********************/
/* BBTree class */
/********************/

class BBTree {

public:

	/* Constructor and destructor */
	BBTree();
	BBTree(ObjectList *objects);
	BBTree(Object **objects, int size, int axis);
	~BBTree();



	/* Functions */
	void SetLeft(BBTree *pTree);  // Sets the left subtree
	void SetRight(BBTree *pTree);  // Sets the right subtree
	void SetElement(Object *pElem); // Sets the root element
	BBTree* GetLeft();       // Returns the left subtree
	BBTree* GetRight();       // Returns the right subtree
	Object* GetElement();      // Returns the root element
	int GetMaxLevel();         // Maximum level of the tree

	/* Returns the nearest object in the direction of the ray. Returns a pointer to that object and
	the t value of the intersection point, otherwise returns NULL */
	Object* NearestInt(const glm::vec3& pos, const glm::vec3& dir, float& tnear, float tmax);

	/* Returns the product of the opacities of the objects found in the direction of the ray */
	glm::vec3 GetOpacity(const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec3& dir);

	ObjectList *objects;
private:
	Object *pElement;
	BBTree *pLeft, *pRight;
	int maxLevel;

	glm::vec3 center = glm::vec3(0., 0., 0.);		// Center coordinates
	glm::vec3 absxyz = glm::vec3(0., 0., 0.);		// Max |x|, |y| and |z| of the boundingbox

	void Sort(Object** objects, int size, int axis, glm::vec3 *min, glm::vec3 *max);
	bool Intersects(const glm::vec3& pos, const glm::vec3& dir, float tmax);
};

/* Constructor */
inline BBTree::BBTree() {
	pElement = NULL;
	pLeft = NULL;
	pRight = NULL;
	maxLevel = 1;
}

/* Constructor */
inline BBTree::BBTree(Object **objects, int size, int axis) {
	int leftsize, rightsize;
	glm::vec3 minroot, maxroot, diff;
	this->pElement = NULL;
	this->pLeft = NULL;
	this->pRight = NULL;
	this->maxLevel = 1;

	if (size > 1) {
		Sort(objects, size, axis, &minroot, &maxroot);
		leftsize = size / 2;
		rightsize = size - leftsize;

		diff = maxroot - minroot;
		this->absxyz = glm::vec3(diff.x / 2.0, diff.y / 2.0, diff.z / 2.0);
		this->center = maxroot - this->absxyz;

		SetLeft(new BBTree(objects, leftsize, (axis + 1) % 3));
		SetRight(new BBTree(objects + leftsize, rightsize, (axis + 1) % 3));
	}
	else {
		this->absxyz = objects[0]->absxyz;
		this->center = objects[0]->center;
		SetElement(*objects);
		//ObjectList *ob = new ObjectList();
	}
}

/* Constructor */
inline BBTree::BBTree(ObjectList *objects) {
	int objectSize, leftsize, rightsize;
	glm::vec3 minroot, maxroot, diff;
	BBTree tree;
	Object **objectArray;

	objectSize = objects->Length();
	objectArray = objects->AsArray();

	this->BBTree::BBTree(objectArray, objectSize, 0);
	this->objects = objects;
}

/* Destructor */
inline BBTree::~BBTree() {
	if (pElement != NULL) {
		delete pElement;
	}
	if (pLeft != NULL) {
		delete pLeft;
	}
	if (pRight != NULL) {
		delete pRight;
	}/*
	if (objects != NULL) {
		delete objects;
	}*/
}

/* Sets the left subtree */
inline void BBTree::SetLeft(BBTree *pTree) {
	glm::vec3 minl, minr, maxl, maxr, min, max;

	this->pLeft = pTree;
	if (pTree != NULL) {
		if (pRight != NULL) {
			maxLevel = maxLevel > pTree->maxLevel ? maxLevel : pTree->maxLevel + 1;
			minl = pTree->center - pTree->absxyz;
			maxl = pTree->center + pTree->absxyz;
			minr = pRight->center - pRight->absxyz;
			maxr = pRight->center + pRight->absxyz;
			min = glm::vec3(std::min(minl.x, minr.x), std::min(minl.y, minr.y), std::min(minl.z, minr.z));
			max = glm::vec3(std::max(maxl.x, maxr.x), std::max(maxl.y, maxr.y), std::max(maxl.z, maxr.z));
			absxyz = (max - min) / 2.0f;
			center = max - absxyz;
			absxyz = absxyz + FLOATFIX;
		}
		else {
			maxLevel = pTree->maxLevel + 1;
			center = pTree->center;
			absxyz = pTree->absxyz + 10.0f;
		}
	}
	else if (pRight != NULL) {
		maxLevel = pRight->maxLevel + 1;
		center = pRight->center;
		absxyz = pRight->absxyz;
	}
	else {
		maxLevel = 1;
		center = glm::vec3(0., 0., 0.);
		absxyz = glm::vec3(0., 0., 0.);
	}
}

/* Sets the right subtree */
inline void BBTree::SetRight(BBTree *pTree) {
	glm::vec3 minl, minr, maxl, maxr, min, max;

	this->pRight = pTree;
	if (pTree != NULL) {
		if (pLeft != NULL) {
			maxLevel = maxLevel > pTree->maxLevel ? maxLevel : pTree->maxLevel + 1;
			minl = pLeft->center - pLeft->absxyz;
			maxl = pLeft->center + pLeft->absxyz;
			minr = pTree->center - pTree->absxyz;
			maxr = pTree->center + pTree->absxyz;
			min = glm::vec3(std::min(minl.x, minr.x), std::min(minl.y, minr.y), std::min(minl.z, minr.z));
			max = glm::vec3(std::max(maxl.x, maxr.x), std::max(maxl.y, maxr.y), std::max(maxl.z, maxr.z));
			absxyz = (max - min) / 2.0f;
			center = max - absxyz;
			absxyz = absxyz + FLOATFIX;
		}
		else {
			maxLevel = pTree->maxLevel + 1;
			center = pTree->center;
			absxyz = pTree->absxyz;
		}
	}
	else if (pLeft != NULL) {
		maxLevel = pLeft->maxLevel + 1;
		center = pLeft->center;
		absxyz = pLeft->absxyz;
	}
	else {
		maxLevel = 1;
		center = glm::vec3(0., 0., 0.);
		absxyz = glm::vec3(0., 0., 0.);
	}
}

/* Sets the root element */
inline void BBTree::SetElement(Object *pElem) {
	pElement = pElem;

	if (pElem != NULL) {
		center = pElem->center;
		absxyz = pElem->absxyz;
	}
}

/* Returns the left subtree */
inline BBTree* BBTree::GetLeft() {
	return pLeft;
}

/* Returns the right subtree */
inline BBTree* BBTree::GetRight() {
	return pRight;
}

/* Returns the root element */
inline Object* BBTree::GetElement() {
	return pElement;
}

/* Maximum level of the tree */
inline int BBTree::GetMaxLevel() {
	return maxLevel;
}

#endif  // !defined _BBTREE_H_
