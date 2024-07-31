#pragma once
#include "Game/FreecamGame.h"
#include "Game/FreecamGameY3.h"
#include "Game/FreecamGameY4.h"
#include "Game/FreecamGameJE.h"
#include "Game/FreecamGameYLAD.h"
#include "Game/FreecamGameLJ.h"
#include "Game/FreecamGameLADGaiden.h"
#include "Game/FreecamGameLADIW.h"

FreecamGame* get_game_class(Game game)
{
	switch (game)
	{
	default:
		std::cout << "Unsupported game: " << getGameName(game) << std::endl;
		return new FreecamGame();
	case Game::Yakuza3:
		return new FreecamGameY3();
	case Game::Yakuza4:
		return new FreecamGameY4();
	case Game::YakuzaLikeADragon:
		return new FreecamGameYLAD();
	case Game::Judgment:
		return new FreecamGameJE();
	case Game::LostJudgment:
		return new FreecamGameLJ();
	case Game::LikeADragonGaidenTheManWhoErasedHisName:
		return new FreecamGameLADGaiden();
	case Game::LikeADragonInfiniteWealth:
		return new FreecamGameLADIW();
	}
}