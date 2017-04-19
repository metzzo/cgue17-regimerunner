#pragma once
#include <string>

namespace Engine {
	class BaseResource
	{
		bool initialized;
	protected:
		std::string filename;
	public:
		explicit BaseResource(std::string filename);
		virtual ~BaseResource();

		void Init();
		virtual void Load() = 0;

		std::string& GetFileName();
	};
}

