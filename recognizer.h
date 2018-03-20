#ifndef RECOGNIZER_H_
#define RECOGNIZER_H_
#include "cmhead.h"
#include "image.h"
using namespace std;
using namespace cv;
using namespace cv::gpu;
/*ʶ���࣬�����У�
	ƥ�������ZKBImage_vector�е�ͼ������frameͼ�����*/
class ZKBRecognizer
{
public:
	int count;		//count����ʶ��֡ͼƬ������
	uchar Recrst;	//Recrst����ʶ����
	ZKBRecognizer();
	ZKBRecognizer(int m_count);


	/************************************************************************/
	/* ��ʼʶ��ʶ����������uchar Recrst��;buhuo_mode = 1ʱΪ����ģʽ*/
	/************************************************************************/
	int startRec(vector<ZKBImage> ZKBImage_vector,int buhuo_mode);

	/************************************************************************/
	/* ���������������д���5�ģ�����Ϊ������ĳ����*/
	/************************************************************************/
	bool hereIsSomething(vector<int>a,int thre);

	/************************************************************************/
	/* �õ������е����ֵ��Ӧ���±�*/
	/************************************************************************/
	void getMax(vector<int>a,int &index);

	/************************************************************************/
	/*���أ��õ���������ƥ���ϵĵ�Ķ���*/
	/************************************************************************/
	int getNumofMatches(ZKBImage img_object,ZKBImage frame_object);

};



#endif