#include "LobbyHandler.h"

using namespace MaloW;

LobbyHandler::LobbyHandler()
{
	LARGE_INTEGER li;
	if(!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency Failed!, High resolution performance counter not available?" << endl;

	PCFreq = float(li.QuadPart)/1000.0f;
	QueryPerformanceCounter(&li);
	prevTimeStamp = li.QuadPart;

	this->AliveTimer = 0;
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
	AddClientEvent* ace = new AddClientEvent(cl);
	this->PutEvent(ace);
}

void LobbyHandler::JoinGameFunc(int GameID, Client* cc, int clientSlot)
{
	bool gameFound = false;
	for(int u = 0; u < this->games.size(); u++)
	{
		if(this->games.get(u)->GetGameID() == GameID)
		{
			gameFound = true;
			JOIN_GAME_ERRORS res = this->games.get(u)->JoinGame(cc);
			if(res == JOIN_SUCCESS)
			{
				this->lobbyClients.remove(clientSlot);
			}
			else
			{
				cc->GetClientChannel()->sendData("FAILED TO JOIN SERVER " + MaloW::convertNrToString(GameID) + ". " + "ERROR: " + MaloW::convertNrToString(res));
			}
			u = this->games.size();
		}
	}
	if(!gameFound)
		cc->GetClientChannel()->sendData("FAILED TO JOIN SERVER " + MaloW::convertNrToString(GameID) + ". SERVER NOT FOUND.");
}

int LobbyHandler::CreateGameFunc(string msg, Client* cc)
{
	string temp = msg.substr(12);
	int maxPl = atoi(temp.substr(0, temp.find(" ")).c_str());
	int mode = atoi(temp.substr(temp.find(" ") + 1).c_str());

	if(maxPl == 0)
		return -1;
				
	GameDescription* gd = new GameDescription();
	gd->MaxPlayers = maxPl;
	gd->Creator = cc->GetUserName();
	gd->mode = GAME_MODE(mode);
	GameInstance* game = new GameInstance(gd, this);

	cc->GetClientChannel()->sendData("GAME " + MaloW::convertNrToString(game->GetGameID()) + " CREATED"); //"GAME <ID> CREATED"
	this->games.add(game);
	game->Start();

	return game->GetGameID();
}

void LobbyHandler::GetServerListFunc(Client* cc)
{
	string msg = MaloW::convertNrToString(this->games.size());
	cc->GetClientChannel()->sendData("NROFSERVERS " + msg);	//NROFSERVERS 30
	for(int u = 0; u < this->games.size(); u++)
	{
		GameDescription* desc = this->games.get(u)->GetGameDescription();
		msg = "SERVER: ";
		msg += MaloW::convertNrToString(this->games.get(u)->GetGameID()) + " ";
		msg += MaloW::convertNrToString(desc->CurrentPlayers) + " ";
		msg += MaloW::convertNrToString(desc->MaxPlayers) + " ";
		msg += MaloW::convertNrToString(desc->mode) + " ";
		msg += desc->Creator + " ";

		cc->GetClientChannel()->sendData(msg);	//SERVER: <ID> <CURPLAYERS> <MAXPLAYERS> <MODE> <CREATOR>
	}
	cc->GetClientChannel()->sendData("ENDOFSERVERLIST");
}

void LobbyHandler::OtherEventFunc(MaloW::ProcessEvent* ev)
{
	if(dynamic_cast<AddClientEvent*>(ev) != NULL)
	{
		Client* cl = ((AddClientEvent*)ev)->GetClient();
		this->lobbyClients.add(cl);
		ClientChannel* cc = cl->GetClientChannel();
		cc->setNotifier(this);
		cc->sendData("NOW IN LOBBY");
	}
	else if(dynamic_cast<DestroyGameEvent*>(ev) != NULL)
	{
		int id = ((DestroyGameEvent*)ev)->GameID;
		for(int u = 0; u < this->games.size(); u++)
		{
			if(this->games.get(u)->GetGameID() == id)
			{
				this->games.remove(u);
				u = this->games.size();
			}
		}
	}
}

void LobbyHandler::HandleMessage(string msg, Client* cc, int clientSlot)
{
	if(msg.substr(0, 11) == "CREATE GAME")		// "CREATE GAME <MAXPLAYERS> <MODE>"
	{
		int gameid = this->CreateGameFunc(msg, cc);
		if(gameid == -1)
			cc->GetClientChannel()->sendData("BAD DATA FOR CREATING A GAME: " + msg);
		else
			this->JoinGameFunc(gameid, cc, clientSlot);
	}
	else if(msg.substr(0, 9) == "JOIN GAME")	//"JOIN GAME <ID>"
	{
		int gameId = atoi(msg.substr(10).c_str());
		this->JoinGameFunc(gameId, cc, clientSlot);
	}
	else if(msg == "GET SERVER LIST")			//"GET SERVER LIST"
	{
		this->GetServerListFunc(cc);				
	}
	else if(msg == "I AM ALIVE")
	{
		cc->aliveStatus = true;
	}
	else if(msg == "PING")
		cc->GetClientChannel()->sendData("PING");
	else
	{
		cc->GetClientChannel()->sendData("MESSAGE NOT RECOGNIZED: " + msg);
	}
}

void LobbyHandler::Life()
{
	while(stayAlive)
	{
		// For timer
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		float diff = (li.QuadPart - prevTimeStamp) / PCFreq;
		prevTimeStamp = li.QuadPart;

		this->AliveTimer += diff;
		if(this->AliveTimer > NETWORK_ALIVE_CHECK_INTERVAL)
		{
			this->CheckAliveStatusOnClients();
			this->AliveTimer = 0;
		}


		if(ProcessEvent* ev = this->PeekEvent())
		{
			if(dynamic_cast<NetworkPacket*>(ev) != NULL)
			{
				Client* cc = NULL;
				std::string msg = ((NetworkPacket*)ev)->getMessage();
				int id = ((NetworkPacket*)ev)->getID();

				int clientSlot = 0;
				for(int i = 0; i < this->lobbyClients.size(); i++)
				{
					if(id == this->lobbyClients.get(i)->GetClientID())
					{
						cc = this->lobbyClients.get(i);
						clientSlot = i;
						i == this->lobbyClients.size();
					}
				}
				if(!cc)
					MaloW::Debug("Network packet recieved with a client-id that doesn't exsist in my lobbyclients array");
				else
					this->HandleMessage(msg, cc, clientSlot);
			}
			else
				this->OtherEventFunc(ev);

			delete ev;
		}
		else 
			Sleep(10);
	}
}

void LobbyHandler::CheckAliveStatusOnClients()
{
	for(int i = 0; i < this->lobbyClients.size(); i++)
	{
		Client* cc = this->lobbyClients.get(i);
		if(!cc->aliveStatus)
		{
			this->lobbyClients.remove(i);
			SAFE_DELETE(cc);
		}
		else
		{
			cc->aliveStatus = false;
			cc->GetClientChannel()->sendData("ALIVE CHECK");
		}
	}
}