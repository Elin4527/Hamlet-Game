#pragma once

#include "TResourceHandler.hpp"
#include "IApp.hpp"

namespace wv
{
	template <class T>
	class TResource
	{
		TResourceHandler<T>& m_handler;
		T* m_resource;
		std::string m_ID;

	public:
		TResource(const std::string& id, LoadTime lt = LOAD_LATER, DropTime dt = DROP_ON_ZERO)
			:m_handler(IApp::instance()->getResourceManager().getHandler<T>()), m_ID(id)
		{
			m_resource = m_handler.getRef(id, lt, dt);
		}

		TResource()
			:m_handler(IApp::instance()->getResourceManager().getHandler<T>())
		{
			m_resource = m_handler.getRef();
		}

		TResource(const TResource<T>& rhs) 
			:m_handler(rhs.m_handler), m_ID(rhs.m_ID)
		{
			if (m_ID != "")
			{
				m_resource = m_handler.getRef(m_ID);
			}
			else
			{
				m_resource = m_handler.getRef();
			}
		}

		virtual ~TResource()
		{
			if (m_ID != "")
			{
				m_handler.dropRef(m_ID);
			}
		}

		bool isLoaded() const
		{
			return m_handler.isLoaded(m_ID);
		}

		const std::string& getID() const
		{
			return m_ID;
		}

		void setID(const std::string& id, LoadTime lt = LOAD_LATER, DropTime dt = DROP_ON_ZERO)
		{
			if (m_ID != "")
			{
				m_handler.dropRef(m_ID);
			}
			m_ID = id;
			if (id != "")
			{
				m_resource = m_handler.getRef(id, lt, dt);
			}
			else
			{
				m_resource = m_handler.getRef();
			}
		}

		T& getResource()
		{
			if (!m_handler.isLoaded(m_ID))
			{
				if (!m_handler.loadResource(m_ID))
				{
					std::cout << "ERROR resource \"" << m_ID << "\" could not be loaded" << std::endl;
				}
			}
			return *m_resource;
		}

		LoadTime getLoadTime() const
		{
			return m_handler.getLoadTime(m_ID);
		}

		void setLoadTime(LoadTime lt)
		{
			m_handler.setLoadTime(m_ID, lt);
		}

		DropTime getDropTime() const
		{
			return m_handler.getDropTime(m_ID);
		}

		void setDropTime(DropTime dt)
		{
			m_handler.setDropTime(m_ID, dt);
		}

		TResource<T>& operator=(TResource<T> rhs)
		{
			swap(*this, rhs);
			return *this;
		}

		friend void swap(TResource<T>& lhs, TResource<T>& rhs)
		{
			using std::swap;
			swap(lhs.m_resource, rhs.m_resource);
			swap(lhs.m_ID, rhs.m_ID);
		}
	};
}