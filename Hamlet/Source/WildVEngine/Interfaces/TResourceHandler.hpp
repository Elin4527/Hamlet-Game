#pragma once

#include "IResourceHandler.hpp"
#include <string>
#include <memory>
#include <map>
#include <iostream>

namespace wv
{
	// Based on: https://code.google.com/p/gqe/


	template<class T>
	class TResourceHandler: public IResourceHandler
	{
		struct ResourceData
		{
			std::unique_ptr<T> resource;
			std::string name;
			LoadTime loadTime;
			DropTime dropTime;
			unsigned int ref;
			bool loaded;
		};

		std::map<const std::string, ResourceData> m_resources;
		T m_dummyResource;
	public:
		TResourceHandler()
			:IResourceHandler(typeid(T).name())
		{}

		virtual ~TResourceHandler()
		{
			for (auto& r : m_resources)
			{
				if (r.second.ref != 0)
				{
					std::cout << "AssetHandler count for " << getID() <<
						" non zero resource reference for " << r.second.name << " count "
						<< r.second.ref << std::endl;
				}
			}
		}

		T* getRef()
		{
			return &m_dummyResource;
		}

		T* getRef(const std::string& id, LoadTime lt = LOAD_LATER,
			DropTime dt = DROP_ON_ZERO)
		{
			auto iter = m_resources.find(id);
			if (iter != m_resources.end())
			{
				if (!iter->second.resource.get())
				{
					iter->second.resource = std::make_unique<T>();
				}
				iter->second.ref++;
				return iter->second.resource.get();
			}
			else
			{
				ResourceData& r = m_resources[id];
				r.resource = std::make_unique<T>();
				r.name = id;
				r.loadTime = lt;
				r.dropTime = dt;
				r.ref = 1;

				if (r.loadTime == LOAD_NOW)
				{
					loadResource(id);
				}
				return r.resource.get();
			}
		}

		virtual void dropRef(const std::string& id) override
		{
			auto iter = m_resources.find(id);
			if (iter != m_resources.end())
			{
				DropTime d = iter->second.dropTime;
				iter->second.ref--;

				if (d == DROP_ON_ZERO && iter->second.ref == 0)
				{
					unloadResource(iter->second);
				}
			}
		}


		virtual bool isLoaded(const std::string& id) const override
		{
			auto iter = m_resources.find(id);
			if (iter != m_resources.end())
			{
				return iter->second.loaded;
			}

			std::cout << "TResourceHandler::isLoaded(" << id << ") id not found" << std::endl;
			return false;
		}

		virtual LoadTime getLoadTime(const std::string& id) const override
		{
			auto iter = m_resources.find(id);
			if (iter != m_resources.end())
			{
				return iter->second.loadTime;
			}

			std::cout << "TResourceHandler::getLoadTime(" << id << ") id not found" << std::endl;
			return LOAD_LATER;
		}

		virtual DropTime getDropTime(const std::string& id) const override
		{
			auto iter = m_resources.find(id);
			if (iter != m_resources.end())
			{
				return iter->second.dropTime;
			}

			std::cout << "TResourceHandler::getDropTime(" << id << ") id not found" << std::endl;
			return DROP_ON_ZERO;
		}

		virtual void setLoadTime(const std::string& id, LoadTime lt) override
		{
			auto iter = m_resources.find(id);
			if (iter != m_resources.end())
			{
				iter->second.loadTime = lt;
			}
			else
			{
				std::cout << "TResourceHandler::setLoadTime(" << id << ") id not found" << std::endl;

			}
		}

		virtual void setDropTime(const std::string& id, DropTime lt) override
		{
			auto iter = m_resources.find(id);
			if (iter != m_resources.end())
			{
				iter->second.dropTime = lt;
			}
			else
			{
				std::cout << "TResourceHandler::setDropTime(" << id << ") id not found" << std::endl;

			}
		}

		virtual bool loadResource(const std::string& id) override
		{
			auto iter = m_resources.find(id);
			if (iter != m_resources.end())
			{
				loadResource(iter->second);
				return iter->second.loaded;
			}
			else
			{
				std::cout << "TResourceHandler::loadResource(" << id << ") id not found" << std::endl;
			}
			return false;
		}

		virtual bool loadAllResources() override
		{
			bool ret = true;
			for (auto& r : m_resources)
			{
				loadResource(r.second);
				ret &= r.second.loaded;
			}
			return ret;
		}

	private:
		bool loadResource(ResourceData& r)
		{
			if (!r.loaded)
			{
				// Only meant for sf::Texture and sf::Font since these use the function loadFromFile
				if (!r.resource->loadFromFile(r.name))
				{
					std::cout << "Loading of resource " << r.name << " failed." << std::endl;
				}
				else
				{
					r.loaded = true;
				}
			}
			return r.loaded;
		}

		void unloadResource(ResourceData& r)
		{
			r.resource.reset();
			r.loaded = false;
		}
	};

}