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
#include "ray.h"
#include "glm/glm.hpp" // glm::vec3, glm::dot




/* Constructor */
BBTree::BBTree ()
{
	pElement = NULL;
	pLeft = NULL;
	pRight = NULL;
	maxLevel = 1;
	center = glm::vec3(0., 0., 0.);
	bounds[0] = glm::vec3(0., 0., 0.);
	bounds[1] = glm::vec3(0., 0., 0.);
}

/* Constructor */
BBTree::BBTree (Object **objects,
				int size,
				int axis)
{
	int leftsize, rightsize;
	glm::vec3 minroot, maxroot;

	this->pElement = NULL;
	this->pLeft = NULL;
	this->pRight = NULL;
	this->maxLevel = 1;
	center = glm::vec3 (0., 0., 0.);
	bounds[0] = glm::vec3(0., 0., 0.);
	bounds[1] = glm::vec3(0., 0., 0.);

	if (size > 1)
		{
			Sort (objects, size, axis, &minroot, &maxroot);
			leftsize = size / 2;
			rightsize = size - leftsize;

			bounds[0] = minroot;
			bounds[1] = minroot;
			this->center = (maxroot + minroot) / 2.0f;

			SetLeft (new BBTree (objects, leftsize, (axis + 1) % 3));
			SetRight (new BBTree (objects + leftsize, rightsize, (axis + 1) % 3));
		}
	else
		{
			bounds[0] = objects[0]->bounds[0];
			bounds[1] = objects[0]->bounds[1];
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
	bounds[0] = glm::vec3(0., 0., 0.);
	bounds[1] = glm::vec3(0., 0., 0.);

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
					minl = pTree->bounds[0];
					maxl = pTree->bounds[1];
					minr = pRight->bounds[0];
					maxr = pRight->bounds[1];
					bounds[0] = glm::vec3 (std::min (minl.x, minr.x), std::min (minl.y, minr.y), std::min (minl.z, minr.z));
					bounds[1] = glm::vec3 (std::max (maxl.x, maxr.x), std::max (maxl.y, maxr.y), std::max (maxl.z, maxr.z));
					center = (bounds[0] + bounds[1]) / 2.0f;
				}
			else
				{
					maxLevel = pTree->maxLevel + 1;
					center = pTree->center;
					bounds[0] = pTree->bounds[0];
					bounds[1] = pTree->bounds[1];
				}
		}
	else if (pRight != NULL)
		{
			maxLevel = pRight->maxLevel + 1;
			center = pRight->center;
			bounds[0] = pRight->bounds[0];
			bounds[1] = pRight->bounds[1];
		}
	else
		{
			maxLevel = 1;
			center = glm::vec3 (0., 0., 0.);
			bounds[0] = glm::vec3(0., 0., 0.);
			bounds[1] = glm::vec3(0., 0., 0.);
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
					minl = pLeft->bounds[0];
					maxl = pLeft->bounds[1];
					minr = pTree->bounds[0];
					maxr = pTree->bounds[1];
					bounds[0] = glm::vec3 (std::min (minl.x, minr.x), std::min (minl.y, minr.y), std::min (minl.z, minr.z));
					bounds[1] = glm::vec3 (std::max (maxl.x, maxr.x), std::max (maxl.y, maxr.y), std::max (maxl.z, maxr.z));
					center = (bounds[0] + bounds[1]) / 2.0f;
				}
			else
				{
					maxLevel = pTree->maxLevel + 1;
					center = pTree->center;
					bounds[0] = pTree->bounds[0];
					bounds[1] = pTree->bounds[1];
				}
		}
	else if (pLeft != NULL)
		{
			maxLevel = pLeft->maxLevel + 1;
			center = pLeft->center;
			bounds[0] = pLeft->bounds[0];
			bounds[1] = pLeft->bounds[1];
		}
	else
		{
			maxLevel = 1;
			center = glm::vec3 (0., 0., 0.);
			bounds[0] = glm::vec3 (0., 0., 0.);
			bounds[1] = glm::vec3(0., 0., 0.);
		}
}

/* Sets the root element */
void
BBTree::SetElement (Object *pElem)
{
	pElement = pElem;

	if (pElem != NULL) {
		center = pElem->center;
		bounds[0] = pElem->bounds[0];
		bounds[1] = pElem->bounds[1];
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
BBTree::NearestInt (const Ray &ray,
					float	  &tnear,
					float	   tmax)
{
	Object *aux,
		   *nearObj = NULL;
	float tl,
		  tr,
		  auxtnear = tmax;

	tnear = 0;
	if (this->Intersects (ray, tmax))
		{
			if (GetMaxLevel () > 1)
				{
					nearObj = this->pRight->NearestInt (ray, tr, tmax);

					if (tr > TMIN && tr < tmax)
						auxtnear = tr;

					aux = this->pLeft->NearestInt (ray, tl, auxtnear);

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
					tnear = this->pElement->NearestInt (ray);
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
BBTree::GetOpacity (const Ray &ray,
					const Ray &invray)
{
	Object *optr;
	glm::vec3 tr = glm::vec3 (1.0f, 1.0f, 1.0f);

	if (this->Intersects (ray, TFAR) && this->Intersects (invray, TFAR))
		{
			if (this->GetMaxLevel () > 1)
				{
					tr = this->GetLeft ()->GetOpacity (ray, invray) * this->GetRight ()->GetOpacity (ray, invray);
				}
			else
				{
					optr = this->GetElement ();

					if (optr->NearestInt (ray) > TMIN && optr->NearestInt (invray) > TMIN)
						{
							tr = optr->pMaterial->Kt;
						}
				}
		}

	return tr;
}

bool
BBTree::Intersects (const Ray &ray,
					float      tmaxarg)
{
	float tmin,
		  tmax,
		  tymin,
		  tymax,
		  tzmin,
		  tzmax;

	tmin = (bounds[ray.sign[0]].x - ray.pos.x) * ray.inv_dir.x;
	tmax = (bounds[1 - ray.sign[0]].x - ray.pos.x) * ray.inv_dir.x;

	tymin = (bounds[ray.sign[1]].y - ray.pos.y) * ray.inv_dir.y;
	tymax = (bounds[1 - ray.sign[1]].y - ray.pos.y) * ray.inv_dir.y;
	
	if ((tmin > tymax) || (tymin > tmax))
		return false;
	
	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	tzmin = (bounds[ray.sign[2]].z - ray.pos.z) * ray.inv_dir.z;
	tzmax = (bounds[1 - ray.sign[2]].z - ray.pos.z) * ray.inv_dir.z;

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	return ((tmin < tmaxarg) && (tmax > TMIN));
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
	*min = objects[0]->bounds[0];
	*max = objects[size - 1]->bounds[1];
	
	for (i = 1; i < size - 2; i++) {
		nowmin = objects[i]->bounds[0];
		nowmax = objects[i]->bounds[1];
		min->x = nowmin.x < min->x ? nowmin.x : min->x;
		min->y = nowmin.y < min->y ? nowmin.y : min->y;
		min->z = nowmin.z < min->z ? nowmin.z : min->z;
		max->x = nowmax.x > max->x ? nowmax.x : max->x;
		max->y = nowmax.y > max->y ? nowmax.y : max->y;
		max->z = nowmax.z > max->z ? nowmax.z : max->z;
	}
}
