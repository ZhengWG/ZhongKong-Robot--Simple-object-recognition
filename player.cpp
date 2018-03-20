#include "player.h"

using namespace  std;

void ZKBPlayer::play(unsigned char recrst)
{
	switch (recrst)
	{
	case 'a':
		{
			if(PlaySound(TEXT("wanglaoji.wav"),NULL,SND_FILENAME))
				cout<<"playing wanglaoji.wav"<<endl;
			else 
				cout<<"play wanglaoji.wav failed!"<<endl;
			break;
		}
	case 'b':
		{
			if(PlaySound(TEXT("jiaduobao.wav"),NULL,SND_FILENAME))
				cout<<"playing jiaduobao.wav"<<endl;
			else 
				cout<<"play jiaduobao.wav failed!"<<endl;
			break;
		}
	case 'c':
		{
			if(PlaySound(TEXT("xingmu.wav"),NULL,SND_FILENAME))
				cout<<"playing xingmu.wav"<<endl;
			else 
				cout<<"play xingmu.wav failed!"<<endl;
			break;
		}
	case 'd':
		{
			if(PlaySound(TEXT("meinianda.wav"),NULL,SND_FILENAME))
				cout<<"playing meinianda.wav"<<endl;
			else 
				cout<<"play meinianda.wav failed!"<<endl;
			break;
		}
	case 'e':
		{
			if(PlaySound(TEXT("kele.wav"),NULL,SND_FILENAME))
				cout<<"playing kele.wav"<<endl;
			else 
				cout<<"play kele.wav failed!"<<endl;
			break;
		}
	case 'f':
		{
			if(PlaySound(TEXT("lingdu.wav"),NULL,SND_FILENAME))
				cout<<"playing lingdu.wav"<<endl;
			else 
				cout<<"play lingdu.wav failed!"<<endl;
			break;
		}
	case 'g':
		{
			if(PlaySound(TEXT("baishi.wav"),NULL,SND_FILENAME))
				cout<<"playing baishi.wav"<<endl;
			else 
				cout<<"play baishi.wav failed!"<<endl;
			break;
		}
	case 'h':
		{
			if(PlaySound(TEXT("xuebi.wav"),NULL,SND_FILENAME))
				cout<<"playing xuebi.wav"<<endl;
			else 
				cout<<"play xuebi.wav failed!"<<endl;
			break;
		}
	case 'R':
		{
			if(PlaySound(TEXT("red.wav"),NULL,SND_FILENAME))
				cout<<"playing red.wav"<<endl;
			else 
				cout<<"play red.wav failed!"<<endl;
			break;
		}
	case 'Y':
		{
			if(PlaySound(TEXT("yellow.wav"),NULL,SND_FILENAME))
				cout<<"playing yellow.wav"<<endl;
			else 
				cout<<"play yellow.wav failed!"<<endl;
			break;
		}
	case 'B':
		{
			if(PlaySound(TEXT("blue.wav"),NULL,SND_FILENAME))
				cout<<"playing blue.wav"<<endl;
			else 
				cout<<"play blue.wav failed!"<<endl;
			break;
		}
	case 'G':
		{
			if(PlaySound(TEXT("green.wav"),NULL,SND_FILENAME))
				cout<<"playing green.wav"<<endl;
			else 
				cout<<"play green.wav failed!"<<endl;
			break;
		}
	case 'y':
		{
			if(PlaySound(TEXT("yes.wav"),NULL,SND_FILENAME))
				cout<<"playing yes.wav"<<endl;
			else 
				cout<<"play yes.wav failed!"<<endl;
			break;
		}
	case 'n':
		{
			if(PlaySound(TEXT("no.wav"),NULL,SND_FILENAME))
				cout<<"playing no.wav"<<endl;
			else 
				cout<<"play no.wav failed!"<<endl;
			break;
		}
	case 1:
		{
			if(PlaySound(TEXT("sichuan.wav"),NULL,SND_FILENAME))
				cout<<"playing sichuan.wav"<<endl;
			else 
				cout<<"play sichuan.wav failed!"<<endl;
			break;
		}
	case 2:
		{
			if(PlaySound(TEXT("shanxi.wav"),NULL,SND_FILENAME))
				cout<<"playing shanxi.wav"<<endl;
			else 
				cout<<"play shanxi.wav failed!"<<endl;
			break;
		}
	case 'i':
		{
			if(PlaySound(TEXT("introduction.wav"),NULL,SND_FILENAME))
				cout<<"playing introduction.wav"<<endl;
			else 
				cout<<"play introduction.wav failed!"<<endl;
			break;
		}
	case 'q':
		{
			if(PlaySound(TEXT("qingdan.wav"),NULL,SND_FILENAME))
				cout<<"playing qingdan.wav"<<endl;
			else 
				cout<<"play qingdan.wav failed!"<<endl;
			break;
		}
	}
}