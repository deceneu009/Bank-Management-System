#pragma once
#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include <conio.h>

extern int IbanExists;

typedef struct user
{
    char *Owner;
    char *Iban;
    char *Coin;
    unsigned long long int Amount;
} user;

void clearConsole();

int isEmpty(const char *str);

int check_last_line(char Filename[]);

int check_Iban(char *Iban, char *Owner, int flag);

char *GetIban(int flag, char *Owner);

char *GetCoin();

unsigned long long int GetAmount();

char *loadUserOwner(char Filename[]);

#endif