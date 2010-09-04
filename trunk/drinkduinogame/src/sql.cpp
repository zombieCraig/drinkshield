/*
 * SQLite3 Class
 */

#include "sql.h"

Sql::Sql()
{
   db = NULL;
   verbose = 0;
   dbfile = string(DB_FILE);
}

Sql::~Sql()
{

}

void Sql::setDBFile(string newdb)
{
   dbfile = newdb;
}

string Sql::getDBFile()
{
   return dbfile;
}

// Initializes the tables for game usuage
// Returns 1 on success 0 on failure
int Sql::init()
{
    struct sqlite3_stmt *stmt;
    const char *err;
    int rc;
    rc = sqlite3_open(dbfile.c_str(), &db);
    if (rc) {
	cerr << "Could not open dbfile: " << dbfile << endl;
	return 0;
     }
     rc= sqlite3_prepare_v2(db, CREATE_PLAYER_TBL, -1, &stmt, NULL);
     if(rc != SQLITE_OK) {
	cerr << "Could not prepare players table" << endl;
	return 0;
     }
     rc = sqlite3_step(stmt);
     if(rc == SQLITE_DONE) {
	if(verbose)
		cout << "Initialized players table" << endl;
	sqlite3_reset(stmt);
     }

     rc = sqlite3_prepare_v2(db, CREATE_SCORE_TBL, -1, &stmt, NULL);
     if(rc != SQLITE_OK) {
	cerr << "Could not prepare scores table" << endl;
	return 0;
     }
     rc = sqlite3_step(stmt);
     if(rc == SQLITE_DONE) {
	if(verbose)
		cout << "Initialized scores table" << endl;
	sqlite3_reset(stmt);
     }
     sqlite3_finalize(stmt);
     return 1;
}

// sqlite3 exec
int Sql::exec(const char *query, int (*callback)(void*,int,char**,char**), void *p)
{
	int rc;
	char *errmsg = 0;
	rc = sqlite3_exec(db, query, callback, p, &errmsg);
  	if(rc!=SQLITE_OK) {
    		cerr << "SQL error: " << errmsg << endl;
		return 0;
  	}
	return 1;
}

// Inserts a new score record for a given player id
void Sql::insertScore(int id, int score, time_t recordtime)
{
   char query[256];
    snprintf(query, 255, "INSERT INTO scores VALUES(%d,%d,%d)",id, score, (int)recordtime);
   exec(query, NULL, NULL);
}

// Inserts a new player to the DB
int Sql::insertPlayer(string playerName)
{
   char query[256];
    snprintf(query, 255, "INSERT INTO players VALUES(NULL,'%s')",playerName.c_str());
   exec(query, NULL, NULL);
   return sqlite3_last_insert_rowid(db);
}
