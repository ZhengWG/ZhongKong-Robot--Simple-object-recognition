#include "image.h"
#include "recognizer.h"
#include "SerialPort.h"
#include "stdafx.h"
#include "player.h"
using namespace std;
using namespace cv;
using namespace cv::gpu;
/************************************************************************/
/* �����˲���ģ����*/
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
	/* ��Ʒ�嵥*/
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
	/* ��ʼ������*/
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
	/* ��ȡgpu device*/
	/************************************************************************/
	int num_devices=getCudaEnabledDeviceCount();
	cout<<"num_devices= "<<num_devices<<endl;
	printShortCudaDeviceInfo(gpu::getDevice());
	/************************************************************************/
	/* ����ͼƬ*/
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
	/* ����8��ͼƬ����*/
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
	/* ��8��ͼƬ����ѹ��ZKBImg_vector*/
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
	/* ����һ��ʶ����ZKBRecognizer,����һ��������ZKBPlayer*/
	/************************************************************************/
	ZKBRecognizer Recognizer(10);
	ZKBPlayer player;
	/************************************************************************/
	/* ���嵥����bao,�ȵȴ�bao��'s'�������ҿ�ʼ����list��Ȼ��ͷ��ĸ���������*/
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
	/* �������ҽ���*/
	/************************************************************************/
	player.play('i');
	player.play(1);
	player.play(2);
	player.play('q');
	Sleep(10000);//�����������ȴ�55s
	char sendlist_response;
	uchar startflag = 'S';
	uchar* pList = &startflag;
	while(!mySerialPort.WriteData(pList,1)){}
	cout<<"I had sent "<<*pList<<" to bao..."<<endl;
	while (!mySerialPort.ReadChar(sendlist_response)){}
	cout<<"I had receive "<<sendlist_response<<" from bao..."<<endl;
	/************************************************************************/
	/* �ȴ�bao��������ʼʶ����ź�'1~:'*/
	/************************************************************************/
	char signal=0x00;
	vector<uchar> send2bao;
	vector<char> address;
	while(1)
	{
		cout<<"waiting for signal:'1~:'..."<<endl;
		while (!mySerialPort.ReadChar(signal) || !((signal>='1') && (signal<=':'))){}
		cout<<"start rec..."<<endl;
		
		if ((signal == '9')||(signal == ':')) //���signal��'9'����':'��Ϊ����ģʽ
		{
			Sleep(500);
			Recognizer.startRec(ZKBImage_vector,1);
			/************************************************************************/
			/* ������*/
			/************************************************************************/
			//player.play(Recognizer.Recrst);
			/************************************************************************/
			/* ��ʼ��ʶ�������з����������ͷ������*/
			/************************************************************************/
			cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
			cout<<"find: "<<Recognizer.Recrst<<endl;
			if ((Recognizer.Recrst == can_list[0]) || (Recognizer.Recrst == can_list[1]))
			{
				player.play(Recognizer.Recrst);
				/************************************************************************/
				/* �ȱ��淢�͵Ķ����͵�ַ */
				/************************************************************************/
				send2bao.push_back(Recognizer.Recrst);
				address.push_back(signal);
				/************************************************************************/
				/* ���� */
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
				/* �ȱ��淢�͵Ķ����͵�ַ */
				/************************************************************************/
				send2bao.push_back(Recognizer.Recrst);
				address.push_back(signal);
				/************************************************************************/
				/* ���� */
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
// 				/* �ȱ��淢�͵Ķ����͵�ַ */
// 				/************************************************************************/
// 				send2bao.push_back(Recognizer.Recrst);
// 				address.push_back(signal);
// 				/************************************************************************/
// 				/* ���� */
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
				/* ���� */
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
			//ȡ��ģʽ
			Sleep(500);
 			Recognizer.startRec(ZKBImage_vector,0);
			/************************************************************************/
			/* ��������δ���*/
			/************************************************************************/
			player.play(Recognizer.Recrst);
			/************************************************************************/
			/* ��ʼ��ʶ�������з����������ͷ������*/
			/************************************************************************/
			cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
			cout<<"find: "<<Recognizer.Recrst<<endl;
			if ((Recognizer.Recrst == can_list[0]) || (Recognizer.Recrst == can_list[1]))
			{
				
				/************************************************************************/
				/* �ȱ��淢�͵Ķ����͵�ַ */
				/************************************************************************/
				send2bao.push_back(Recognizer.Recrst);
				address.push_back(signal);
				/************************************************************************/
				/* ���� */
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
				/* �ȱ��淢�͵Ķ����͵�ַ */
				/************************************************************************/
				send2bao.push_back(Recognizer.Recrst);
				address.push_back(signal);
				/************************************************************************/
				/* ���� */
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
				/* �ȱ��淢�͵Ķ����͵�ַ */
				/************************************************************************/
				send2bao.push_back(Recognizer.Recrst);
				address.push_back(signal);
				/************************************************************************/
				/* ���� */
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
				/* ���� */
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