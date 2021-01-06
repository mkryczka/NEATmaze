#include <bot.h>

namespace botmaze {
	Bot::Bot() : pos_(), speed_(), angle_(), traveled_(0), canGo_(true), tick(0), path_(sf::Lines, 0) {}

	Bot::Bot(const Vector2f &pos, float angle)
		: pos_(pos), speed_(0), angle_(angle), traveled_(0), canGo_(true), tick(0), path_(sf::Lines, 0) {}

	void Bot::UpdatePos(bool acc, bool dec, bool left, bool right) {
		if (canGo_) {
			if (acc) speed_ += kAccPerTick;
			if (dec) speed_ = speed_ - kDecPerTick > 0 ? speed_ - kDecPerTick : 0;
			if (left) angle_ += kDegreeChangePerTick;
			if (right) angle_ -= kDegreeChangePerTick;

			bool add_to_path = false;
			if (tick%kPathIncrement == 0) {
				path_.append(pos_);
			}

			traveled_ += speed_;
			pos_.x += speed_ * cos(angle_);
			pos_.y += -speed_ * sin(angle_);

			if (tick%kPathIncrement == 0) {
				path_.append(pos_);
			}
			tick += 1;
		}
	}

	const Vector2f &Bot::GetPos() const {
		return pos_;
	}

	float Bot::GetTraveled() const {
		return traveled_;
	}
	float Bot::GetSpeed() const {
		return speed_;
	}
	float Bot::GetAngle() const {
		return angle_;
	}
	size_t Bot::GetTick() const {
		return tick;
	}

	sf::VertexArray & Bot::GetPath() {
		return path_;
	}
} //namespace botmaze