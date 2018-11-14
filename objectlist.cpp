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


#include "objectlist.h"
#include "limits.h"


/* Returns the nearest object in the direction of the ray. Returns a pointer to that object and
   the t value of the intersection point, otherwise returns NULL */
Object* ObjectList::NearestInt(const glm::vec3& pos, const glm::vec3& dir, float& tnear, float tmax) {

	Object *optr, *nearObj = NULL;
	float t;

	if (Length() > 0) {

		tnear = tmax;

		/* Access the object list in sequence */
		optr = First();
		while (optr != NULL) {

			/* Compute ray-object intersection */
			t = optr->NearestInt(pos, dir);
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
glm::vec3 ObjectList::GetOpacity(const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec3& dir) {

	Object *optr;
	glm::vec3 tr = glm::vec3(1., 1., 1.);

	if (Length() > 0) {

		/* Access the object list in sequence */
		optr = First();
		while (optr != NULL) {

			/* Compute ray-object intersection */
			if (optr->NearestInt(pos1, dir) > TMIN && optr->NearestInt(pos2, -dir) > TMIN)
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
