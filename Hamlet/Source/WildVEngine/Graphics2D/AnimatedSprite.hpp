#pragma once

#include "Animation.hpp"
#include <SFML\Graphics.hpp>
#include <memory>
#include "DynamicGraphic.hpp"

namespace wv
{
	// Sprite that uses an animation to update the Graphic to different sections of a spritesheet
	// Based on this: https://github.com/SFML/SFML/wiki/Source%3A-AnimatedSprite

	class AnimatedSprite : public DynamicGraphic
	{
		std::shared_ptr<Animation> m_animation;
		sf::Time m_frameTime;
		sf::Time m_currentTime;
		std::size_t m_currentFrame;
		bool m_isPaused;
		bool m_isLooped;
		const sf::Texture* m_texture;
		sf::Vertex m_vertices[4];

	public:
		explicit AnimatedSprite(sf::Time frameTime = sf::seconds(1.0f / 24.0f), bool paused = false, bool looped = true);

		virtual void update(sf::Time fd) override;
		virtual bool isPlaying() const override;

		void setAnimation(std::shared_ptr<Animation> animation);
		void setFrameTime(sf::Time time);
		void play();
		void play(std::shared_ptr<Animation> animation);
		void pause();
		void stop();
		void setLooped(bool looped);
		void setColor(const sf::Color& color);

		std::shared_ptr<Animation> getAnimation();
		sf::FloatRect getLocalBounds() const;
		bool isLooped() const;
		sf::Time getFrameTime() const;
		void setFrame(std::size_t newFrame, bool resetTime = true);

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	};
}