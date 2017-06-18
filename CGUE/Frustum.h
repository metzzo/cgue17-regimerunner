#pragma once
#include "glm/glm.hpp"
using namespace glm;

namespace Engine
{


	class Plane
	{
	public:

		vec3 normal, point;
		float d;


		Plane::Plane(vec3 &v1, vec3 &v2, vec3 &v3)
		{
			set3Points(v1, v2, v3);
		}
		Plane::Plane(void)
		{
			d = 0;
		}
		Plane::~Plane()
		{

		}

		void set3Points(vec3 &v1, vec3 &v2, vec3 &v3)
		{
			vec3 aux1, aux2;

			aux1 = v1 - v2;
			aux2 = v3 - v2;

			normal = normalize(cross(aux2, aux1));
			point = v2;

			d = -dot(normal, point);
		}
		void setNormalAndPoint(vec3 normal, vec3 point)
		{

			this->normal = normalize(normal);
			d = -dot(this->normal, point);
		}
		float distance(vec3 &p) const
		{
			return d + dot(normal, p);
		}
	};

	class AABox
	{

	public:

		vec3 corner;
		float x, y, z;


		AABox::AABox(vec3 corner, float x, float y, float z)
		{
			setBox(corner, x, y, z);
		}
		AABox::AABox(void)
		{
			corner.x = 0; corner.y = 0; corner.z = 0;

			x = 1.0f;
			y = 1.0f;
			z = 1.0f;
		}
		AABox::~AABox()
		{
			
		}

		void AABox::setBox(vec3 corner, float x, float y, float z)
		{
			this->corner = corner;

			if (x < 0.0) {
				x = -x;
				this->corner.x -= x;
			}
			if (y < 0.0) {
				y = -y;
				this->corner.y -= y;
			}
			if (z < 0.0) {
				z = -z;
				this->corner.z -= z;
			}
			this->x = x;
			this->y = y;
			this->z = z;

		}

		// for use in frustum computations
		vec3 AABox::getVertexP(vec3 normal) const
		{
			auto res = corner;

			if (normal.x > 0)
				res.x += x;

			if (normal.y > 0)
				res.y += y;

			if (normal.z > 0)
				res.z += z;

			return(res);
		}
		vec3 AABox::getVertexN(vec3 normal) const
		{
			auto res = corner;

			if (normal.x < 0)
				res.x += x;

			if (normal.y < 0)
				res.y += y;

			if (normal.z < 0)
				res.z += z;

			return(res);
		}


	};
}