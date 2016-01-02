#pragma once

#include "../Interfaces/TResourceHandler.hpp"
#include <iostream>

namespace wv
{

	// Manages resources used in the game (specifically sf::Texture and sf::Font)
	// Based on: https://code.google.com/p/gqe/
	class ResourceManager
	{
		std::map<const std::string, std::unique_ptr<IResourceHandler>> m_handlers;
	public:
		ResourceManager();
		virtual ~ResourceManager();

		template <class T>
		TResourceHandler<T>& getHandler()	// Return a resoure handler for the given Resource Type
		{
			TResourceHandler<T>* res = nullptr;
			auto iter = m_handlers.find(typeid(T).name());

			if (iter != m_handlers.end())
			{
				res = dynamic_cast<TResourceHandler<T>*>(iter->second.get());
			}
			if (!res)
			{
				std::cout << "ResourceManager::getHandler("
					<< typeid(T).name() << " not found!" << std::endl;
			}
			return *res;
		}
		IResourceHandler& getHandler(const std::string& handlerID);

		void registerHandler(IResourceHandler* handlerID);
		bool loadAllResources();
	private:
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
	};
}