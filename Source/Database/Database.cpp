#include "Database.h"

/*
string SQLCHARToString(SQLCHAR sc[])
{
	stringstream ss;
	ss << sc;
	return ss.str();
}

void StringToSQLCHAR(SQLCHAR sc[], string in)
{
	for(int i = 0; i < in.size(); i++)
		sc[i] = in[i];
}

void show_error(unsigned int handletype, const SQLHANDLE& handle)
{
    SQLCHAR sqlstate[1024];
    SQLCHAR message[1024];
    if(SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL))
		MaloW::Debug("Message: " + SQLCHARToString(message) + "\nSQLSTATE: " + SQLCHARToString(sqlstate));
        //cout<<"Message: "<<message<<"\nSQLSTATE: "<<sqlstate<<endl;
}
*/

Database::Database(string ip, int port, string user, string pass, string database)
{
	/*
    SQLRETURN retcode;
	string temp = "DRIVER={SQL Server};SERVER=" + ip + ", " + MaloW::convertNrToString(port) + ";DATABASE=" + database + ";UID=" + user + ";PWD=" + pass + ";";

    if(SQL_SUCCESS!=SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlenvhandle))
        goto FINISHED;

    if(SQL_SUCCESS!=SQLSetEnvAttr(sqlenvhandle,SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)) 
        goto FINISHED;
    
    if(SQL_SUCCESS!=SQLAllocHandle(SQL_HANDLE_DBC, sqlenvhandle, &sqlconnectionhandle))
        goto FINISHED;

    SQLCHAR retconstring[1024];
	SQLCHAR conIn[1024];
	StringToSQLCHAR(conIn, temp);

    switch(SQLDriverConnect (sqlconnectionhandle, 
                NULL, 
				//(SQLCHAR*)conIn, 
				(SQLCHAR*)"DRIVER={SQL Server};SERVER=localhost, 3306;DATABASE=PowerBall;UID=root;PWD=fogdebth3a;",
                SQL_NTS, 
                retconstring, 
                1024, 
                NULL,
                SQL_DRIVER_NOPROMPT))
	{
        case SQL_SUCCESS_WITH_INFO:
            show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
            break;
        case SQL_INVALID_HANDLE:
        case SQL_ERROR:
            show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
            goto FINISHED;
        default:
            break;
    }
    
    if(SQL_SUCCESS!=SQLAllocHandle(SQL_HANDLE_STMT, sqlconnectionhandle, &sqlstatementhandle))
        goto FINISHED;

FINISHED:
	;

	*/
}

Database::~Database()
{
	/*
	if(sqlstatementhandle)
		SQLFreeHandle(SQL_HANDLE_STMT, sqlstatementhandle);
	if(sqlconnectionhandle)
		SQLDisconnect(sqlconnectionhandle);
	if(sqlconnectionhandle)
		SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);
	if(sqlenvhandle)
		SQLFreeHandle(SQL_HANDLE_ENV, sqlenvhandle);

		*/
}

MaloW::Array<MaloW::Array<string>*>* Database::ExecuteQuery(string query)
{
	/*
	SQLCHAR sc[1024];
	StringToSQLCHAR(sc, query);

    if(SQL_SUCCESS!=SQLExecDirect(sqlstatementhandle, (SQLCHAR*)sc, SQL_NTS))
	{
        show_error(SQL_HANDLE_STMT, sqlstatementhandle);
    }
    else
	{
		//SQLDescribeCol

		
		SQLDescribeCol(
			  SQLHSTMT       StatementHandle,
			  SQLUSMALLINT   ColumnNumber,
			  SQLCHAR *      ColumnName,
			  SQLSMALLINT    BufferLength,
			  SQLSMALLINT *  NameLengthPtr,
			  SQLSMALLINT *  DataTypePtr,
			  SQLULEN *      ColumnSizePtr,
			  SQLSMALLINT *  DecimalDigitsPtr,
			  SQLSMALLINT *  NullablePtr);


		SQLRETURN SQLNumResultCols(
			 SQLHSTMT        StatementHandle,
			 SQLSMALLINT *   ColumnCountPtr);

			  






        char name[64];
        char address[64];
        int id;
        while(SQLFetch(sqlstatementhandle)==SQL_SUCCESS)
		{
            SQLGetData(sqlstatementhandle, 1, SQL_C_ULONG, &id, 0, NULL);
            SQLGetData(sqlstatementhandle, 2, SQL_C_CHAR, name, 64, NULL);
            SQLGetData(sqlstatementhandle, 3, SQL_C_CHAR, address, 64, NULL);
            cout<<id<<" "<<name<<" "<<address<<endl;
        }
    }

	*/
	return NULL;
}

bool Database::VerifyUser(string account, string pass, string txtfile)
{
	ifstream file;
	file.open(txtfile.c_str());
	if(!file)
	{
		MaloW::Debug("Failed to open " + txtfile + " to verify user " + account);
		return false;
	}
	while(!file.eof())
	{
		string line = "";
		getline(file, line);
		if(line.substr(0, account.size()) == account)
		{
			if(line.substr(account.size() + 1) == pass)
				return true;
		}
	}
	return false;
}