#pragma once

#include "stdafx.h"
#include "Process.h"
#include "GameInstance.h"
#include "ClientChannel.h"
#include "Client.h"

class GameInstance;

class DestroyGameEvent : public MaloW::ProcessEvent
{
public:
	int GameID;
	DestroyGameEvent(int gid) { this->GameID = gid; }
	virtual ~DestroyGameEvent() { }
};

class LobbyHandler : public MaloW::Process
{
private:
	MaloW::Array<GameInstance*> games;
	MaloW::Array<Client*> lobbyClients;

	void JoinGameFunc(int GameID, Client* cc, int clientSlot);
	int CreateGameFunc(string msg, Client* cc);
	void GetServerListFunc(Client* cc);
	void OtherEventFunc(MaloW::ProcessEvent* ev);
	void HandleMessage(string msg, Client* cc, int clientSlot);
	void CheckAliveStatusOnClients();

	// for timer
	float PCFreq;
	__int64 prevTimeStamp;
	float AliveTimer;

public:
	LobbyHandler();
	virtual ~LobbyHandler();

	void AddClient(Client* cl);

	void Life();

	
	

	// Statistics
	int GetLobbyClientsSize() const { return this->lobbyClients.size(); }
	int GetGamesSize() const { return this->games.size(); }
};