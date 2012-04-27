#pragma once

#include "stdafx.h"

class Database
{
private:

public:
	Database(string ip, string user, string pass, string database);
	virtual ~Database();

	/* ResultSet */void ExecuteQuery(string query);

};