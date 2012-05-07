#pragma once

#include "stdafx.h"
#include "ClientChannel.h"

class Client
{
private:
	MaloW::ClientChannel* cc;
	string username;


public:
	bool aliveStatus;
	Client(MaloW::ClientChannel* cc, string username);
	virtual ~Client();

	MaloW::ClientChannel* GetClientChannel() { return this->cc; }
	int GetClientID() { return this->cc->getClientID(); }
	string GetUserName() { return this->username; }

};

class AddClientEvent : public MaloW::ProcessEvent
{
private:
	Client* cl;
	bool selfDestroy;

public:
	AddClientEvent(Client* cl)
	{
		this->cl = cl;
		this->selfDestroy = true;
	}
	virtual ~AddClientEvent()
	{
		if(this->selfDestroy)
			delete this->cl;
		this->cl = NULL;
	}
	Client* GetClient()
	{
		this->selfDestroy = false;
		return this->cl;
	}

};