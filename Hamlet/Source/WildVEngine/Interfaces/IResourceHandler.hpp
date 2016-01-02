#pragma once

#include <map>

namespace wv
{
	// Based on: https://code.google.com/p/gqe/

	enum LoadTime
	{
		LOAD_NOW,
		LOAD_LATER
	};

	enum DropTime
	{
		DROP_ON_ZERO,
		DROP_ON_EXIT
	};

	class IResourceHandler
	{
	public:
		IResourceHandler(const std::string& id);
		virtual ~IResourceHandler();

		const std::string& getID() const;
		virtual void dropRef(const std::string& id) = 0;
		virtual bool loadResource(const std::string& id) = 0;
		virtual bool loadAllResources() = 0;

		virtual bool isLoaded(const std::string& id) const = 0;
		virtual LoadTime getLoadTime(const std::string& id) const = 0;
		virtual DropTime getDropTime(const std::string& id) const = 0;

		virtual void setLoadTime(const std::string& id, LoadTime lt) = 0;
		virtual void setDropTime(const std::string& id, DropTime dt) = 0;
	private:
		IResourceHandler(const IResourceHandler& rhs) = delete;
		IResourceHandler& operator=(const IResourceHandler&) = delete;

		const std::string m_ID;
	};

}