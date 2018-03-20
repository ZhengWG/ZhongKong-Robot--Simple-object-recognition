#include "recognizer.h"
using namespace std;
using namespace cv;
using namespace cv::gpu;

ZKBRecognizer::ZKBRecognizer()
{
	count = 20;
	Recrst = 0x00;
}
ZKBRecognizer::ZKBRecognizer(int m_count)
{
	count = m_count;
	Recrst = 0x00;
}

int ZKBRecognizer::startRec(vector<ZKBImage> ZKBImage_vector,int buhuo_mode)
{
	/************************************************************************/
	/* 初始化用来累计的向量*/
	/************************************************************************/
	vector<int> leiji;
	for (int i=0;i!=ZKBImage_vector.size();i++)
	{
		leiji.push_back(0);
	}
	/************************************************************************/
	/* 初始化用来累计颜色的变量*/
	/************************************************************************/
	int red_leiji=0;
	int yellow_leiji=0;
	int blue_leiji=0;
	int green_leiji=0;
	/************************************************************************/
	/* 打开摄像头,摄像头原图为Mat frame_img,得到摄像头图像对象frame_object*/
	/************************************************************************/
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cout<<"cap open failed!!!";
		return -1;	
	}
	cout<<"cap open success"<<endl;
/************************************************************************/
/* 开始对count张摄像头图像做识别*/
/************************************************************************/
	Mat frame_img;
	for (int j=0;j!=count;j++)
	{
		for (int i=0;i!=3;i++)
		{
			cap.read(frame_img);
		}
		if (buhuo_mode)
		{
			cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~buhuo....."<<endl;
			Mat ROI_img(frame_img,Rect(int(0.25*frame_img.cols),0,int(0.65*frame_img.cols),frame_img.rows));
			ROI_img.copyTo(frame_img);
		}
		//如果是补货模式，就把farme_img两边砍掉
		imshow("capture",frame_img);
		ZKBImage frame_object(frame_img);
		/************************************************************************/
		/* 先判断物体颜色,并累计20帧*/
		/************************************************************************/
		switch (frame_object.getcolor())
		{
		case 'R':
			{
				red_leiji++;
				//cout<<"red"<<endl;
				break;
			}
		case 'Y':
			{
				yellow_leiji++;
				//cout<<"yellow"<<endl;
				break;
			}
		case 'B':
			{
				blue_leiji++;
				//cout<<"blue"<<endl;
				break;
			}
		case 'G':
			{
				green_leiji++;
				//cout<<"green"<<endl;
				break;
			}
		}
		vector<int> NumofMatch;
		/************************************************************************/
		/* 得到8张图与frame图像匹配的个数，保存在NumofMatches向量中*/
		/************************************************************************/
		for (int i=0;i!=ZKBImage_vector.size();i++)
		{
			NumofMatch.push_back(getNumofMatches(ZKBImage_vector[i],frame_object));
		}
		//cout<<"numofmatch.size(): "<<NumofMatch.size()<<endl;
		for (int i=0;i!=NumofMatch.size();i++)
		{
			cout<<i<<": "<<NumofMatch[i]<<", ";
		}
		cout<<endl;
		/************************************************************************/
		/* 判断这里是否有某物存在，如果有，获得匹配个数最多的图像索引index，并使leiji[index]++ */
		/************************************************************************/
		if (hereIsSomething(NumofMatch,9))	//	如果NumofMatch向量中有一个值是大于9，也就是认为和8张中的某一张匹配上了，那么对应的leiji向量加1
		{
			int index;
			getMax(NumofMatch,index);
			uchar temp_rec_rst = index+'a';
			cout<<"recrst of every single frame is "<<temp_rec_rst<<endl;	
			leiji[index]++;
		}
		if (waitKey(30)==27)
		{
			break;
		}
	}
	destroyWindow("capture");
	/************************************************************************/
	/* 识别完count张frame之后，对生成的leiji向量和四个颜色累计变量作分析*/
	/************************************************************************/
	vector<int> color_leiji;	//先把颜色累计值压入color_leiji向量中，压入的顺序和后面有关，下标0~3代表：红黄蓝绿
	color_leiji.push_back(red_leiji);
	color_leiji.push_back(yellow_leiji);
	color_leiji.push_back(blue_leiji);
	color_leiji.push_back(green_leiji);
	/************************************************************************/
	/* 输出leiji向量，也就是在count次识别后8张图片匹配过的次数*/
	/************************************************************************/
	cout<<"leiji: "<<endl;
	for (int i=0;i!=leiji.size();i++)
	{
		cout<<i<<":"<<leiji[i]<<", ";
	}
	cout<<endl;
	if (hereIsSomething(leiji,5))	//如果leiji向量中有一个值是大于5，也就是8张图片中有一张被匹配了大于5次，则认为有罐头在眼前
	{
		int index;
		getMax(leiji,index); 
		Recrst = index + 'a';
		cout<<"the result of recognization in this time is "<<Recrst<<endl;
	}
	else if(hereIsSomething(color_leiji,5))	//如果color_leiji向量后总有大于5的，则认为前面有颜色木块
	{
		int index;
		getMax(color_leiji,index);
		switch (index)
		{
		case 0:Recrst = 'R';
			break;
		case 1:Recrst = 'Y';
			break;
		case 2:Recrst = 'B';
			break;
		case 3:Recrst = 'G';
			break;
		}
		cout<<"the result of recognization in this time is "<<Recrst<<endl;
	}
	else
	{
		Recrst = 0x02;
		cout<<"Nothing!"<<endl;
	}
	return 0;
}
/************************************************************************/
/* hereIsSomething*/
/* 如果输入的向量中有大于5的，则认为这里有某事物*/
/************************************************************************/
bool ZKBRecognizer::hereIsSomething(vector<int>a,int thre)
{
	for (int i=0;i!=a.size();i++)
	{
		if (a[i] >= thre)
			return true;
	}
	return false;
}
/************************************************************************/
/* 得到向量中的最大值对应的下标*/
/************************************************************************/
void ZKBRecognizer::getMax(vector<int>a,int &index)
{
	int max = 0;
	for (int i=0;i!=a.size();i++)
	{
		if (a[i]>max)
		{
			max = a[i];
			index = i;
		}
	}
}
/************************************************************************/
/*返回：得到两个对象匹配上的点的对数*/
/************************************************************************/
int ZKBRecognizer::getNumofMatches(ZKBImage img_object,ZKBImage frame_object)
{
	vector<DMatch> finalmatches;
	BruteForceMatcher_GPU<L2<float>> matcher;
	vector<vector<DMatch>> matches;
	matches.clear();
	matcher.knnMatch(img_object.img_descriptor_gpu,frame_object.img_descriptor_gpu,matches,2);
	int finalmsize = matches.size();
	return finalmsize;
}