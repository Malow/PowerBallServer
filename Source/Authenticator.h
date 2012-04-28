#pragma once

#include "LobbyHandler.h"
#include "Database\Database.h"
#include "Client.h"

class Authenticator : public MaloW::Process
{
private:
	int totAuthed;
	LobbyHandler lh;
	MaloW::Array<MaloW::ClientChannel*> nonAuthed;
	Database* db;


public:
	Authenticator();
	virtual ~Authenticator();

	void NewClient(MaloW::ClientChannel* cc);

	void Life();

	// Statistics
	int GetAuthenticatedClientsSinceStart() const { return this->totAuthed; }
	int GetClientsInLobby() const { return this->lh.GetLobbyClientsSize(); }
	int GetActiveGames() const { return this->lh.GetGamesSize(); }
};