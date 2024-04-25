#pragma once
#include <unordered_map>
#include <unordered_set>

#include "string_utils.h"
#include "types.h"

using namespace std;

typedef unordered_set<string> stringset;
typedef std::unordered_map<std::string, std::string> stringmap;

enum class Game
{
	Unsupported = -1,

	// Old Engine
	Yakuza3,
	Yakuza4,
	Yakuza5,
	Yakuza0,
	YakuzaKiwami,

	// Dragon Engine
	Yakuza6,
	YakuzaKiwami2,
	YakuzaLikeADragon,
	Judgment,
	LostJudgment,
	VFeSports,
	LikeADragonGaidenTheManWhoErasedHisName,
	LikeADragonInfiniteWealthDemo,
	LikeADragonInfiniteWealth
};

Game getGame(string name)
{

	if (startsWith(name, "Yakuza3")) return Game::Yakuza3;
	if (startsWith(name, "Yakuza4")) return Game::Yakuza4;
	if (startsWith(name, "Yakuza5")) return Game::Yakuza5;
	if (startsWith(name, "Yakuza0")) return Game::Yakuza0;
	if (name == "YakuzaKiwami") return Game::YakuzaKiwami;
	if (startsWith(name, "Yakuza6")) return Game::Yakuza6;
	if (startsWith(name, "YakuzaKiwami2")) return Game::YakuzaKiwami2;
	if (name == "YakuzaLikeADragon") return Game::YakuzaLikeADragon;
	if (name == "eve") return Game::VFeSports;
	if (name == "Judgment") return Game::Judgment;
	if (name == "LostJudgment") return Game::LostJudgment;
	if (name == "LikeaDragonGaiden") return Game::LikeADragonGaidenTheManWhoErasedHisName;
	if (name == "LikeADragonGaiden") return Game::LikeADragonGaidenTheManWhoErasedHisName;
	if (name == "LikeADragon8") return Game::LikeADragonInfiniteWealth;

	return Game::Unsupported;
}

const char* getGameName(Game game)
{
	switch (game)
	{
	case Game::Yakuza3:
		return "Yakuza 3 Remastered";
	case Game::Yakuza4:
		return "Yakuza 4 Remastered";
	case Game::Yakuza5:
		return "Yakuza 5 Remastered";
	case Game::Yakuza0:
		return "Yakuza 0";
	case Game::YakuzaKiwami:
		return "Yakuza Kiwami";
	case Game::Yakuza6:
		return "Yakuza 6: The Song of Life";
	case Game::YakuzaKiwami2:
		return "Yakuza Kiwami 2";
	case Game::YakuzaLikeADragon:
		return "Yakuza: Like a Dragon";
	case Game::Judgment:
		return "Judgment";
	case Game::LostJudgment:
		return "Lost Judgment";
	case Game::VFeSports:
		return "Virtua Fighter eSports";
	case Game::LikeADragonGaidenTheManWhoErasedHisName:
		return "Like a Dragon Gaiden: The Man Who Erased His Name";
	case Game::LikeADragonInfiniteWealthDemo:
		return "Like a Dragon: Infinite Wealth";
	case Game::LikeADragonInfiniteWealth:
		return "Like a Dragon: Infinite Wealth";
	case Game::Unsupported:
	default:
		return "Unsupported";
	}
}

enum class Locale
{
	English,
	Japanese,
	Chinese,
	Korean
};