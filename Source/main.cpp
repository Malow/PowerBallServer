
#include "stdafx.h"
#include "Server.h"

#include <iostream>
using namespace std;


/*
Flow:

Server listens for connections, when found it sends them to the Authenticator.
The authenticator takes for example email address and password in a packet encrypted and checks with the database. If correct info then send the client to the Lobbyhandler.
Lobbyhandler accepts events from the clients and depending on their choices they might get sent to a GameInstance.
The game instances takes care of the clients while waiting for the game to get full / started. When it does it goes Play() with the clients and stop accepting new ones.


Server is threaded, Authenticator is threaded, Lobbyhandler is threaded, every GameInstance is threaded and every Client is threaded.
*/


int main()
{
	int ListenPort = 10000;

	MaloW::ClearDebug();
	Server* server = new Server(ListenPort);
	server->Start();

	bool go = true;
	while(go)
	{
		system("CLS");
		string input = "";
		cout << "PowerBall Master server" << endl;
		cout << "Connected Clients since start: " << server->GetConnectedClientsSinceStart() << endl;
		cout << "Failed client-authentications since start: " << server->GetFailedAuthenticationsSinceStart() << endl;
		cout << "Clients currently in lobby: " << server->GetClientsInLobby() << endl;
		cout << "Current active games (playing or waiting to get started): " << server->GetActiveGames() << endl;

		cout << endl;
		getline(cin, input);

		if(input == "exit" || input == "Exit" || input == "quit" || input == "Quit")
			go = false;
	}
	delete server;
	return 0;
};