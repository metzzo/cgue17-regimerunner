
#include "BaseShape.h"

namespace Engine {
	class TextureResource;

	class HeightFieldShape :
		public BaseShape
	{
		TextureResource* resource;
	public:
		explicit HeightFieldShape(TextureResource *resource);
		~HeightFieldShape();

		void Init() override;
	};
}
