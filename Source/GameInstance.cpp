#include "GameInstance.h"

using namespace MaloW;

int GameInstance::NEXTGAMEID = 0;

GameInstance::GameInstance(GameDescription* gd, LobbyHandler* lh)
{
	this->lh = lh;
	this->desc = gd;
	this->GameID = this->NEXTGAMEID;
	this->NEXTGAMEID++;


	LARGE_INTEGER li;
	if(!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency Failed!, High resolution performance counter not available?" << endl;

	PCFreq = float(li.QuadPart)/1000.0f;
	QueryPerformanceCounter(&li);
	prevTimeStamp = li.QuadPart;

	this->AliveTimer = 0;
}

GameInstance::~GameInstance()
{
	// close wait and delete.
	while(this->clients.size() > 0)
		delete this->clients.getAndRemove(0);

	if(this->desc)
		delete this->desc;
}

JOIN_GAME_ERRORS GameInstance::JoinGame(Client* cc)
{
	this->desc->CurrentPlayers++;
	// Check if full etc;
	if(this->desc->MaxPlayers < this->desc->CurrentPlayers)
	{
		this->desc->CurrentPlayers--;
		return GAME_FULL;
	}

	
	AddClientEvent* ace = new AddClientEvent(cc);
	this->PutEvent(ace);
	
	return JOIN_SUCCESS;
}

void GameInstance::Life()
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
				for(int i = 0; i < this->clients.size(); i++)
				{
					if(id == this->clients.get(i)->GetClientChannel()->getClientID())
					{
						cc = this->clients.get(i);
						clientSlot = i;
						i == this->clients.size();
					}
				}
				if(!cc)
					MaloW::Debug("Network packet recieved with a client-id that doesn't exsist in my game-clients array");

				// Decipher string here and do shit accordingly

				if(msg == "LEAVE GAME")	// check and do something
				{
					this->lh->AddClient(cc);
					cc->GetClientChannel()->sendData("RETURNING TO LOBBY");
					this->clients.remove(clientSlot);
					this->desc->CurrentPlayers--;

					if(this->desc->CurrentPlayers == 0)
					{
						DestroyGameEvent* dge = new DestroyGameEvent(this->GameID);
						this->lh->PutEvent(dge);
					}
					else if(cc->GetUserName() == this->desc->Creator)
					{
						this->clients.get(0)->GetClientChannel()->sendData("YOU ARE NOW HOST");
						this->desc->Creator = this->clients.get(0)->GetUserName();
					}
				}
				else if(msg == "START GAME")
				{
					if(cc->GetUserName() == this->desc->Creator)
						this->StartGame();
					else
						cc->GetClientChannel()->sendData("YOU CANT START THE GAME, YOU ARE NOT HOST");
				}
				else if(msg == "PING")
					cc->GetClientChannel()->sendData("PING");
				else
				{
					cc->GetClientChannel()->sendData("MESSAGE NOT RECOGNIZED: " + msg);
				}

				// Leave game, send message to lobbyhandler and give ClientChannel to it.
			}
			if(dynamic_cast<AddClientEvent*>(ev) != NULL)
			{
				Client* cl = ((AddClientEvent*)ev)->GetClient();
				this->clients.add(cl);
				ClientChannel* cc = cl->GetClientChannel();
				cc->setNotifier(this);
				cc->sendData("NOW IN GAME " + MaloW::convertNrToString(this->GameID));
			}
			delete ev;
		}
	}
}

void GameInstance::CheckAliveStatusOnClients()
{
	for(int i = 0; i < this->clients.size(); i++)
	{
		Client* cc = this->clients.get(i);
		if(!cc->aliveStatus)
		{
			this->clients.remove(i);
			SAFE_DELETE(cc);
		}
		else
		{
			cc->aliveStatus = false;
			cc->GetClientChannel()->sendData("ALIVE CHECK");
		}
	}
}

void GameInstance::StartGame()
{
	for(int i = 0; i < this->clients.size(); i++)
	{
		Client* cc = this->clients.get(i);
		cc->GetClientChannel()->sendData("STARTING GAME");
	}
	

	// Play game
}