#pragma once
#include <bot.h>
#include <botmaze.h>
#include <vector>
#include <random>
#include <ctime>

#include <NEAT/Population.h>
#include <NEAT/Genome.h>

namespace botmaze {
	/*
	 * Draws and updates Maze, player, and bots
	 */
	class Maze {
	public:

		/*
		 * Constructs maze using constants defined in botmaze file
		 */
		Maze();

		void Draw(sf::RenderWindow& window);

		void Update();

		/*
		 * Resets bots and evolves population
		 */
		void NewRound();

		/*
		 * Sets maze up and creates starting population
		 */
		void Setup(const vector<vector<BlockType>> &grid, float block_size);

		// Functions that change booleans which represent bot instructions
		void UpKeyDown() { (player_.acc) = true; }
		void UpKeyUp() { (player_.acc) = false; }
		void DownKeyDown() { (player_.dec) = true; }
		void DownKeyUp() { (player_.dec) = false; }
		void LeftKeyDown() { (player_.left) = true; }
		void LeftKeyUp() { (player_.left) = false; }
		void RightKeyDown() { (player_.right) = true; }
		void RightKeyUp() { (player_.right) = false; }

		/*
		 * Returns position of the player
		 */
		const Vector2f &GetPlayerPos() const { return player_.bot.GetPos(); }

	private:

		/*
		 * Block stores position and color of block
		 */
		struct Block {
			Vector2f top_left;
			Vector2f bottom_right;
			Color color;
			Block(const Vector2f &t_l, const Vector2f &b_r, const Color &c) : top_left(t_l), bottom_right(b_r), color(c) {}
		};

		vector<vector<BlockType>> grid_;
		vector<vector<Block>> gridPos_;
		sf::VertexArray gridArray_;

		NEAT::Population pop;
		size_t numBots_;
		size_t tick_;
		Vector2f startPos_;
		Vector2f endPos_;
		bool winner_;
		sf::VertexArray winnerPath_;

		/*
		 * Player stores the bot which the player will control and it's input booleans
		 */
		struct Player {
			Bot bot;
			bool acc;
			bool dec;
			bool left;
			bool right;
			Player(const Bot &b, bool a, bool d, bool l, bool r) : bot(b), acc(a), dec(d), left(l), right(r) {}
		};

		Player player_;
		vector<std::pair<Bot, NEAT::NeuralNetwork>> bots_;
		sf::VertexArray botPos_;

		void CheckBotCol(Bot &bot);
		void CountBots();
		size_t SetFitness();
		void SetBots();
		BlockType GetBlockType(const Vector2f &pos) const;
		vector<double> BotInputs(const Bot &bot) const;
		float BotInputDistance(const Bot &bot, float offset) const;
		std::pair<int, int> GetIdx(const Vector2f &pos) const;
		void UpdateBot(const Bot &bot, size_t bot_idx);
		float distance(const Vector2f& a, const Vector2f& b) const;
	};
} //namespace botmaze