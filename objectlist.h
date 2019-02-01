/* objectlist.h
 *
 * Interaccion y Visualizacion de la Informacion.
 *
 * Practice 2.
 * Ray tracing.
 *
 * Jose Pascual Molina Masso.
 * Escuela Superior de Ingenieria Informatica de Albacete.
 */

#ifndef _OBJECTLIST_H_
#define _OBJECTLIST_H_

#include "glm/vec3.hpp" // glm::vec3

#include "object.h"
#include "list.h"
#include "ray.h"


/********************/
/* ObjectList class */
/********************/

class ObjectList : public List<Object*> {

public:

	/* Returns the nearest object in the direction of the ray. Returns a pointer to that object and 
	   the t value of the intersection point, otherwise returns NULL */
	Object* NearestInt(const Ray &ray, float& tnear, float tmax);

	/* Returns the product of the opacities of the objects found in the direction of the ray */
	glm::vec3 GetOpacity(const Ray &ray, const Ray &invray);

	Object** AsArray(); // Returns the list as an array (pointer form)
};

#endif  // !defined _OBJECTLIST_H_
