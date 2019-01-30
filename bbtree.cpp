/* bbtree.cpp
*
* Interaccion y Visualizacion de la Informacion.
*
* Trabajo 1c.
* Bounding Boxes y Median cut.
*
* Hernan Indibil de la Cruz Calvo.
* Escuela Superior de Ingenieria Informatica de Albacete.
*/

#include <utility>
#include <algorithm>
#include "bbtree.h"
#include "limits.h"
#include "glm/glm.hpp" // glm::vec3, glm::dot




/* Constructor */
BBTree::BBTree ()
{
	pElement = NULL;
	pLeft = NULL;
	pRight = NULL;
	maxLevel = 1;
	center = glm::vec3(0., 0., 0.);
	absxyz = glm::vec3(0., 0., 0.);
}

/* Constructor */
BBTree::BBTree (Object **objects,
				int size,
				int axis)
{
	int leftsize, rightsize;
	glm::vec3 minroot, maxroot, diff;

	this->pElement = NULL;
	this->pLeft = NULL;
	this->pRight = NULL;
	this->maxLevel = 1;
	center = glm::vec3 (0., 0., 0.);
	absxyz = glm::vec3 (0., 0., 0.);

	if (size > 1)
		{
			Sort (objects, size, axis, &minroot, &maxroot);
			leftsize = size / 2;
			rightsize = size - leftsize;

			diff = maxroot - minroot;
			this->absxyz = glm::vec3 (diff.x / 2.0, diff.y / 2.0, diff.z / 2.0);
			this->center = maxroot - this->absxyz;

			SetLeft (new BBTree (objects, leftsize, (axis + 1) % 3));
			SetRight (new BBTree (objects + leftsize, rightsize, (axis + 1) % 3));
		}
	else
		{
			this->absxyz = objects[0]->absxyz;
			this->center = objects[0]->center;
			SetElement (*objects);
		}
}

/* Constructor */
BBTree::BBTree (ObjectList *objects)
{
	int objectSize, leftsize, rightsize;
	glm::vec3 minroot, maxroot, diff;
	BBTree tree;
	Object **objectArray;

	center = glm::vec3 (0., 0., 0.);
	absxyz = glm::vec3 (0., 0., 0.);

	objectSize = objects->Length ();
	objectArray = objects->AsArray ();

	this->BBTree::BBTree (objectArray, objectSize, 0);
	this->objects = objects;
}

/* Destructor */
BBTree::~BBTree () {
	if (pElement != NULL)
		delete pElement;

	if (pLeft != NULL)
		delete pLeft;
	
	if (pRight != NULL)
		delete pRight;
	/*
	if (objects != NULL)
		delete objects;
	*/
}

/* Sets the left subtree */
void BBTree::SetLeft (BBTree *pTree)
{
	glm::vec3 minl,
			  minr,
		      maxl,
			  maxr,
			  min,
			  max;

	this->pLeft = pTree;
	if (pTree != NULL)
		{
			if (pRight != NULL)
				{
					maxLevel = maxLevel > pTree->maxLevel ? maxLevel : pTree->maxLevel + 1;
					minl = pTree->center - pTree->absxyz;
					maxl = pTree->center + pTree->absxyz;
					minr = pRight->center - pRight->absxyz;
					maxr = pRight->center + pRight->absxyz;
					min = glm::vec3 (std::min (minl.x, minr.x), std::min (minl.y, minr.y), std::min (minl.z, minr.z));
					max = glm::vec3 (std::max (maxl.x, maxr.x), std::max (maxl.y, maxr.y), std::max (maxl.z, maxr.z));
					absxyz = (max - min) / 2.0f;
					center = max - absxyz;
					absxyz = absxyz + FLOATFIX;
				}
			else
				{
					maxLevel = pTree->maxLevel + 1;
					center = pTree->center;
					absxyz = pTree->absxyz + 10.0f;
				}
		}
	else if (pRight != NULL)
		{
			maxLevel = pRight->maxLevel + 1;
			center = pRight->center;
			absxyz = pRight->absxyz;
		}
	else
		{
			maxLevel = 1;
			center = glm::vec3 (0., 0., 0.);
			absxyz = glm::vec3 (0., 0., 0.);
		}
}

