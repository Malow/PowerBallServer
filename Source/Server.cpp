#include "Server.h"

Server::Server(int port) : MaloW::NetworkServer(port)
{
	this->auth.Start();
	this->totClients = 0;
}

Server::~Server()
{
	this->auth.Close();
	this->auth.WaitUntillDone();
}

void Server::ClientConnected(MaloW::ClientChannel* cc)
{
	this->totClients++;
	this->auth.NewClient(cc);
}
