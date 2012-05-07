#include "Authenticator.h"

using namespace MaloW;

Authenticator::Authenticator()
{
	this->db = new Database();
	this->lh.Start();
	this->totAuthed = 0;


	LARGE_INTEGER li;
	if(!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency Failed!, High resolution performance counter not available?" << endl;

	PCFreq = float(li.QuadPart)/1000.0f;
	QueryPerformanceCounter(&li);
	prevTimeStamp = li.QuadPart;

	this->AliveTimer = 0;
}

Authenticator::~Authenticator()
{
	this->lh.Close();
	this->lh.WaitUntillDone();

	while(this->nonAuthed.size() > 0)
		this->nonAuthed.remove(0);

	if(this->db)
		delete this->db;
}

void Authenticator::NewClient(ClientChannel* cc)
{
	cc->setNotifier(this);
	cc->Start();
	this->nonAuthed.add(NonAuthedClient(cc));
	cc->sendData("AUTH YOURSELF");
}

void Authenticator::Life()
{
	while(stayAlive)
	{
		// For timer
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		float diff = (li.QuadPart - prevTimeStamp) / PCFreq;
		prevTimeStamp = li.QuadPart;

		this->AliveTimer += diff/* * 0.001f*/;
		if(this->AliveTimer > NETWORK_ALIVE_CHECK_INTERVAL)
		{
			this->CheckAliveStatusOnClients();
			this->AliveTimer = 0;
		}



		if(ProcessEvent* ev = this->PeekEvent())
		{
			if(dynamic_cast<NetworkPacket*>(ev) != NULL)
			{
				ClientChannel* cc = NULL;
				std::string msg = ((NetworkPacket*)ev)->getMessage();
				int id = ((NetworkPacket*)ev)->getID();

				int arraySlot = 0;
				for(int i = 0; i < this->nonAuthed.size(); i++)
				{
					if(id == this->nonAuthed.get(i).cc->getClientID())
					{
						cc = this->nonAuthed.get(i).cc;
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

					if(splitpos > 0 && msg.length() > 2 && this->db->VerifyUser(account, password, "accounts.txt"))
					{
						cc->sendData("AUTH SUCCESSFUL");
						Client* cl = new Client(cc, account);
						this->lh.AddClient(cl);
						this->nonAuthed.remove(arraySlot);
					}
					else if(msg == "I AM ALIVE")
					{
						this->nonAuthed.get(arraySlot).aliveStatus = true;
					}
					else if(msg == "PING")
						cc->sendData("PING");
					else
						cc->sendData("AUTH FAILED, WRONG PW OR ACC");
				}
			}
			delete ev;
		}
		else 
			Sleep(10);
	}
}

void Authenticator::CheckAliveStatusOnClients()
{
	for(int i = 0; i < this->nonAuthed.size(); i++)
	{
		if(!this->nonAuthed.get(i).aliveStatus)
		{
			NonAuthedClient nac = this->nonAuthed.getAndRemove(i);
			MaloW::ClientChannel* cc = nac.cc;
			cc->Close();
			cc->WaitUntillDone();
			SAFE_DELETE(cc);
		}
		else
		{
			this->nonAuthed.get(i).aliveStatus = false;
			this->nonAuthed.get(i).cc->sendData("ALIVE CHECK");
		}
	}
}
