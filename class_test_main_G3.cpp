#include "image.h"
#include "recognizer.h"
#include "SerialPort.h"
#include "stdafx.h"
#include "player.h"
using namespace std;
using namespace cv;
using namespace cv::gpu;
/************************************************************************/
/* 增加了补货模部分*/
/************************************************************************/
/************************************************************************/
/* 	
 NO			= 'n'
 WANGLAOJI	= 'a';
 JIADUOBAO	= 'b';
 XINGMU		= 'c';
 MEINIANDA	= 'd';
 KELE		= 'e';
 LINGDU		= 'f';
 BAISHI		= 'g';
 XUEBI		= 'h';                                                                    
*/
/************************************************************************/
int main()
{

	/************************************************************************/
	/* 物品清单*/
	/************************************************************************/
	uchar NO = 'n';
	uchar WANGLAOJI	= 'a';
	uchar JIADUOBAO	= 'b';
	uchar XINGMU	= 'c';
	uchar MEINIANDA	= 'd';
	uchar KELE		= 'e';
	uchar LINGDU	= 'f';
	uchar BAISHI	= 'g';
	uchar XUEBI		= 'h';
	uchar RED		= 'R';
	uchar YELLOW	= 'Y';
	uchar BLUE		= 'B';
	uchar GREEN		= 'G';

/************************************************************************/
/* 	
	uchar wood_list[2] = {RED,BLUE};          //A B 
	uchar can_list[2] = {XUEBI,WANGLAOJI};    //C D
*/
/************************************************************************/
	uchar wood_list[2] = {BLUE,GREEN};
	uchar can_list[2] = {LINGDU,XINGMU};
	/************************************************************************/
	/* 初始化串口*/
	/************************************************************************/
	CSerialPort mySerialPort;

	if (!mySerialPort.InitPort(1))
	{
		cout << "initPort fail !" << endl;
		return -1;
	}
	else
	{
		cout << "initPort success !" << endl;
	}
	/************************************************************************/
	/* 获取gpu device*/
	/************************************************************************/
	int num_devices=getCudaEnabledDeviceCount();
	cout<<"num_devices= "<<num_devices<<endl;
	printShortCudaDeviceInfo(gpu::getDevice());
	/************************************************************************/
	/* 读入图片*/
	/************************************************************************/
	Mat wanglaoji_img	=imread("e:/f/wanglaoji.jpg");
	Mat jiaduobao_img	=imread("e:/f/jiaduobao.jpg");
	Mat xingmu_img		=imread("e:/f/xingmu.jpg");
	Mat meinianda_img	=imread("e:/f/meinianda.jpg");
	Mat kele_img		=imread("e:/f/kele.jpg");
	Mat lingdu_img		=imread("e:/f/lingdu.jpg");
	Mat baishi_img		=imread("e:/f/baishi.jpg");
	Mat xuebi_img		=imread("e:/f/xuebi.jpg");
	
	if (wanglaoji_img.empty()||jiaduobao_img.empty()||xingmu_img.empty()||meinianda_img.empty()
		||kele_img.empty()||lingdu_img.empty()||baishi_img.empty()||xuebi_img.empty())
	{
		cout<<"Error:img open failed!!"<<endl;
		return -1;
	}
	/************************************************************************/
	/* 创建8张图片对象*/
	/************************************************************************/
	ZKBImage wanglaoji_object(wanglaoji_img);
	ZKBImage jiaduobao_object(jiaduobao_img);
	ZKBImage xingmu_object(xingmu_img);
	ZKBImage meinianda_object(meinianda_img);
	ZKBImage kele_object(kele_img);
	ZKBImage lingdu_object(lingdu_img);
	ZKBImage baishi_object(baishi_img);
	ZKBImage xuebi_object(xuebi_img);
	/************************************************************************/
	/* 把8个图片对象压入ZKBImg_vector*/
	/************************************************************************/
	vector<ZKBImage> ZKBImage_vector;
	ZKBImage_vector.push_back(wanglaoji_object);
	ZKBImage_vector.push_back(jiaduobao_object);
	ZKBImage_vector.push_back(xingmu_object);
	ZKBImage_vector.push_back(meinianda_object);
	ZKBImage_vector.push_back(kele_object);
	ZKBImage_vector.push_back(lingdu_object);
	ZKBImage_vector.push_back(baishi_object);
	ZKBImage_vector.push_back(xuebi_object);
	/************************************************************************/
	/* 定义一个识别器ZKBRecognizer,定义一个播放器ZKBPlayer*/
	/************************************************************************/
	ZKBRecognizer Recognizer(10);
	ZKBPlayer player;
	/************************************************************************/
	/* 把清单发给bao,先等待bao发's'过来叫我开始发送list，然后就发四个东西给他*/
	/************************************************************************/
	char sendlist_signal = 0x00;
	cout<<"waiting for signal to send list to bao..."<<endl;
	while (!mySerialPort.ReadChar(sendlist_signal) || !(sendlist_signal=='s')){}
	for (int i=0;i!=2;i++)
	{
		char sendlist_response;
		uchar* pList = &wood_list[i];
		while(!mySerialPort.WriteData(pList,1)){}
		cout<<"I had sent "<<*pList<<" to bao..."<<endl;
		while (!mySerialPort.ReadChar(sendlist_response)){}
		cout<<"I had receive "<<sendlist_response<<" from bao..."<<endl;
	}
	for (int i=0;i!=2;i++)
	{
		char sendlist_response;
		uchar* pList = &can_list[i];
		while(!mySerialPort.WriteData(pList,1)){}
		cout<<"I had sent "<<*pList<<" to bao..."<<endl;
		while (!mySerialPort.ReadChar(sendlist_response)){}
		cout<<"I had receive "<<sendlist_response<<" from bao..."<<endl;
	}
	/************************************************************************/
	/* 语音自我介绍*/
	/************************************************************************/
	player.play('i');
	player.play(1);
	player.play(2);
	player.play('q');
	Sleep(10000);//加上语音共等待55s
	char sendlist_response;
	uchar startflag = 'S';
	uchar* pList = &startflag;
	while(!mySerialPort.WriteData(pList,1)){}
	cout<<"I had sent "<<*pList<<" to bao..."<<endl;
	while (!mySerialPort.ReadChar(sendlist_response)){}
	cout<<"I had receive "<<sendlist_response<<" from bao..."<<endl;
	/************************************************************************/
	/* 等待bao发过来开始识别的信号'1~:'*/
	/************************************************************************/
	char signal=0x00;
	vector<uchar> send2bao;
	vector<char> address;
	while(1)
	{
		cout<<"waiting for signal:'1~:'..."<<endl;
		while (!mySerialPort.ReadChar(signal) || !((signal>='1') && (signal<=':'))){}
		cout<<"start rec..."<<endl;
		
		if ((signal == '9')||(signal == ':')) //如果signal是'9'或者':'则为补货模式
		{
			Sleep(500);
			Recognizer.startRec(ZKBImage_vector,1);
			/************************************************************************/
			/* 加语音*/
			/************************************************************************/
			//player.play(Recognizer.Recrst);
			/************************************************************************/
			/* 开始对识别结果进行分析，并发送分析结果*/
			/************************************************************************/
			cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
			cout<<"find: "<<Recognizer.Recrst<<endl;
			if ((Recognizer.Recrst == can_list[0]) || (Recognizer.Recrst == can_list[1]))
			{
				player.play(Recognizer.Recrst);
				/************************************************************************/
				/* 先保存发送的东西和地址 */
				/************************************************************************/
				send2bao.push_back(Recognizer.Recrst);
				address.push_back(signal);
				/************************************************************************/
				/* 发送 */
				/************************************************************************/
				uchar* pRec_rst = &Recognizer.Recrst;
				while(!mySerialPort.WriteData(pRec_rst,1)){}
				cout<<"I had sent "<<Recognizer.Recrst<<" to bao..."<<endl;
				char response;
				while (!mySerialPort.ReadChar(response)){}
				cout<<"I had receive "<<response<<" from bao..."<<endl;
				player.play('y');
			}
			else if ((Recognizer.Recrst == wood_list[0]) || (Recognizer.Recrst == wood_list[1]))
			{
				player.play(Recognizer.Recrst);
				/************************************************************************/
				/* 先保存发送的东西和地址 */
				/************************************************************************/
				send2bao.push_back(Recognizer.Recrst);
				address.push_back(signal);
				/************************************************************************/
				/* 发送 */
				/************************************************************************/
				uchar* pRec_rst = &Recognizer.Recrst;
				while(!mySerialPort.WriteData(pRec_rst,1)){}
				cout<<"I had sent "<<Recognizer.Recrst<<" to bao..."<<endl;
				char response;
				while (!mySerialPort.ReadChar(response)){}
				cout<<"I had receive "<<response<<" from bao..."<<endl;
				//cout<<"send :"<<Recognizer.Recrst<<endl;
				player.play('y');
			}
// 			else if ((Recognizer.Recrst == wood_list[1]) && ((signal == '3') || (signal == '4')))
// 			{
// 
// 				/************************************************************************/
// 				/* 先保存发送的东西和地址 */
// 				/************************************************************************/
// 				send2bao.push_back(Recognizer.Recrst);
// 				address.push_back(signal);
// 				/************************************************************************/
// 				/* 发送 */
// 				/************************************************************************/
// 				uchar* pRec_rst = &Recognizer.Recrst;
// 				while(!mySerialPort.WriteData(pRec_rst,1)){}
// 				cout<<"I had sent "<<Recognizer.Recrst<<" to bao..."<<endl;
// 				char response;
// 				while (!mySerialPort.ReadChar(response)){}
// 				cout<<"I had receive "<<response<<" from bao..."<<endl;
// 				//cout<<"send :"<<Recognizer.Recrst<<endl;
// 				player.play('y');
// 			}
			else 
			{

				/************************************************************************/
				/* 发送 */
				/************************************************************************/
				uchar* pRec_rst = &NO;
				while(!mySerialPort.WriteData(pRec_rst,1)){}
				cout<<"I had sent "<<NO<<" to bao..."<<endl;
				char response;
				while (!mySerialPort.ReadChar(response)){}
				cout<<"I had receive "<<response<<" from bao..."<<endl;
				//cout<<"send: No"<<endl;
				//player.play('n');
			}
		}
		else
		{
			//取货模式
			Sleep(500);
 			Recognizer.startRec(ZKBImage_vector,0);
			/************************************************************************/
			/* 加语音，未完成*/
			/************************************************************************/
			player.play(Recognizer.Recrst);
			/************************************************************************/
			/* 开始对识别结果进行分析，并发送分析结果*/
			/************************************************************************/
			cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
			cout<<"find: "<<Recognizer.Recrst<<endl;
			if ((Recognizer.Recrst == can_list[0]) || (Recognizer.Recrst == can_list[1]))
			{
				
				/************************************************************************/
				/* 先保存发送的东西和地址 */
				/************************************************************************/
				send2bao.push_back(Recognizer.Recrst);
				address.push_back(signal);
				/************************************************************************/
				/* 发送 */
				/************************************************************************/
				uchar* pRec_rst = &Recognizer.Recrst;
				while(!mySerialPort.WriteData(pRec_rst,1)){}
				cout<<"I had sent "<<Recognizer.Recrst<<" to bao..."<<endl;
				char response;
				while (!mySerialPort.ReadChar(response)){}
				cout<<"I had receive "<<response<<" from bao..."<<endl;
				player.play('y');
			}
			else if ((Recognizer.Recrst == wood_list[0]) && ((signal == '1') || (signal == '2')))
			{
				
				/************************************************************************/
				/* 先保存发送的东西和地址 */
				/************************************************************************/
				send2bao.push_back(Recognizer.Recrst);
				address.push_back(signal);
				/************************************************************************/
				/* 发送 */
				/************************************************************************/
				uchar* pRec_rst = &Recognizer.Recrst;
				while(!mySerialPort.WriteData(pRec_rst,1)){}
				cout<<"I had sent "<<Recognizer.Recrst<<" to bao..."<<endl;
				char response;
				while (!mySerialPort.ReadChar(response)){}
				cout<<"I had receive "<<response<<" from bao..."<<endl;
				//cout<<"send :"<<Recognizer.Recrst<<endl;
				player.play('y');
			}
			else if ((Recognizer.Recrst == wood_list[1]) && ((signal == '3') || (signal == '4')))
			{
				
				/************************************************************************/
				/* 先保存发送的东西和地址 */
				/************************************************************************/
				send2bao.push_back(Recognizer.Recrst);
				address.push_back(signal);
				/************************************************************************/
				/* 发送 */
				/************************************************************************/
				uchar* pRec_rst = &Recognizer.Recrst;
				while(!mySerialPort.WriteData(pRec_rst,1)){}
				cout<<"I had sent "<<Recognizer.Recrst<<" to bao..."<<endl;
				char response;
				while (!mySerialPort.ReadChar(response)){}
				cout<<"I had receive "<<response<<" from bao..."<<endl;
				//cout<<"send :"<<Recognizer.Recrst<<endl;
				player.play('y');
			}
			else 
			{
				
				/************************************************************************/
				/* 发送 */
				/************************************************************************/
				uchar* pRec_rst = &NO;
				while(!mySerialPort.WriteData(pRec_rst,1)){}
				cout<<"I had sent "<<NO<<" to bao..."<<endl;
				char response;
				while (!mySerialPort.ReadChar(response)){}
				cout<<"I had receive "<<response<<" from bao..."<<endl;
				//cout<<"send: No"<<endl;
				player.play('n');

			}
		}
	}	
		return 0;
}