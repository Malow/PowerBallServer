#ifndef CLIENTCHANNEL_H
#define CLIENTCHANNEL_H

#include <winsock2.h>
#include <iostream>
#include <process.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <string>

#include "MaloW.h"
#include "Process.h"
#include "NetworkPacket.h"

#pragma comment(lib, "WS2_32.lib")

using namespace std;

/*
Implement your own client class with this class as an object for sending / recieving data.
1. Create ClientChannel Object.
2. Use setNotifier to set which process recivied data will be sent to.
3. Call on ->Start
This class will be listening for data with its own process and it will send data with
->SendData(string).
*/

namespace MaloW
{
	class ClientChannel : public MaloW::Process
	{
	private:
		SOCKET sock;
		Process* notifier;
		string buffer;

		static long NextCID;
		long id;

		string receiveData();

	protected:
		void CloseSpecific();

	public:
		ClientChannel(SOCKET hClient);
		virtual ~ClientChannel();
		
		void sendData(string msg);

		void Life();
		
		void setNotifier(Process* notifier) { this->notifier = notifier; }

		long getClientID() { return this->id; }
	};
}


#endif