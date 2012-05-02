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

	for(int i = 0; i < this->games.size(); i++)
		this->games.get(i)->WaitUntillDone();

	while(this->games.size() > 0)
		delete this->games.getAndRemove(0);

	while(this->lobbyClients.size() > 0)
		delete this->lobbyClients.getAndRemove(0);
}

void LobbyHandler::AddClient(Client* cl)
{
	this->lobbyClients.add(cl);
	ClientChannel* cc = cl->GetClientChannel();
	cc->setNotifier(this);
	cc->sendData("NOW IN LOBBY");
}

void LobbyHandler::Life()
{
	while(stayAlive)
	{
		ProcessEvent* ev = this->WaitEvent();
		if(dynamic_cast<NetworkPacket*>(ev) != NULL)
		{
			Client* cc = NULL;
			std::string msg = ((NetworkPacket*)ev)->getMessage();
			int id = ((NetworkPacket*)ev)->getID();

			for(int i = 0; i < this->lobbyClients.size(); i++)
			{
				if(id == this->lobbyClients.get(i)->GetClientID())
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

			if(msg.substr(0, 11) == "CREATE GAME")
			{
				cc->GetClientChannel()->sendData("CREATING GAME");
				//GameInstance* game 
			}
			else if(msg == "PING")
				cc->GetClientChannel()->sendData("PING");
		}
		delete ev;
	}
}