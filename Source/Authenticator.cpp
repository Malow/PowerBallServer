#include "Authenticator.h"

using namespace MaloW;

Authenticator::Authenticator()
{
	this->db = new Database("127.0.0.1", "root", "fogdebth3a", "Powerball");
	this->lh.Start();
	this->totAuthed = 0;
}

Authenticator::~Authenticator()
{
	this->lh.Close();
	this->lh.WaitUntillDone();

	if(this->db)
		delete this->db;
}

void Authenticator::NewClient(ClientChannel* cc)
{
	cc->setNotifier(this);
	cc->Start();
	this->nonAuthed.add(cc);
	cc->sendData("AUTH YOURSELF");
}

void Authenticator::Life()
{
	while(stayAlive)
	{
		ProcessEvent* ev = this->WaitEvent();
		if(dynamic_cast<NetworkPacket*>(ev) != NULL)
		{
			ClientChannel* cc = NULL;
			std::string msg = ((NetworkPacket*)ev)->getMessage();
			int id = ((NetworkPacket*)ev)->getID();

			for(int i = 0; i < this->nonAuthed.size(); i++)
			{
				if(id == this->nonAuthed.get(i)->getClientID())
				{
					cc = this->nonAuthed.get(i);
					i == this->nonAuthed.size();
				}
			}
			if(!cc)
				MaloW::Debug("Network packet recieved with a client-id that doesn't exsist in my nonAuthed array");
			else
			{
				int splitpos = msg.find(" ");

				string email = msg.substr(0, splitpos);
				string password = msg.substr(splitpos + 1);

				

				// Decipher string here and do shit accordingly

				// if(Authenticated correct) this->lh.AddClient(cc); this->nonAuthed.remove(cc);
				// else drop?{

				if(msg == "MALOW@SPRAY.SE 12345")
					cc->sendData("YOU ARE AUTHED");
				else
					cc->sendData("AUTH FAILED");
			}
		}
		delete ev;
	}
}
