#pragma once

#include "stdafx.h"
#include "Process.h"
#include "LobbyHandler.h"
#include "ClientChannel.h"
#include "Client.h"

class LobbyHandler;

enum JOIN_GAME_ERRORS
{
	JOIN_SUCCESS = 0,
	GAME_FULL = 1
};

enum GAME_MODE
{
	DEATHMATCH = 0,
	CTF = 1,
	KOTH = 2
};

struct GameDescription
{
	GAME_MODE mode;
	string Creator;
	int MaxPlayers;
	int CurrentPlayers;

	GameDescription()
	{
		this->mode = DEATHMATCH;
		this->Creator = "";
		this->MaxPlayers = 0;
		this->CurrentPlayers = 0;
	}
};

class GameInstance : public MaloW::Process
{
private:
	GameDescription* desc;
	int GameID;
	static int NEXTGAMEID;
	MaloW::Array<Client*> clients;
	LobbyHandler* lh;

	void CheckAliveStatusOnClients();
	void StartGame();

	// for timer
	float PCFreq;
	__int64 prevTimeStamp;
	float AliveTimer;


public:
	GameInstance(GameDescription* gd, LobbyHandler* lh);
	virtual ~GameInstance();

	JOIN_GAME_ERRORS JoinGame(Client* cc);

	void Life();
	GameDescription* GetGameDescription() { return this->desc; }
	int GetGameID() { return this->GameID; }

};