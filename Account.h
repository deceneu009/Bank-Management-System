#pragma once
#ifndef ACCOUNT_H_
#define ACCOUNT_H_

#include "Functionalities.h"
#include <unistd.h>

extern char *name, *surname, *fullname, *filename;

void Edit();
void View();
void Create();
void Transaction();
void Delete();
void Add_New_Account();
char* person_init(char* argv1, char* argv2);
int check_option(int option);

#endif

