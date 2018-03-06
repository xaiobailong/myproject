#ifndef USER_H
#define USER_H

#include<json/json.h>
#include<mysql/mysql.h>
#include <iostream>
using namespace std;

void user_insert(Json::Value val);

int user_find(Json::Value val);

char *get_name();

void online_insert(Json::Value val);

char *online_find(char *name);

#endif
