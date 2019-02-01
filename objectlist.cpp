/* objectlist.cpp
 *
 * Interaccion y Visualizacion de la Informacion.
 *
 * Practice 2.
 * Ray tracing.
 *
 * Jose Pascual Molina Masso.
 * Escuela Superior de Ingenieria Informatica de Albacete.
 */


#include <iostream> 
#include "objectlist.h"
#include "limits.h"


/* Returns the nearest object in the direction of the ray. Returns a pointer to that object and
   the t value of the intersection point, otherwise returns NULL */
Object* ObjectList::NearestInt(const Ray &ray, float& tnear, float tmax) {

	Object *optr, *nearObj = NULL;
	float t;

	if (Length() > 0) {

		tnear = tmax;

		/* Access the object list in sequence */
		optr = First();
		while (optr != NULL) {

			/* Compute ray-object intersection */
			t = optr->NearestInt(ray);
			if ((TMIN < t) && (t < tnear)) {  // Beware of self-occlusion
				nearObj = optr;
				tnear = t;
			}
			optr = Next();
		}
	}

	return nearObj;
}

/* Returns the product of the opacities of the objects found in the direction of the ray */
glm::vec3 ObjectList::GetOpacity(const Ray &ray, const Ray &invray) {

	Object *optr;
	glm::vec3 tr = glm::vec3(1., 1., 1.);

	if (Length() > 0) {

		/* Access the object list in sequence */
		optr = First();
		while (optr != NULL) {

			/* Compute ray-object intersection */
			if (optr->NearestInt(ray) > TMIN && optr->NearestInt(invray) > TMIN)
			{
				tr *= optr->pMaterial->Kt;
				if (tr.x == 0. && tr.y == 0. && tr.z == 0.) {
					break;
				}
			}

			optr = Next();
		}
	}

	return tr;
}

Object** ObjectList::AsArray() {
	int len, i = 0;
	Object **objectArray, *optr;
	glm::vec3 nowmin, nowmax;

	len = Length();
	objectArray = (Object**)malloc(sizeof(Object*) * len);

	optr = First();
	while (optr != NULL) {
		if (i < len) {
			*(objectArray + i) = optr;
		}
		else {
			std::cerr << "El método Length de ObjectList no funciona correctamente" << std::endl;
			exit(-1);
		}
		i++;
		optr = Next();
	}

	return objectArray;
}
