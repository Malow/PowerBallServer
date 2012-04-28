#pragma once

#include "stdafx.h"
#include "Process.h"
#include "ClientChannel.h"

enum JOIN_GAME_ERRORS
{
	JOIN_SUCCESS = 0,
	GAME_FULL = 1

};

enum GAME_MODE
{
	KNOCKOUT = 0,
	CTF = 1,
	KOTH = 2
};

struct GameDescription
{
	GAME_MODE mode;
	string Creator;
	int MaxPlayers;
	int CurrentPlayers;
};

class GameInstance : public MaloW::Process
{
private:
	GameDescription* desc;
	int GameID;
	static int NEXTGAMEID;
	MaloW::Array<MaloW::ClientChannel*> clients;


public:
	GameInstance(GameDescription* gd);
	virtual ~GameInstance();

	JOIN_GAME_ERRORS JoinGame(MaloW::ClientChannel* cc);

	void Life();
	GameDescription* GetGameDescription() { return this->desc; }
	int GetGameID() { return this->GameID; }

};