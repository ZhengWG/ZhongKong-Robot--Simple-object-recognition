#ifndef PLAYER_H_
#define PLAYER_H_
#include <iostream>
#include <Windows.h>
#include <MMSystem.h>
#pragma comment(lib,"WINMM.LIB")

using namespace std;

class ZKBPlayer
{
public:
	void play(unsigned char recrst);
};

#endif