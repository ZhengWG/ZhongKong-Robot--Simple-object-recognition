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
	/* ��ʼ�������ۼƵ�����*/
	/************************************************************************/
	vector<int> leiji;
	for (int i=0;i!=ZKBImage_vector.size();i++)
	{
		leiji.push_back(0);
	}
	/************************************************************************/
	/* ��ʼ�������ۼ���ɫ�ı���*/
	/************************************************************************/
	int red_leiji=0;
	int yellow_leiji=0;
	int blue_leiji=0;
	int green_leiji=0;
	/************************************************************************/
	/* ������ͷ,����ͷԭͼΪMat frame_img,�õ�����ͷͼ�����frame_object*/
	/************************************************************************/
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cout<<"cap open failed!!!";
		return -1;	
	}
	cout<<"cap open success"<<endl;
/************************************************************************/
/* ��ʼ��count������ͷͼ����ʶ��*/
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
		//����ǲ���ģʽ���Ͱ�farme_img���߿���
		imshow("capture",frame_img);
		ZKBImage frame_object(frame_img);
		/************************************************************************/
		/* ���ж�������ɫ,���ۼ�20֡*/
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
		/* �õ�8��ͼ��frameͼ��ƥ��ĸ�����������NumofMatches������*/
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
		/* �ж������Ƿ���ĳ����ڣ�����У����ƥ���������ͼ������index����ʹleiji[index]++ */
		/************************************************************************/
		if (hereIsSomething(NumofMatch,9))	//	���NumofMatch��������һ��ֵ�Ǵ���9��Ҳ������Ϊ��8���е�ĳһ��ƥ�����ˣ���ô��Ӧ��leiji������1
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
	/* ʶ����count��frame֮�󣬶����ɵ�leiji�������ĸ���ɫ�ۼƱ���������*/
	/************************************************************************/
	vector<int> color_leiji;	//�Ȱ���ɫ�ۼ�ֵѹ��color_leiji�����У�ѹ���˳��ͺ����йأ��±�0~3�����������
	color_leiji.push_back(red_leiji);
	color_leiji.push_back(yellow_leiji);
	color_leiji.push_back(blue_leiji);
	color_leiji.push_back(green_leiji);
	/************************************************************************/
	/* ���leiji������Ҳ������count��ʶ���8��ͼƬƥ����Ĵ���*/
	/************************************************************************/
	cout<<"leiji: "<<endl;
	for (int i=0;i!=leiji.size();i++)
	{
		cout<<i<<":"<<leiji[i]<<", ";
	}
	cout<<endl;
	if (hereIsSomething(leiji,5))	//���leiji��������һ��ֵ�Ǵ���5��Ҳ����8��ͼƬ����һ�ű�ƥ���˴���5�Σ�����Ϊ�й�ͷ����ǰ
	{
		int index;
		getMax(leiji,index); 
		Recrst = index + 'a';
		cout<<"the result of recognization in this time is "<<Recrst<<endl;
	}
	else if(hereIsSomething(color_leiji,5))	//���color_leiji���������д���5�ģ�����Ϊǰ������ɫľ��
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
/* ���������������д���5�ģ�����Ϊ������ĳ����*/
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
/* �õ������е����ֵ��Ӧ���±�*/
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
/*���أ��õ���������ƥ���ϵĵ�Ķ���*/
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