#ifndef IMAGE_H_
#define IMAGE_H_
#include "cmhead.h"
using namespace std;
using namespace cv::gpu;
using namespace cv;
/*图片类，具有彩图、灰度图、颜色、gpu图像、gpu特征点、gpu描述子、cpu特征点*/
/*该类的功能有：获得图片img的颜色*/
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