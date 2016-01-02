#include "IResourceHandler.hpp"

namespace wv
{
	IResourceHandler::IResourceHandler(const std::string& id)
		:m_ID(id)
	{
	}

	IResourceHandler::~IResourceHandler()
	{
	}

	const std::string& IResourceHandler::getID() const
	{
		return m_ID;
	}
}