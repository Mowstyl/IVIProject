/* object.h
 *
 * Interaccion y Visualizacion de la Informacion.
 *
 * Practice 2.
 * Ray tracing.
 *
 * Jose Pascual Molina Masso.
 * Escuela Superior de Ingenieria Informatica de Albacete.
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "glm/vec3.hpp" // glm::vec3

#include "shadinfo.h"
#include "material.h"
#include "ray.h"


/****************/
/* Object class */
/****************/

class Object {

public:

  /* Computes the nearest intersection point between a ray and the object, 
     in the direction of the ray. Returns the t value of that point if it
     is positive, otherwise returns 0. */
  virtual float NearestInt(const Ray &ray) = 0;

  /* Returns the color of the object at the intersection point with the ray */
  virtual glm::vec3 Shade(ShadingInfo &shadInfo) = 0;

  /* Draw the object with OpenGL */
  virtual void Draw() = 0;

  Material *pMaterial;

  glm::vec3 center;  // Center coordinates
  glm::vec3 bounds[2];  // bounds is an array containing the minimum and maximum corner of the box
  // BOUNDING BOXES FTW!!! min in bounds[0], max in bounds[1]
};


/****************/
/* Sphere class */
/****************/

class Sphere : public Object {

public:

  /* Constructor */
  Sphere(const glm::vec3& Center, float Radius, Material *pMat);

  /* Inherited functions that are implemented in this class */
  virtual float NearestInt(const Ray &ray);
  virtual glm::vec3 Shade(ShadingInfo &shadInfo);
  virtual void Draw();

  float radius;      // Radius
};


#endif  // !defined _OBJECT_H_
