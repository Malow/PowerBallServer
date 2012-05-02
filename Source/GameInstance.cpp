#include "GameInstance.h"

using namespace MaloW;

int GameInstance::NEXTGAMEID = 0;

GameInstance::GameInstance(GameDescription* gd)
{
	this->desc = gd;
	this->GameID = this->NEXTGAMEID;
	this->NEXTGAMEID++;
}

GameInstance::~GameInstance()
{
	// close wait and delete.
	for(int i = 0; i < this->clients.size(); i++)
		this->clients.get(i)->Close();

	for(int i = 0; i < this->clients.size(); i++)
		this->clients.get(i)->WaitUntillDone();

	while(this->clients.size() > 0)
		delete this->clients.getAndRemove(0);

	if(this->desc)
		delete this->desc;
}

JOIN_GAME_ERRORS GameInstance::JoinGame(MaloW::ClientChannel* cc)
{
	cc->setNotifier(this);
	this->clients.add(cc);
	cc->sendData("NOW IN GAME " + MaloW::convertNrToString(this->GameID));
	return JOIN_SUCCESS;
}

void GameInstance::Life()
{
	while(stayAlive)
	{
		ProcessEvent* ev = this->WaitEvent();
		if(dynamic_cast<NetworkPacket*>(ev) != NULL)
		{
			ClientChannel* cc = NULL;
			std::string msg = ((NetworkPacket*)ev)->getMessage();
			int id = ((NetworkPacket*)ev)->getID();

			for(int i = 0; i < this->clients.size(); i++)
			{
				if(id == this->clients.get(i)->getID())
				{
					cc = this->clients.get(i);
					i == this->clients.size();
				}
			}
			if(!cc)
				MaloW::Debug("Network packet recieved with a client-id that doesn't exsist in my game-clients array");

			// Decipher string here and do shit accordingly

			if(msg == "")	// check and do something
			{

			}
			else if(msg == "PING")
				cc->sendData("PING");

			// Leave game, send message to lobbyhandler and give ClientChannel to it.
		}
		delete ev;
	}
}