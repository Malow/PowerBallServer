#pragma once

#include "stdafx.h"
#include "NetworkServer.h"
#include "GameInstance.h"
#include "Authenticator.h"

class Server : public MaloW::NetworkServer
{
private:
	Authenticator auth;
	int totClients;

public:
	Server(int port);
	virtual ~Server();
	void ClientConnected(MaloW::ClientChannel* cc);

	// Statistics
	int GetConnectedClientsSinceStart() const { return this->totClients; }
	int GetFailedAuthenticationsSinceStart() { return this->totClients - this->auth.GetAuthenticatedClientsSinceStart(); }
	int GetClientsInLobby() const { return this->auth.GetClientsInLobby(); }
	int GetActiveGames() const { return this->auth.GetActiveGames(); }
};