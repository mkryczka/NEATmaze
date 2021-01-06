#pragma once
#include <botmaze.h>

namespace botmaze {
	/*
	 * Bot which travels around the maze
	 */
	class Bot {
	public:

		Bot();

		/*
		 * Constructs bot with initial position, and initial angle
		 */
		Bot(const Vector2f &pos, float angle);

		/*
		 * Updates position of the bot with fours inputs: acceleration, deceleration, turn left, and turn right
		 */

		void UpdatePos(bool acc, bool dec, bool left, bool right);

		const Vector2f &GetPos() const;

		float GetTraveled() const;

		float GetSpeed() const;

		float GetAngle() const;

		size_t GetTick() const;

		sf::VertexArray &GetPath();

		/*
		 * Stops the bot from moving when called
		 */
		void TurnOff() { canGo_ = false; }

		/*
		 * Checks if bot can move
		 */
		bool CheckEngine() { return canGo_; }

	private:
		Vector2f pos_;
		bool canGo_;
		float speed_;
		float angle_;
		float traveled_;
		size_t tick;
		sf::VertexArray path_;
	};
}//namespace botmaze