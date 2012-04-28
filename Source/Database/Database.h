#pragma once

#include "stdafx.h"
/*
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
*/

class Database
{
private:
	/*
    SQLHANDLE sqlenvhandle;    
    SQLHANDLE sqlconnectionhandle;
    SQLHANDLE sqlstatementhandle;
	*/

public:
	Database(string ip = "localhost", int port = 3306, string user = "root", string pass = "fogdebth3a", string database = "PowerBall");
	virtual ~Database();

	MaloW::Array<MaloW::Array<string>*>* ExecuteQuery(string query);
	bool VerifyUser(string account, string pass, string txtfile);

};