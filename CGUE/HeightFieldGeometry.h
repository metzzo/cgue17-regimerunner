
#include "BaseGeometry.h"
#include "glm/glm.hpp"

using namespace glm;

namespace Engine {
	class TextureResource;

	class HeightFieldGeometry :
		public BaseGeometry
	{
		TextureResource* resource;
		vec3 size;
	public:
		explicit HeightFieldGeometry(TextureResource *resource, vec3 size);
		~HeightFieldGeometry();


		PxGeometry *MakeGeometry(RigidBody *rigidBody) override;
	};
}
