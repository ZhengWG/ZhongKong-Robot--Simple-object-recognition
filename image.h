#ifndef IMAGE_H_
#define IMAGE_H_
#include "cmhead.h"
using namespace std;
using namespace cv::gpu;
using namespace cv;
/*ͼƬ�࣬���в�ͼ���Ҷ�ͼ����ɫ��gpuͼ��gpu�����㡢gpu�����ӡ�cpu������*/
/*����Ĺ����У����ͼƬimg����ɫ*/
class ZKBImage
{
public:
	Mat img;
	Mat color_img;
	Mat img_grey;
	uchar color;
	GpuMat img_gpu;
	GpuMat img_keypoint_gpu;	
	GpuMat img_descriptor_gpu;
	vector<KeyPoint> img_keypoints;
	
		
	ZKBImage(Mat m_img);	
	~ZKBImage();
	uchar getcolor();
	long isRed(Mat &res);
	long isYellow(Mat &res);
	long isBlue(Mat &res);
	long isGreen(Mat &res);


};





#endif