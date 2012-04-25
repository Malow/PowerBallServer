#include "Server.h"

Server::Server(int port) : MaloW::NetworkServer(port)
{
	this->lh.Start();
	this->totClients = 0;
}

Server::~Server()
{
	this->lh.Close();
	this->lh.WaitUntillDone();
}

void Server::ClientConnected(MaloW::ClientChannel* cc)
{
	this->lh.AddClient(cc);
	this->totClients++;
}
