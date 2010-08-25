/*
 * SQLite3 Class
 */
#ifndef __SQL_H__
#define __SQL_H__

#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <iostream>

#define DB_FILE "drinkScorez"
#define CREATE_PLAYER_TBL "CREATE TABLE IF NOT EXISTS players(id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(256) NOT NULL UNIQUE, pic VARCHAR(256))"
#define CREATE_SCORE_TBL "CREATE TABLE IF NOT EXISTS scores(id INTEGER, score INTEGER, submitted INTEGER)"
 #define GET_SCORES "SELECT * FROM scores WHERE id=? ORDER BY score DESC LIMIT 10 "

using namespace std;

class Sql
{
public:
  Sql();
  ~Sql();
  int verbose;

  void setDBFile(string);
  string getDBFile();
  int exec(const char *, int (*callback)(void*,int,char**,char**), void *);
  int init();
  void insertScore(int,int,time_t);
  int insertPlayer(string,string);
private:
  sqlite3 *db;
  string dbfile;
};

#endif /* __SQL_H__ */
