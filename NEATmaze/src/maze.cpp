#include <maze.h>
namespace botmaze {
	Maze::Maze()
		: grid_(),
		gridArray_(),
		player_(Bot(), false, false, false, false),
		pop(),
		bots_(kNumBots),
		numBots_(kNumBots),
		tick_(0),
		startPos_(),
		endPos_(),
		winner_(),
		botPos_(),
		winnerPath_(sf::Lines, 0) {
		Setup(kGrid, kBlockSize);
	}

	void Maze::Setup(const vector<vector<BlockType>> &grid, float block_size) {
		NEAT::Parameters param;
		param.PopulationSize = kNumBots;
		param.OverallMutationRate = kMutationRate;
		param.MinSpecies = kMinSpecies;
		param.MaxSpecies = kMaxSpecies;

		int seed = 0;

		auto genome = NEAT::Genome(0,
			kNumInputs,
			kNumHidden,
			kNumOutputs,
			false,
			NEAT::UNSIGNED_SIGMOID,
			NEAT::SIGNED_SIGMOID,
			seed,
			param,
			0,
			kNEATLinks);
		pop = NEAT::Population(genome, param, true, 1.0, 0);

		grid_ = grid;
		Vector2f top_left_pos;
		Vector2f bot_right_pos;
		Vector2f bot_left_pos;
		Vector2f top_right_pos;

		Color block_color;
		vector<Block> column_pos;
		gridArray_.setPrimitiveType(sf::Quads);
		gridArray_.resize((grid_.size() * grid_.front().size()) * 4);
		botPos_.setPrimitiveType(sf::Triangles);

		size_t count = 0;
		for (size_t x = 0; x < grid.size(); x++) {
			for (size_t y = 0; y < grid[x].size(); y++) {
				top_left_pos = { kMazeTopLeft.x + ((float)x * block_size), kMazeTopLeft.y + ((float)y * block_size) };
				bot_left_pos = { top_left_pos.x, top_left_pos.y + block_size };
				bot_right_pos = { top_left_pos.x + block_size, bot_left_pos.y };
				top_right_pos = { bot_right_pos.x, top_left_pos.y };

				gridArray_[count].position = top_left_pos;
				gridArray_[count + 1].position = bot_left_pos;
				gridArray_[count + 2].position = bot_right_pos;
				gridArray_[count + 3].position = top_right_pos;

				block_color = kBlockColor(grid_[x][y]);
				gridArray_[count].color = block_color;
				gridArray_[count + 1].color = block_color;
				gridArray_[count + 2].color = block_color;
				gridArray_[count + 3].color = block_color;

				count += 4;

				if (grid_[x][y] == kEnd) endPos_ = { top_left_pos.x + block_size / 2.0f, bot_right_pos.y - block_size / 2.0f };
			}
		}
		startPos_ = { kMazeTopLeft.x + kBlockSize / 2.0f, kMazeTopLeft.y + kBlockSize / 2.0f };
		SetBots();
	}

	void Maze::Draw(sf::RenderWindow& window) {
		window.clear();
		window.draw(gridArray_);

		for (auto& bot : bots_) {
			window.draw(bot.first.GetPath());
		}
		window.draw(player_.bot.GetPath());
		window.draw(winnerPath_);

		window.draw(botPos_);
		window.display();
	}

	void Maze::Update() {
		for (size_t bot_idx = 0; bot_idx < numBots_; bot_idx++) {
			auto &cur_bot = bots_[bot_idx].first;
			auto &cur_net = bots_[bot_idx].second;
			CheckBotCol(cur_bot);

			if (cur_bot.CheckEngine()) {
				auto inputs = BotInputs(cur_bot);
				cur_net.Input(inputs);
				cur_net.Activate();

				auto out_vec = cur_net.Output();
				bool acc = out_vec[0] > 0;
				bool dec = out_vec[1] > 0;
				bool left = out_vec[2] > 0;
				bool right = out_vec[3] > 0;

				cur_bot.UpdatePos(acc, dec, left, right);

				UpdateBot(cur_bot, (bot_idx) * 3);
			}
		}
		player_.bot.UpdatePos(player_.acc, player_.dec, player_.left, player_.right);
		CheckBotCol(player_.bot);
		UpdateBot(player_.bot, bots_.size() * 3);

		tick_++;
	}

	void Maze::NewRound() {
		SetFitness();
		pop.Epoch();

		CountBots();

		SetBots();
		tick_ = 0;
		winnerPath_.clear();
	}

	void Maze::CheckBotCol(Bot &bot) {
		auto type = GetBlockType(bot.GetPos());
		if (type == kFilled || type == kEnd) {
			bot.TurnOff();
			if (type == kEnd) {
				if (!winner_) {
					winnerPath_ = bot.GetPath();
					for (size_t i = 0; i < winnerPath_.getVertexCount(); i++) {
						winnerPath_[i].color = kBlockColor(kEnd);
					}
					winner_ = true;
				}
			}
		}
	}

