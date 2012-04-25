#pragma once

#include "stdafx.h"
#include "NetworkServer.h"
#include "GameInstance.h"
#include "LobbyHandler.h"

class Server : public MaloW::NetworkServer
{
private:
	LobbyHandler lh;
	int totClients;

public:
	Server(int port);
	virtual ~Server();
	void ClientConnected(MaloW::ClientChannel* cc);

	int GetConnectedClientsSinceStart() const { return this->totClients; }
	int GetClientsInLobby() const { return this->lh.GetLobbyClientsSize(); }
	int GetActiveGames() const { return this->lh.GetGamesSize(); }

};