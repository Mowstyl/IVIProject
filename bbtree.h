/* bbtree.h
*
* Interaccion y Visualizacion de la Informacion.
*
* Trabajo 1c.
* Bounding Boxes y Median cut.
*
* Hernan Indibil de la Cruz Calvo.
* Escuela Superior de Ingenieria Informatica de Albacete.
*/

#ifndef _BBTREE_H_
#define _BBTREE_H_

#include "glm/vec3.hpp" // glm::vec3

#include <algorithm>
#include "limits.h"
#include "object.h"
#include "objectlist.h"


/********************/
/* BBTree class */
/********************/

class BBTree
{
	public:
		/* Constructor and destructor */
		 BBTree ();
		 BBTree (ObjectList  *objects);
		 BBTree (Object     **objects,
			     int          size,
			     int          axis);
		~BBTree ();

		/* Functions */
		void        SetLeft       (BBTree          *pTree);  // Sets the left subtree
		void        SetRight      (BBTree          *pTree);  // Sets the right subtree
		void        SetElement    (Object          *pElem); // Sets the root element
		BBTree*     GetLeft       ();       // Returns the left subtree
		BBTree*     GetRight      ();       // Returns the right subtree
		Object*     GetElement    ();      // Returns the root element
		int         GetMaxLevel   ();         // Maximum level of the tree
		ObjectList* GetObjectList ();
		/* Returns the nearest object in the direction of the ray. Returns a pointer to that object and
		the t value of the intersection point, otherwise returns NULL */
		Object*     NearestInt    (const glm::vec3 &pos,
							       const glm::vec3 &dir,
							       float           &tnear,
							       float            tmax);
		/* Returns the product of the opacities of the objects found in the direction of the ray */
		glm::vec3   GetOpacity    (const glm::vec3 &pos1,
							       const glm::vec3 &pos2,
							       const glm::vec3 &dir);

	private:
		/* Attributes */
		ObjectList *objects;
		Object     *pElement;
		BBTree     *pLeft,
			       *pRight;
		int         maxLevel;
		glm::vec3   center;		// Center coordinates
		glm::vec3   absxyz;		// Max |x|, |y| and |z| of the boundingbox

		/* Functions */
		void Sort       (Object          **objects,
			             int               size,
			             int               axis,
			             glm::vec3        *min,
			             glm::vec3        *max);
		bool Intersects (const glm::vec3  &pos,
			             const glm::vec3  &dir,
			             float             tmax);
};

#endif  // !defined _BBTREE_H_
