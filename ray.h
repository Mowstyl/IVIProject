#ifndef _RAY_H_
#define _RAY_H_

#include "glm/glm.hpp" // glm::vec3, glm::dot

class Ray
{
	public:
		Ray();
		Ray(glm::vec3 &pos, glm::vec3 &dir);

		glm::vec3 pos;
		glm::vec3 dir;
		glm::vec3 inv_dir;
		float innerdir;
		int sign[3];
};

inline Ray::Ray() {

}

inline Ray::Ray(glm::vec3 &pos, glm::vec3 &dir)
{
	this->pos = pos;
	this->dir = dir;
	inv_dir = glm::vec3(1.0f / dir.x, 1.0f / dir.y, 1.0f / dir.z);
	innerdir = glm::dot(dir, dir);
	sign[0] = (inv_dir.x < 0);
	sign[1] = (inv_dir.y < 0);
	sign[2] = (inv_dir.z < 0);
}

#endif  // !defined _RAY_H_