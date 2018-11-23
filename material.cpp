/* Material.cpp
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

#include "glm/glm.hpp" // glm::vec3, glm::dot

#include "Material.h"
#include "light.h"
#include "lightlist.h"
#include "world.h"
#include "limits.h"


/* Constructors */
Material::Material(const glm::vec3 &diff) {

	Ka = glm::vec3(0.0, 0.0, 0.0);
	Kd = diff;
	Kdt = glm::vec3(0.0, 0.0, 0.0);
	Ks = glm::vec3(0.0, 0.0, 0.0);
	Kst = glm::vec3(0.0, 0.0, 0.0);
	n = 0;
	Kr = glm::vec3(0.0, 0.0, 0.0);
	Kt = glm::vec3(0.0, 0.0, 0.0);
	ior = 0.0;
}

Material::Material(const glm::vec3 &amb, const glm::vec3 &diff, const glm::vec3 &diffTrans,
	const glm::vec3 &spec, const glm::vec3 &specTrans, int shine, const glm::vec3 &emis,
	const glm::vec3 &refl, const glm::vec3 &trans, float index) {

	Ka = amb;
	Kd = diff;
	Ks = spec;
	n = shine;
	Kr = refl;
	Kt = trans;
	ior = index;

	Kdt = diffTrans;
	Kst = specTrans;
	Ie = emis;
}

glm::vec3* refractionRay(const glm::vec3& I, const glm::vec3& N, float _IdotN, float a) {
	float aux = 1 + a * a * (_IdotN * _IdotN - 1);
	if (aux >= 0) {
		float b = a * _IdotN - sqrt(aux);
		return &glm::normalize(a * I + b * N);
	}
	else {
		return NULL;
	}
}

/* Implements the global illumination model */
glm::vec3 Material::Shade(ShadingInfo &shadInfo)
{
	glm::vec3 color(0.0, 0.0, 0.0), V;
	float VdotN, ratio;
	bool isTrans;

	V = -shadInfo.rayDir;
	VdotN = glm::dot(V, shadInfo.normal);
	isTrans = (Kt.x > 0. || Kt.y > 0. || Kt.z > 0.);
	if (VdotN < 0) {

		// The viewer stares at an interior or back face of the object,
		// we will only illuminate it if material is transparent
		if (isTrans) {
			shadInfo.normal = -shadInfo.normal;  // Reverse normal
			VdotN = -VdotN;
			ratio = 1.0 / ior; // Ray always comes from vacuum (hollow objects)
			//ratio = ior;  // Use this instead for solid objects
		}
		else
			return color;
	}
	else {
		// The viewer stares at a front face of the object
		if (isTrans)
			ratio = 1.0 / ior; // Ray comes from vacuum
	}
	// To do ...

	glm::vec3 Ia = glm::vec3(0., 0., 0.);
	glm::vec3 Id = glm::vec3(0., 0., 0.);
	glm::vec3 Is = glm::vec3(0., 0., 0.);
	glm::vec3 Idt = glm::vec3(0., 0., 0.);
	glm::vec3 Ist = glm::vec3(0., 0., 0.);

	Light *clight = shadInfo.pWorld->lights.First();
	while (clight != NULL) {
		glm::vec3 L = glm::normalize(clight->position - shadInfo.point);

		shadInfo.pWorld->numShadRays++;
		glm::vec3 tr = shadInfo.pWorld->objects.GetOpacity(shadInfo.point, clight->position, L);

		float LdotN = glm::dot(L, shadInfo.normal);

		Ia += tr * clight->Ia;
		if (LdotN > 0) {
			glm::vec3 R = glm::normalize(2 * LdotN*shadInfo.normal - L); // reflexion perfecta

			Id += tr * clight->Id * LdotN;
			Is += tr * clight->Is * pow(glm::dot(R, V), n);
		}
		else if (LdotN < 0 && isTrans) {
			glm::vec3* pT = refractionRay(-L, -shadInfo.normal, glm::dot(L, -shadInfo.normal), ratio);
			if (pT != NULL) {
				Ist += tr * clight->Is * pow(glm::dot(*pT, V), n);
			}

			Idt += tr * clight->Id * glm::dot(-L, shadInfo.normal);
		}

		clight = shadInfo.pWorld->lights.Next();
	}

	color = Ka * (shadInfo.pWorld->Iga * Ia) + Kd * Id + Ks * Is + Kdt * Idt + Kst * Ist + Ie;

	if (shadInfo.depth < shadInfo.pWorld->maxDepth)
	{
		if (Kr.x > 0. || Kr.y > 0. || Kr.z > 0.) // El objeto es especular
		{
			shadInfo.pWorld->numReflRays++;
			glm::vec3 R = glm::normalize(2 * VdotN * shadInfo.normal - V);

			color += shadInfo.pWorld->Trace(shadInfo.point, R, shadInfo.depth + 1) * Kr;
		}

		if (isTrans) // El objeto es translucido
		{
			glm::vec3 *pSnell = refractionRay(-V, shadInfo.normal, VdotN, ratio);

			if (pSnell != NULL) {
				shadInfo.pWorld->numRefrRays++;
				color += shadInfo.pWorld->Trace(shadInfo.point, *pSnell, shadInfo.depth + 1) * Kt;
			}
		}
	}

	if (color.x > 1.) {
		color.x = 1.;
	}
	if (color.y > 1.) {
		color.y = 1.;
	}
	if (color.z > 1.) {
		color.z = 1.;
	}

	return color;
}