/* Sets the right subtree */
void
BBTree::SetRight (BBTree *pTree)
{
	glm::vec3 minl,
			  minr,
			  maxl,
			  maxr,
			  min,
			  max;

	this->pRight = pTree;
	if (pTree != NULL)
		{
			if (pLeft != NULL)
				{
					maxLevel = maxLevel > pTree->maxLevel ? maxLevel : pTree->maxLevel + 1;
					minl = pLeft->center - pLeft->absxyz;
					maxl = pLeft->center + pLeft->absxyz;
					minr = pTree->center - pTree->absxyz;
					maxr = pTree->center + pTree->absxyz;
					min = glm::vec3 (std::min (minl.x, minr.x), std::min (minl.y, minr.y), std::min (minl.z, minr.z));
					max = glm::vec3 (std::max (maxl.x, maxr.x), std::max (maxl.y, maxr.y), std::max (maxl.z, maxr.z));
					absxyz = (max - min) / 2.0f;
					center = max - absxyz;
					absxyz = absxyz + FLOATFIX;
				}
			else
				{
					maxLevel = pTree->maxLevel + 1;
					center = pTree->center;
					absxyz = pTree->absxyz;
				}
		}
	else if (pLeft != NULL)
		{
			maxLevel = pLeft->maxLevel + 1;
			center = pLeft->center;
			absxyz = pLeft->absxyz;
		}
	else
		{
			maxLevel = 1;
			center = glm::vec3 (0., 0., 0.);
			absxyz = glm::vec3 (0., 0., 0.);
		}
}

/* Sets the root element */
void
BBTree::SetElement (Object *pElem)
{
	pElement = pElem;

	if (pElem != NULL) {
		center = pElem->center;
		absxyz = pElem->absxyz;
	}
}

/* Returns the left subtree */
BBTree*
BBTree::GetLeft ()
{
	return pLeft;
}

/* Returns the right subtree */
BBTree*
BBTree::GetRight ()
{
	return pRight;
}

/* Returns the root element */
Object*
BBTree::GetElement ()
{
	return pElement;
}

/* Maximum level of the tree */
int
BBTree::GetMaxLevel ()
{
	return maxLevel;
}

/* Maximum level of the tree */
ObjectList*
BBTree::GetObjectList ()
{
	return objects;
}

/* Returns the nearest object in the direction of the ray. Returns a pointer to that object and
the t value of the intersection point, otherwise returns NULL */
Object*
BBTree::NearestInt (const glm::vec3 &pos,
					const glm::vec3 &dir,
					float		    &tnear,
					float			 tmax)
{
	Object *aux,
		   *nearObj = NULL;
	float tl,
		  tr,
		  auxtnear = tmax;

	tnear = 0;
	if (this->Intersects (pos, dir, tmax))
		{
			if (GetMaxLevel () > 1)
				{
					nearObj = this->pRight->NearestInt (pos, dir, tr, tmax);

					if (tr > TMIN && tr < tmax)
						auxtnear = tr;

					aux = this->pLeft->NearestInt (pos, dir, tl, auxtnear);

					if (tr > TMIN && tr < tmax)
						{
							if (tl > TMIN && tl < tmax)
								{
									if (tr < tl)
										{
											tnear = tr;
										}
									else
										{
											tnear = tl;
											nearObj = aux;
										}
								}
							else
								{
									tnear = tr;
								}
						}
					else
						{
							if (tl > TMIN && tl < tmax)
								{
									tnear = tl;
									nearObj = aux;
								}
							else
								{
									tnear = 0;
									nearObj = NULL;
								}
						}
				}
			else
				{
					tnear = this->pElement->NearestInt (pos, dir);
					if (TMIN < tnear && tnear < tmax)
						{  // Beware of self-occlusion
							nearObj = this->pElement;
						}
					else
						{
							tnear = 0;
							nearObj = NULL;
						}
				}
		}

	return nearObj;
}

/* Returns the product of the opacities of the objects found in the direction of the ray */
glm::vec3
BBTree::GetOpacity (const glm::vec3 &pos1,
					const glm::vec3 &pos2,
					const glm::vec3 &dir)
{
	Object *optr;
	glm::vec3 tr = glm::vec3 (1.0f, 1.0f, 1.0f);

	if (this->Intersects (pos1, dir, TFAR) && this->Intersects (pos2, -dir, TFAR))
		{
			if (this->GetMaxLevel () > 1)
				{
					tr = this->GetLeft ()->GetOpacity (pos1, pos2, dir) * this->GetRight ()->GetOpacity (pos1, pos2, dir);
				}
			else
				{
					optr = this->GetElement ();

					if (optr->NearestInt (pos1, dir) > TMIN && optr->NearestInt (pos2, -dir) > TMIN)
						{
							tr = optr->pMaterial->Kt;
						}
				}
		}

	return tr;
}

