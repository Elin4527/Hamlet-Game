#include "ResourceManager.hpp"

namespace wv
{

	ResourceManager::ResourceManager()
	{}

	ResourceManager::~ResourceManager()
	{}



	IResourceHandler& ResourceManager::getHandler(const std::string& id)
	{
		IResourceHandler* res = nullptr;
		auto iter = m_handlers.find(id);

		if (iter != m_handlers.end())
		{
			res = iter->second.get();
		}
		if (!res)
		{
			std::cout << "ResourceManager::getHandler("
				<< id << " not found!" << std::endl;
		}
		return *res;
	}

	void ResourceManager::registerHandler(IResourceHandler* handler)
	{
		if (handler)
		{
			auto iter = m_handlers.find(handler->getID());
			if (iter == m_handlers.end())
			{
				m_handlers.insert(std::pair<const std::string, std::unique_ptr<IResourceHandler>>(
					handler->getID(), std::unique_ptr<IResourceHandler>(handler)));
			}
			else
			{
				std::cout << "AssetManager::registerHandler(" << handler->getID()
					<< ") Handler already register for this ID!" << std::endl;
			}
		}
		else
		{
			std::cout << "AssetManager::registerHandler() Handler pointer provided was nullptr"
				<< std::endl;
		}
	}

	bool ResourceManager::loadAllResources()
	{
		bool res = true;
		for (auto& p : m_handlers)
		{
			res &= p.second->loadAllResources();
		}
		return res;
	}

}