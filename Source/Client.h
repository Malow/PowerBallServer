#pragma once

#include "stdafx.h"
#include "ClientChannel.h"

class Client
{
private:
	MaloW::ClientChannel* cc;
	string username;


public:
	Client(MaloW::ClientChannel* cc, string username);
	virtual ~Client();

	MaloW::ClientChannel* GetClientChannel() { return this->cc; }
	int GetClientID() { return this->cc->getClientID(); }

};