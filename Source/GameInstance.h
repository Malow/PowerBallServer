#pragma once

#include "stdafx.h"
#include "Process.h"
#include "ClientChannel.h"

enum JOIN_GAME_ERRORS
{
	JOIN_SUCCESS = 0,
	GAME_FULL = 1

};

class GameInstance : public MaloW::Process
{
private:
	int GameID;
	static int NEXTGAMEID;
	MaloW::Array<MaloW::ClientChannel*> clients;


public:
	GameInstance();
	virtual ~GameInstance();

	JOIN_GAME_ERRORS JoinGame(MaloW::ClientChannel* cc);

	void Life();

};