#pragma once
#ifndef ACCOUNT_H_
#define ACCOUNT_H_

#include "Functionalities.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>

extern char *name, *surname,*fullname, *filename;

void Edit();

void View();

void Create();

void Transaction();

void Delete();

void Add_New_Account();

#endif


