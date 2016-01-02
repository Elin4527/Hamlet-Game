#pragma once
#include <SFML\Graphics.hpp>
#include "IHitBox.hpp"
#include "../Math/MathUtil.hpp"

namespace wv
{
	// Template calss that uses a sf::Shape to create a collision hit box for an object
	// Uses the seperate axis theorem for collisions

	template <class T>
	class THitBox: public T, public IHitBox
	{
	public:
		THitBox(const T& t, bool vis = false)
			:T(t), IHitBox(vis)
		{
			// Place the origin on the center of the hitbox
			setOrigin(getLocalBounds().width / 2.0f, getLocalBounds().height / 2.0f);
		}

		virtual ~THitBox()
		{}


		virtual bool collision(const IHitBox & rhs) const override
		{
			return sat(rhs) && rhs.sat(*this);
		}
	protected:
		virtual bool sat(const IHitBox& rhs) const override
		{
			size_t size = getPointCount();

			// Check if any projections do not overlap on a normal axis of each side of the hitbox
			// This means a line can be drawn between the two hitboxes and a collision isn't occuring
			for (int i = 0; i < size; i++)
			{
				sf::Transform t = getTransform();
				// NOTE: transforming the points are extremely important or else comparisons will be made with the hitboxes at origin untransformed

				// Normalize the vector of a side to a unit vector
				sf::Vector2f n = normalize(t.transformPoint(getPoint((i + 1) % size)) - t.transformPoint(getPoint(i)));	

				// Get the normal of that vector
				sf::Vector2f axis(n.y, -n.x);

				// Project both hitboxes on the axis
				sf::Vector2f r1 = project(axis);
				sf::Vector2f r2 = rhs.project(axis);

				// If there is no overlap there is no collision
				if (!overlap(r1, r2))
				{
					return false;
				}
			}
			return true;
		}

		virtual sf::Vector2f project(const sf::Vector2f & axis) const override
		{
			sf::Transform t = getTransform();

			// Transform each point
			float d = dot(t.transformPoint(getPoint(0)), axis);
			sf::Vector2f r(d, d);
			for (int i = 1; i < getPointCount(); i++)
			{
				// Do a dot product to project the point onto the axis
				d = dot(t.transformPoint(getPoint(i)), axis);

				// Adjust beginning and end points of projection to maximize the area
				if (r.x > d)
				{
					r.x = d;
				}
				else if (r.y < d)
				{
					r.y = d;
				}
			}
			// Return the projection
			return r;
		}
	};
}