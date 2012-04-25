#include "LobbyHandler.h"

using namespace MaloW;

LobbyHandler::LobbyHandler()
{

}

LobbyHandler::~LobbyHandler()
{
	// close wait and delete.
	for(int i = 0; i < this->games.size(); i++)
		this->games.get(i)->Close();

	for(int i = 0; i < this->lobbyClients.size(); i++)
		this->lobbyClients.get(i)->Close();

	for(int i = 0; i < this->games.size(); i++)
		this->games.get(i)->WaitUntillDone();

	for(int i = 0; i < this->lobbyClients.size(); i++)
		this->lobbyClients.get(i)->WaitUntillDone();

	while(this->games.size() > 0)
		delete this->games.getAndRemove(0);

	while(this->lobbyClients.size() > 0)
		delete this->lobbyClients.getAndRemove(0);
}

void LobbyHandler::AddClient(MaloW::ClientChannel* cc)
{
	cc->setNotifier(this);
	cc->Start();
	this->lobbyClients.add(cc);
}

void LobbyHandler::Life()
{
	while(stayAlive)
	{
		ProcessEvent* ev = this->WaitEvent();
		if(dynamic_cast<NetworkPacket*>(ev) != NULL)
		{
			ClientChannel* cc = NULL;
			std::string msg = ((NetworkPacket*)ev)->getMessage();
			int id = ((NetworkPacket*)ev)->getID();

			for(int i = 0; i < this->lobbyClients.size(); i++)
			{
				if(id == this->lobbyClients.get(i)->getID())
				{
					cc = this->lobbyClients.get(i);
					i == this->lobbyClients.size();
				}
			}
			if(!cc)
				MaloW::Debug("Network packet recieved with a client-id that doesn't exsist in my lobbyclients array");

			// Decipher string here and do shit accordingly

			// "Join game 15" lookup GameID 15 in games array and then JoinGame(cc);

			// "Create game" create a game in the games list, start it, (and send id of it back to the client?)
		}
		delete ev;
	}
}