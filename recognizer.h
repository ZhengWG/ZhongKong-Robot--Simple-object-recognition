#ifndef RECOGNIZER_H_
#define RECOGNIZER_H_
#include "cmhead.h"
#include "image.h"
using namespace std;
using namespace cv;
using namespace cv::gpu;
/*识别类，功能有：
	匹配输入的ZKBImage_vector中的图像对象和frame图像对象*/
class ZKBRecognizer
{
public:
	int count;		//count保存识别几帧图片输出结果
	uchar Recrst;	//Recrst保存识别结果
	ZKBRecognizer();
	ZKBRecognizer(int m_count);


	/************************************************************************/
	/* 开始识别，识别结果保存在uchar Recrst中;buhuo_mode = 1时为补货模式*/
	/************************************************************************/
	int startRec(vector<ZKBImage> ZKBImage_vector,int buhuo_mode);

	/************************************************************************/
	/* 如果输入的向量中有大于5的，则认为这里有某事物*/
	/************************************************************************/
	bool hereIsSomething(vector<int>a,int thre);

	/************************************************************************/
	/* 得到向量中的最大值对应的下标*/
	/************************************************************************/
	void getMax(vector<int>a,int &index);

	/************************************************************************/
	/*返回：得到两个对象匹配上的点的对数*/
	/************************************************************************/
	int getNumofMatches(ZKBImage img_object,ZKBImage frame_object);

};



#endif