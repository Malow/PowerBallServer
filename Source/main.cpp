
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
#if defined(DEBUG) || defined(_DEBUG)
	myInitMemoryCheck();
#endif

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
		cout << "Non-authed clients since start: " << server->GetFailedAuthenticationsSinceStart() << endl;
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


/*
Network Interface:
All steps will repeatedly send "ALIVE CHECK" where you just answer "I AM ALIVE". This is to remove clients that has disconnected without closing the socket and fake / bugged clients.
When connecting you will get to the Authenticator.


Authenticator:
Client will get message "AUTH YOURSELF".
Client authenticates himself by sending "EMAIL PASSWORD".
Client can send "PING" where the server instantly returns the message "PING" so that you can time it client-side.


LobbyHandler:
When the client has sent correct account and password and authenticated himself he will get to the LobbyHandler.
LobbyHandler will send "NOW IN LOBBY" when auth is successfull.
Client can send "PING" where the server instantly returns the message "PING" so that you can time it client-side.
Client can send "CREATE GAME <MAXPLAYERS> <MODE>" (FOLLOWED BY SETTINGS ETC, NOT YET IMPLEMENTED), where server returns "GAME <ID> CREATED". Returns "BAD DATA FOR CREATING A GAME: " + msg if failed.
"JOIN GAME <ID>".
"GET SERVER LIST" will get packet with nr of servers and then a list of every server with info about it, ex:
	NROFSERVERS 30
	SERVER: <ID> <CURPLAYERS> <MAXPLAYERS> <MODE> <CREATOR>
	SERVER: <ID> <CURPLAYERS> <MAXPLAYERS> <MODE> <CREATOR>
	SERVER: <ID> <CURPLAYERS> <MAXPLAYERS> <MODE> <CREATOR>
	ENDOFSERVERLIST


GameInstance:
When joining or creating a game a client gets sent to GameInstance. When successfull join client will get sent "NOW IN GAME " + this->GameID.
Client can send "PING" where the server instantly returns the message "PING" so that you can time it client-side.
In gamelobby:
	"LEAVE GAME". Returns you to LobbyHandler. Sends "RETURNING TO LOBBY"
	"YOU ARE NOW HOST" if host leaves some1 gets promoted.
	"START GAME" starts the game and sends "STARTING GAME" to all clients in the game, returns "YOU CANT START THE GAME, YOU ARE NOT HOST" if ur not the host.
	"JOIN TEAM " + "BLUE" || "RED" || "NONE" joins a team. Returns "JOINED TEAM " + "BLUE" || "RED" || "NONE" + " SUCCESSFULLY"
	"PLAYER JOINED: " + id + " " + username + " " + team		when a player joins a game this gets sent. When you connect you will get this sent for every player allready in the game.
	"PLAYER LEFT: " + id	when a player leaves a game this will be sent.
	"PLAYER JOINED TEAM " + id + " " + "BLUE" || "RED" || "NONE"	when a player changes team.

Ingame:
	

*/