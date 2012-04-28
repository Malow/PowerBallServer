#pragma once

#include "stdafx.h"
#include "Process.h"
#include "GameInstance.h"
#include "ClientChannel.h"
#include "Client.h"

class LobbyHandler : public MaloW::Process
{
private:
	MaloW::Array<GameInstance*> games;
	MaloW::Array<Client*> lobbyClients;

public:
	LobbyHandler();
	virtual ~LobbyHandler();

	void AddClient(Client* cl);

	void Life();




	// Statistics
	int GetLobbyClientsSize() const { return this->lobbyClients.size(); }
	int GetGamesSize() const { return this->games.size(); }
};