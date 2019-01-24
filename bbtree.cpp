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


/* Returns the nearest object in the direction of the ray. Returns a pointer to that object and
the t value of the intersection point, otherwise returns NULL */
Object* BBTree::NearestInt(const glm::vec3& pos, const glm::vec3& dir, float& tnear, float tmax) {

	Object *aux, *nearObj = NULL;
	float tl, tr, auxtnear;

	tnear = 0.0f;
	if (GetMaxLevel() > 1) {
		tr = this->pRight->Intersects(pos, dir, tmax);
		tl = this->pLeft->Intersects(pos, dir, tmax);
		if (tr > TMIN && tr < tmax) {
			if (tl > TMIN && tl < tmax) {
				if (tr < tl) {
					nearObj = this->pRight->NearestInt(pos, dir, tnear, tmax);

					if (tnear >= tl) {
						aux = this->pLeft->NearestInt(pos, dir, auxtnear, tnear);

						if (auxtnear <= TMIN && auxtnear >= tmax && auxtnear < tnear) {
							nearObj = aux;
							tnear = auxtnear;
						}
					}
					else if (tnear <= TMIN || tnear >= tmax) {
						nearObj = this->pLeft->NearestInt(pos, dir, tnear, tmax);
					}
				}
				else if (tl < tr) {
					nearObj = this->pLeft->NearestInt(pos, dir, tnear, tmax);

					if (tnear >= tr) {
						aux = this->pRight->NearestInt(pos, dir, auxtnear, tnear);

						if (auxtnear <= TMIN && auxtnear >= tmax && auxtnear < tnear) {
							nearObj = aux;
							tnear = auxtnear;
						}
					}
					else if (tnear <= TMIN || tnear >= tmax) {
						nearObj = this->pRight->NearestInt(pos, dir, tnear, tmax);
					}
				}
				else {
					aux = this->pLeft->NearestInt(pos, dir, tl, tmax);
					nearObj = this->pRight->NearestInt(pos, dir, tr, tmax);

					if (TMIN < tl && tl < tmax) {
						if (TMIN < tr && tr < tmax) {
							if (tl < tr) {
								tnear = tl;
								nearObj = aux;
							}
							else {
								tnear = tr;
							}
						}
						else {
							tnear = tl;
							nearObj = aux;
						}
					}
				}
			}
			else {
				nearObj = this->pRight->NearestInt(pos, dir, tnear, tmax);
			}
		}
		else {
			if (tl > TMIN && tl < tmax) {
				nearObj = this->pLeft->NearestInt(pos, dir, tnear, tmax);
			}
		}

		if (nearObj == NULL || tnear <= TMIN || tnear >= tmax) {
			tnear = 0;
			nearObj = NULL;
		}
	}
	else {
		tnear = this->pElement->NearestInt(pos, dir);
		if (TMIN < tnear && tnear < tmax) {  // Beware of self-occlusion
			nearObj = this->pElement;
		}
		else {
			tnear = 0;
			nearObj = NULL;
		}
	}

	return nearObj;
}

/* Returns the product of the opacities of the objects found in the direction of the ray */
glm::vec3 BBTree::GetOpacity(const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec3& dir) {

	Object *optr;
	glm::vec3 tr = glm::vec3(1., 1., 1.);

	if (this->GetMaxLevel() > 1) {
		if (this->Intersects(pos1, dir, TFAR) > TMIN && this->Intersects(pos2, -dir, TFAR) > TMIN) {
			tr = this->GetLeft()->GetOpacity(pos1, pos2, dir) * this->GetRight()->GetOpacity(pos1, pos2, dir);
		}
	}
	else {
		optr = this->GetElement();

		if (optr->NearestInt(pos1, dir) > TMIN && optr->NearestInt(pos2, -dir) > TMIN)
		{
			tr = optr->pMaterial->Kt;
		}
	}

	return tr;
}

float BBTree::Intersects(const glm::vec3& pos, const glm::vec3& dir, float tmax) {
	float tmin = 0;
	int count = 0;

	if (dir.x != 0.0) {
		float tx = 0;
		float tx1 = (this->center.x - this->absxyz.x - pos.x) / dir.x;
		float tx2 = (this->center.x + this->absxyz.x - pos.x) / dir.x;
		if (TMIN < tx1 && tx1 < tmax) {
			tmin = tx1;
			count = 1;
			if (TMIN < tx2 && tx2 < tmax) {
				if (tx2 < tmin)
					tmin = tx2;
				count = 2;
			}
		}
		else if (TMIN < tx2 && tx2 < tmax) {
			tmin = tx2;
			count = 1;
		}
	}

	if (count < 2 && dir.y != 0.0) {
		float ty = 0;
		float ty1 = (this->center.y - this->absxyz.y - pos.y) / dir.y;
		float ty2 = (this->center.y + this->absxyz.y - pos.y) / dir.y;
		if (TMIN < ty1 && ty1 < tmax) {
			if (ty1 != tmin)
				count++;
			if (ty1 < tmin)
				tmin = ty1;
			if (count < 2 && TMIN < ty2 && ty2 != tmin) {
				count = 2;
				if (ty2 < tmin)
					tmin = ty2;
			}
		}
		else if (TMIN < ty2 && ty2 < tmax && ty2 != tmin) {
			count++;
			if (ty2 < tmin)
				ty = ty2;
		}
	}

	if (count < 2 && dir.z != 0.0) {
		float tz = 0;
		float tz1 = (this->center.z - this->absxyz.z - pos.z) / dir.z;
		float tz2 = (this->center.z + this->absxyz.z - pos.z) / dir.z;
		if (TMIN < tz1 && tz1 < tmax) {
			if (tz1 != tmin)
				count++;
			if (tz1 < tmin)
				tmin = tz1;
			if (count < 2 && TMIN < tz2 && tz2 != tmin) {
				count = 2;
				if (tz2 < tmin)
					tmin = tz2;
			}
		}
		else if (TMIN < tz2 && tz2 < tmax && tz2 != tmin) {
			count++;
			if (tz2 < tmin)
				tz = tz2;
		}
	}

	return tmin;
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