bool
BBTree::Intersects (const glm::vec3 &pos,
					const glm::vec3 &dir,
					float tmax)
{
	bool result = false;

	if (dir.x != 0.0)
		{
			float tx1 = (this->center.x - this->absxyz.x - pos.x) / dir.x;
			float tx2 = (this->center.x + this->absxyz.x - pos.x) / dir.x;
			result = (TMIN < tx1 && tx1 < tmax) || (TMIN < tx2 && tx2 < tmax);
		}

	if (!result && dir.y != 0.0)
		{
			float ty1 = (this->center.y - this->absxyz.y - pos.y) / dir.y;
			float ty2 = (this->center.y + this->absxyz.y - pos.y) / dir.y;
			result = (TMIN < ty1 && ty1 < tmax) || (TMIN < ty2 && ty2 < tmax);
		}

	if (!result && dir.z != 0.0)
		{
			float tz1 = (this->center.z - this->absxyz.z - pos.z) / dir.z;
			float tz2 = (this->center.z + this->absxyz.z - pos.z) / dir.z;
			result = (TMIN < tz1 && tz1 < tmax) || (TMIN < tz2 && tz2 < tmax);
		}

	return result;
}

/* This function takes last element as pivot, places
the pivot element at its correct position in sorted
array, and places all smaller (smaller than pivot)
to left of pivot and all greater elements to right
of pivot */
int partition(Object **arr, int low, int high, int axis)
{
	float newcenter, centerpivot;
	Object *pAux;

	switch (axis) {
	case 1:
		centerpivot = arr[high]->center.y;
		break;
	case 2:
		centerpivot = arr[high]->center.z;
		break;
	default:
		centerpivot = arr[high]->center.x;
		break;
	}

	int i = (low - 1);  // Index of smaller element 

	for (int j = low; j <= high - 1; j++)
	{
		// If current element is smaller than or 
		// equal to pivot 
		switch (axis) {
			case 1:
				newcenter = arr[j]->center.y;
				break;
			case 2:
				newcenter = arr[j]->center.z;
				break;
			default:
				newcenter = arr[j]->center.x;
				break;
		}

		if (newcenter <= centerpivot)
		{
			i++;    // increment index of smaller element 
			pAux = arr[i];
			arr[i] = arr[j];
			arr[j] = pAux;
		}
	}
	pAux = arr[i + 1];
	arr[i + 1] = arr[high];
	arr[high] = pAux;
	return (i + 1);
}

/* The main function that implements QuickSort
arr[] --> Array to be sorted,
low  --> Starting index,
high  --> Ending index */
void quickSort(Object **arr, int low, int high, int axis)
{
	if (low < high)
	{
		/* pi is partitioning index, arr[p] is now
		at right place */
		int pi = partition(arr, low, high, axis);

		// Separately sort elements before 
		// partition and after partition 
		quickSort(arr, low, pi - 1, axis);
		quickSort(arr, pi + 1, high, axis);
	}
}

void BBTree::Sort(Object** objects, int size, int axis, glm::vec3 *min, glm::vec3 *max) {
	int i;
	glm::vec3 nowmin, nowmax;

	quickSort(objects, 0, size - 1, axis);
	*min = objects[0]->center - objects[0]->absxyz;
	*max = objects[size - 1]->center + objects[0]->absxyz;
	
	for (i = 1; i < size - 2; i++) {
		nowmin = objects[i]->center - objects[i]->absxyz;
		nowmax = objects[i]->center + objects[i]->absxyz;
		min->x = nowmin.x < min->x ? nowmin.x : min->x;
		min->y = nowmin.y < min->y ? nowmin.y : min->y;
		min->z = nowmin.z < min->z ? nowmin.z : min->z;
		max->x = nowmax.x > max->x ? nowmax.x : max->x;
		max->y = nowmax.y > max->y ? nowmax.y : max->y;
		max->z = nowmax.z > max->z ? nowmax.z : max->z;
	}
}