	void Maze::CountBots() {
		size_t count = 0;
		for (auto &specie : pop.m_Species) {
			count += specie.NumIndividuals();
		}
		numBots_ = count;
	}

	size_t Maze::SetFitness() {
		size_t bot_idx = 0;
		for (auto &specie : pop.m_Species) {
			for (auto &genome : specie.m_Individuals) {
				auto &cur_bot = bots_[bot_idx].first;
				double fitness;
				if (GetBlockType(cur_bot.GetPos()) != kEnd) {
					fitness = kDistanceWeight / distance(cur_bot.GetPos(), endPos_) + cur_bot.GetTraveled() / kTraveledWeight;
				}
				else {
					fitness = kWinWeight / cur_bot.GetTick();
				}
				genome.SetFitness(fitness);
				bot_idx++;
			}
		}
		return bot_idx;
	}

	void Maze::SetBots() {
		winnerPath_.clear();
		size_t num_verticies = (numBots_) * 3;
		botPos_.resize(num_verticies + 3);
		Vector2f start(0.0f, 0.0f);

		for (size_t i = 0; i < num_verticies; i += 3) {
			botPos_[i].color = kBotColorTip;
			botPos_[i + 1].color = kBotColorWing;
			botPos_[i + 2].color = kBotColorWing;
		}

		botPos_[num_verticies].color = kPlayerColorTip;
		botPos_[num_verticies + 1].color = kPlayerColorWing;
		botPos_[num_verticies + 2].color = kPlayerColorWing;

		winner_ = false;
		player_.bot = Bot(startPos_, kStartAngle);
		size_t bot_idx = 0;
		const static Bot start_bot = Bot(startPos_, kStartAngle);
		for (auto &specie : pop.m_Species) {
			for (auto &genome : specie.m_Individuals) {
				bots_[bot_idx].first = start_bot;
				bots_[bot_idx].second = NEAT::NeuralNetwork();
				genome.BuildPhenotype(bots_[bot_idx].second);
				bot_idx++;
			}
		}
	}

	BlockType Maze::GetBlockType(const Vector2f &pos) const {
		auto idxs = GetIdx(pos);
		if (idxs.first < 0 || idxs.second < 0 || idxs.first >= kNumXBlocks || idxs.second >= kNumYBlocks) {
			return kFilled;
		}
		return grid_[idxs.first][idxs.second];
	}

	vector<double> Maze::BotInputs(const Bot &bot) const {
		vector<double> inputs(kNumInputs);

		const static float quad_pi = kPI / 4;
		inputs[0] = BotInputDistance(bot, 0.0);
		inputs[1] = BotInputDistance(bot, quad_pi);
		inputs[2] = BotInputDistance(bot, -quad_pi);
		inputs[3] = kBias;

		return inputs;
	}

	std::pair<int, int> Maze::GetIdx(const Vector2f &pos) const {
		return std::make_pair(static_cast<int>(floor((pos.x - kMazeTopLeft.x) / kBlockSize)),
			static_cast<int>(floor((pos.y - kMazeTopLeft.y) / kBlockSize)));
	}

	float Maze::BotInputDistance(const Bot &bot, float offset) const {
		auto &pos = bot.GetPos();
		auto iter_pos = pos;
		BlockType iter_block = kOpen;
		float angle = bot.GetAngle() + offset;
		Vector2f step_dir = { cos(angle) * kSearchStep, -sin(angle) * kSearchStep };
		while (iter_block != kFilled) {
			iter_pos += step_dir;
			iter_block = GetBlockType(iter_pos);
		}
		return distance(iter_pos, pos);
	}

	void Maze::UpdateBot(const Bot &bot, size_t bot_idx) {
		const static float hpi = kPI * 0.75;
		auto &pos = bot.GetPos();
		auto angle = bot.GetAngle();

		botPos_[bot_idx].position.x = pos.x + cos(angle) * kBotRadius;
		botPos_[bot_idx].position.y = pos.y - sin(angle) * kBotRadius;
		botPos_[bot_idx + 1].position.x = pos.x + cos(angle + hpi) * kBotRadius;
		botPos_[bot_idx + 1].position.y = pos.y - sin(angle + hpi) * kBotRadius;
		botPos_[bot_idx + 2].position.x = pos.x + cos(angle - hpi) * kBotRadius;
		botPos_[bot_idx + 2].position.y = pos.y - sin(angle - hpi) * kBotRadius;
	}

	float Maze::distance(const Vector2f & a, const Vector2f & b) const
	{
		return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2));
	}
} //namespace botmaze