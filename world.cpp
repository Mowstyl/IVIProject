/* world.cpp
 *
 * Interaccion y Visualizacion de la Informacion.
 *
 * Practice 2.
 * Ray tracing.
 *
 * Jose Pascual Molina Masso.
 * Escuela Superior de Ingenieria Informatica de Albacete.
 */


#include "world.h"
#include "limits.h"
#include "ray.h"


/* Constructor */
World::World() : objects(), lights() {

	maxDepth = 5;
	bgcolor = glm::vec3(0.0, 0.0, 0.0);
	eye = glm::vec3(0.0, 0.0, 5.0);
	center = glm::vec3(0.0, 0.0, 0.0);
	up = glm::vec3(0.0, 1.0, 0.0);
	Iga = glm::vec3(0.0, 0.0, 0.0);

	ClearStats();
}

/* Reset statistics */
void World::ClearStats() {
	numPrimRays = numShadRays = numReflRays = numRefrRays = numTests = 0;
}

/* Key function in recursive ray tracing */
glm::vec3 World::Trace(const Ray &ray, int depth) {

	Object *nearObj = NULL;
	float tnear, tmax;
	tnear = tmax = TFAR;

	/* Find nearest object in the direction of the ray */
	nearObj = objects.NearestInt(ray, tnear, tmax);

	// If the ray intersects some object ...
	if (nearObj != NULL) { 
                 
		ShadingInfo shadInfo;
		glm::vec3 color;

		shadInfo.ray = ray;
		shadInfo.t = tnear;
		shadInfo.depth = depth;
		shadInfo.pWorld = this;

		color = nearObj->Shade(shadInfo);
		return color;
	}

	// If the ray misses ...
	return bgcolor;  // Background color
}