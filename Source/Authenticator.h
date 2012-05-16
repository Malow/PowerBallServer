#pragma once

#include "LobbyHandler.h"
#include "Database\Database.h"
#include "Client.h"

struct NonAuthedClient
{
	MaloW::ClientChannel* cc;
	bool aliveStatus;

	NonAuthedClient()
	{
		this->cc = NULL;
		this->aliveStatus = true;
	}

	NonAuthedClient(MaloW::ClientChannel* cc)
	{
		this->cc = cc;
		this->aliveStatus = true;
	}
	/*
	virtual ~NonAuthedClient()
	{
		if(this->cc)
		{
			cc->Close();
			cc->WaitUntillDone();
			SAFE_DELETE(cc);
		}
	}
	*/
};

class Authenticator : public MaloW::Process
{
private:
	int totAuthed;
	LobbyHandler lh;
	MaloW::Array<NonAuthedClient> nonAuthed;
	Database* db;
	
	// for timer
	float PCFreq;
	__int64 prevTimeStamp;
	float AliveTimer;


public:
	void CheckAliveStatusOnClients();
	Authenticator();
	virtual ~Authenticator();

	void NewClient(MaloW::ClientChannel* cc);

	void Life();

	// Statistics
	int GetAuthenticatedClientsSinceStart() const { return this->totAuthed; }
	int GetClientsInLobby() const { return this->lh.GetLobbyClientsSize(); }
	int GetActiveGames() const { return this->lh.GetGamesSize(); }
};