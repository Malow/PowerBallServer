#include "Authenticator.h"

using namespace MaloW;

Authenticator::Authenticator()
{
	this->db = new Database();
	this->lh.Start();
	this->totAuthed = 0;
}

Authenticator::~Authenticator()
{
	this->lh.Close();
	this->lh.WaitUntillDone();

	for(int i = 0; i < this->nonAuthed.size(); i++)
		this->nonAuthed.get(i)->Close();
	for(int i = 0; i < this->nonAuthed.size(); i++)
		this->nonAuthed.get(i)->WaitUntillDone();
	while(this->nonAuthed.size() > 0)
		delete this->nonAuthed.getAndRemove(0);

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

			int arraySlot = 0;
			for(int i = 0; i < this->nonAuthed.size(); i++)
			{
				if(id == this->nonAuthed.get(i)->getClientID())
				{
					cc = this->nonAuthed.get(i);
					arraySlot = i;
					i = this->nonAuthed.size();
				}
			}
			if(!cc)
				MaloW::Debug("Network packet recieved with a client-id that doesn't exsist in my nonAuthed array");
			else
			{
				int splitpos = msg.find(" ");

				string account = msg.substr(0, splitpos);
				string password = msg.substr(splitpos + 1);


				if(this->db->VerifyUser(account, password, "accounts.txt"))
				{
					cc->sendData("AUTH SUCCESSFUL");
					Client* cl = new Client(cc, account);
					this->lh.AddClient(cl);
					this->nonAuthed.remove(arraySlot);
				}
				else
					cc->sendData("AUTH FAILED, WRONG PW OR ACC");
			}
		}
		delete ev;
	}
}
