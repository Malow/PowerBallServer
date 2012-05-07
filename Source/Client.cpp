#include "Client.h"

Client::Client(MaloW::ClientChannel* cc, string username)
{
	this->cc = cc;
	this->username = username;
	this->aliveStatus = true;
}

Client::~Client()
{
	if(this->cc)
	{
		this->cc->Close();
		this->cc->WaitUntillDone();
		delete this->cc;
		this->cc = NULL;
	}
}