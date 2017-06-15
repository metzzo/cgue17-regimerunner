#include "BaseResource.h"


namespace Engine {
	BaseResource::BaseResource(std::string filename)
	{
		this->filename = filename;
		this->initialized = false;
	}

	BaseResource::~BaseResource()
	{
	}

	void BaseResource::Init()
	{

		if (initialized)
		{
			return;
		}
		initialized = true;

		this->Load();
	}

	std::string& BaseResource::GetFileName()
	{
		return filename;
	}

	void BaseResource::SetFilename(std::string filename)
	{
		this->filename = filename;
	}
}
