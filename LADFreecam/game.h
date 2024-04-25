#pragma once
#include "Game/FreecamGame.h"
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