#pragma once
#include <SFML/Graphics.hpp>

/*
 * Maze with controllable player and bots
 */
namespace botmaze {
	using std::vector;
	using sf::Vector2f;
	using std::tuple;
	using std::pair;
	using sf::Color;

	static const int kWindowWidth = 900;
	static const int kWindowHeight = 900;
	static const Color kBackgroundColor = sf::Color::Black;

	static const Color kPlayerColorTip = Color(51, 255, 255, 255);
	static const Color kPlayerColorWing = Color(102, 255, 178, 220);
	static const Color kBotColorTip = Color(225, 0, 0, 200);
	static const Color kBotColorWing = Color(255, 102, 204, 150);
	static const float kStartAngle = 0.0f;
	static const float kBotRadius = 15.0f;
	static const size_t kPathIncrement = 3;

	static const float kAccPerTick = 0.03f;
	static const float kDecPerTick = 0.10f;
	static const float kDegreeChangePerTick = 0.05f;

	static const float kBlockSize = 160.0f;
	static const Vector2f kMazeTopLeft = { 50, 50 };
	enum BlockType { kFilled = 0, kOpen = 1, kStart = 2, kEnd = 3 };

	static size_t kNumBots = 20;

	static Color kBlockColor(BlockType type) {
		switch (type) {
		case kFilled:return Color(0, 0, 0, 255);
		case kOpen:return Color(112, 128, 144, 255);
		case kStart:return Color(0, 0, 225, 255);
		case kEnd:return Color(0, 255, 0, 255);
		}
		return Color::White;
	}

	//NEAT constants
	const static double kMutationRate = 0.30;
	const static size_t kMinSpecies = 2;
	const static size_t kMaxSpecies = 5;
	const static size_t kNumInputs = 4;
	const static size_t kNumHidden = 3;
	const static size_t kNumOutputs = 4;
	const static size_t kNEATLinks = 3;
	const static double kBias = 1.0;
	const static float kSearchStep = kBlockSize / 10.0f;
	const static float kPI = 3.1415926535f;
	//input weights
	const static float kDistanceWeight = 100.0f;
	const static float kTraveledWeight = 5.0f;
	const static float kWinWeight = 100000000.0f;

	static const vector<vector<int>> kIntGrid = {
		{2, 1, 1, 1, 0},
		{0, 0, 0, 1, 0},
		{0, 1, 1, 1, 0},
		{0, 1, 0, 0, 0},
		{0, 1, 1, 1, 3}
	};

	/*
	 * Converts manually input integer grid to enums
	 */
	static vector<vector<BlockType>> kGridIntToBlock(const vector<vector<int>> &int_grid) {
		vector<vector<BlockType>> grid(int_grid[0].size(), vector<BlockType>(int_grid.size(), kFilled));

		for (size_t x = 0; x < int_grid.size(); x++) {
			for (size_t y = 0; y < int_grid[x].size(); y++) {
				grid[x][y] = (static_cast<BlockType>(int_grid[y][x]));
			}
		}
		return grid;
	}

	static const vector<vector<BlockType>> kGrid = kGridIntToBlock(kIntGrid);
	static const int kNumXBlocks = kGrid.size();
	static const int kNumYBlocks = kGrid.front().size();
} // namespace botmaze
