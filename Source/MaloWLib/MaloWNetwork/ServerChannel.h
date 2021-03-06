#ifndef SERVERCHANNEL_H
#define SERVERCHANNEL_H

#include <winsock2.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <windows.h>

#include "MaloW.h"
#include "Process.h"
#include "NetworkPacket.h"
#include "MaloWFileDebug.h"

#pragma comment(lib, "WS2_32.lib")

using namespace std;

/*
Implement your own client class with this class as an object for sending / recieving data.
1. Create ServerChannel Object.
2. Use setNotifier to set which process recivied data will be sent to.
3. Call on ->Start
This class will create a channel to a server that has a process that will read data from the
server and you can also send data to the server.s
*/

namespace MaloW
{
	class ServerChannel : public MaloW::Process
	{
	private:
		SOCKET sock;
		Process* notifier;
		string buffer;
		string receiveData();
		string unImportantFilter;

	protected:
		void CloseSpecific();

	public:
		ServerChannel(string IP, int port);
		virtual ~ServerChannel();
		void sendData(string msg);

		void setNotifier(Process* notifier) { this->notifier = notifier; }
		void Life();
		void setUnImportantFilter(string filter) { this->unImportantFilter = filter; }
		
	};
}

#endif