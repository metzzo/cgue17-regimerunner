#pragma once
#include <string>

namespace Engine {
	class BaseResource
	{
	protected:
		std::string filename;
		bool initialized;
	public:
		explicit BaseResource(std::string filename);
		virtual ~BaseResource();

		void Init();
		virtual void Load() = 0;

		std::string& GetFileName();
		void SetFilename(std::string filename);
	};
}

