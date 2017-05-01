
#include "BaseShape.h"
#include "glm/glm.hpp"

using namespace glm;

namespace Engine {
	class TextureResource;

	class HeightFieldShape :
		public BaseShape
	{
		TextureResource* resource;
		vec3 size;
	public:
		explicit HeightFieldShape(TextureResource *resource, vec3 size);
		~HeightFieldShape();

		void Init() override;
	};
}
