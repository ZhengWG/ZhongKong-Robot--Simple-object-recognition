#include "image.h"
using namespace std;
using namespace cv;
using namespace cv::gpu;

ZKBImage::ZKBImage(Mat m_img)
{
	img = m_img;
	Mat ROI_img(m_img,Rect(0,int(0.5*m_img.rows),m_img.cols,int(0.499*m_img.rows)));
	ROI_img.copyTo(color_img);
	cvtColor(m_img,img_grey,CV_BGR2GRAY);
	img_gpu.upload(img_grey);
	//cout<<"upload img to gpu finished..."<<endl;
	SURF_GPU gpusurf(2000);
	gpusurf(img_gpu,GpuMat(),img_keypoint_gpu,img_descriptor_gpu);
	//cout<<"compute keypoints and descriptors finished...."<<endl;
	gpusurf.downloadKeypoints(img_keypoint_gpu,img_keypoints);
	//cout<<"download keypoints_gpu to keypoints finished..."<<endl;
	//cout<<"Constructor end!"<<endl;
}
ZKBImage::~ZKBImage(){}

uchar ZKBImage::getcolor()
{
	uchar RED = 'R';
	uchar YELLOW = 'Y';
	uchar BLUE = 'B';
	uchar GREEN = 'G';
	//init reliabilities
	long reliability_R,reliability_Y,reliability_B,reliability_G;
	reliability_B=reliability_G=reliability_R=reliability_Y=0;
	Mat res_Red,res_Yellow,res_Blue,res_Green;

	reliability_R = isRed(res_Red);
	//imshow("red",res_Red);
	//cout<<"R:  "<<reliability_R;
	reliability_Y=isYellow(res_Yellow);
	//imshow("yellow",res_Yellow);
	//cout<<"  Y:  "<<reliability_Y;
	reliability_B=isBlue(res_Blue);
	//imshow("blue",res_Blue);
	//cout<<"  B   "<<reliability_B;
	reliability_G=isGreen(res_Green);
	//imshow("green",res_Green);
	//cout<<"  G:   "<<reliability_G;
	long i=14000;
	if ( (reliability_R> i )||(reliability_Y> i )||(reliability_B> i )||(reliability_G> i ) )
	{
		long max=i;
		if (reliability_R>max)
			max=reliability_R;
		if (reliability_Y>max)
			max=reliability_Y;
		if (reliability_B>max)
			max=reliability_B;
		if (reliability_G>max)
			max=reliability_G;
		if (max==reliability_R)
		{
			cout<<" Red : "<<reliability_R<<endl;
			color = RED;
			return RED;
		}
		if (max==reliability_Y)
		{
			cout<<" Yellow : "<<reliability_Y<<endl;
			color = YELLOW;
			return YELLOW;
		}
		if (max==reliability_B)
		{
			cout<<" Blue : "<<reliability_B<<endl;
			color = BLUE;
			return BLUE;
		}
		if (max==reliability_G)
		{
			cout<<" Green : "<<reliability_G<<endl;
			color = GREEN;
			return GREEN;
		}
	}
	color = GREEN; //实在没办法了就乱造一个绿色
	return 'G';
}
long ZKBImage::isRed(Mat &res)
{
	Mat hsv;
	Mat chanel[3];
	Mat ch_result[3];
	cvtColor(color_img,hsv,CV_BGR2HSV);
	split(hsv,chanel);
	// 	imshow("h",chanel[0]);
	// 	imshow("s",chanel[1]);
	threshold(chanel[0],ch_result[0],165,255,THRESH_BINARY);	//H>165->ch_result[0]
	// 	imshow("H>165",ch_result[0]);
	threshold(chanel[0],ch_result[1],15,255,THRESH_BINARY_INV);	//H<15->ch_result[1]
	// 	imshow("H<15",ch_result[1]);
	threshold(chanel[1],ch_result[2],100,255,THRESH_BINARY);	//S>200->ch_result[2]
	// 	imshow("S>200",ch_result[2]);
	bitwise_or(ch_result[0],ch_result[1],ch_result[0]);			//(H>165)||(H<15) ->ch_result[0]
	// 	imshow("H<15,H>165",ch_result[0]);
	bitwise_and(ch_result[0],ch_result[2],ch_result[0]);		//((H>165)||(H<15)) && (S>200) ->ch_result[0]

	threshold(chanel[2],chanel[2],20,255,THRESH_BINARY);	//V>40->chanel[2]
	// 		imshow("V>20",chanel[2]);
	bitwise_and(chanel[2],ch_result[0],ch_result[0]);		//(H>45)&&(H<75) && (S>200) && (V>20) ->ch_result[0]
	erode(ch_result[0],res,Mat(5,5,CV_8UC1),Point(-1,-1),1);

	return countNonZero(res);
}
long ZKBImage::isYellow(Mat &res)
{
	Mat hsv;
	Mat chanel[3];
	Mat ch_result[3];
	cvtColor(color_img,hsv,CV_BGR2HSV);
	split(hsv,chanel);
	// 	imshow("h",chanel[0]);
	// 	imshow("s",chanel[1]);

	threshold(chanel[0],ch_result[0],15,255,THRESH_BINARY);	//H>15->ch_result[0]
	// 	imshow("H>15",ch_result[0]);
	threshold(chanel[0],ch_result[1],45,255,THRESH_BINARY_INV);	//H<45->ch_result[1]
	// 	imshow("H<45",ch_result[1]);
	threshold(chanel[1],ch_result[2],100,255,THRESH_BINARY);	//S>200 -> ch_result[2]
	// 	imshow("S>200",ch_result[2]);
	bitwise_and(ch_result[0],ch_result[1],ch_result[0]);			//(H>15)&&(H<45) ->ch_result[0]
	// 	imshow("H>15,H<45",ch_result[1]);
	bitwise_and(ch_result[0],ch_result[2],ch_result[0]);		//(H>15)&&(H<45) && (S>200) ->ch_result[0]

	threshold(chanel[2],chanel[2],20,255,THRESH_BINARY);	//V>20 -> chanel[2]
	// 		imshow("V>20",chanel[2]);
	bitwise_and(chanel[2],ch_result[0],ch_result[0]);		//(H>45)&&(H<75) && (S>200) && (V>20) ->ch_result[0]

	erode(ch_result[0],res,Mat(5,5,CV_8UC1),Point(-1,-1),1);

	return countNonZero(res);
}
long ZKBImage::isBlue(Mat &res)
{
	Mat hsv;
	Mat chanel[3];
	Mat ch_result[3];
	cvtColor(color_img,hsv,CV_BGR2HSV);
	split(hsv,chanel);
	// 	imshow("h",chanel[0]);
	// 	imshow("s",chanel[1]);
	threshold(chanel[0],ch_result[0],100,255,THRESH_BINARY);	//H>105
	// 	imshow("H>105",ch_result[0]);
	threshold(chanel[0],ch_result[1],130,255,THRESH_BINARY_INV);	//H<135
	// 	imshow("H<135",ch_result[1]);
	threshold(chanel[1],ch_result[2],120,255,THRESH_BINARY);	//S>200
	// 	imshow("S>200",ch_result[2]);
	bitwise_and(ch_result[0],ch_result[1],ch_result[0]);			//(H>105)&&(H<135) ->ch_result[0]

	// 	imshow("H>105,H<135",ch_result[0]);
	bitwise_and(ch_result[0],ch_result[2],ch_result[0]);		//(H>105)&&(H<135) && (S>200) ->ch_result[0]

	threshold(chanel[2],chanel[2],10,255,THRESH_BINARY);	//V>20;
	// 		imshow("V>20",chanel[2]);
	bitwise_and(chanel[2],ch_result[0],ch_result[0]);		//(H>45)&&(H<75) && (S>200) && (V>20) ->ch_result[0]

	erode(ch_result[0],res,Mat(3,3,CV_8UC1),Point(-1,-1),2);

	return countNonZero(res);
}
long ZKBImage::isGreen(Mat &res)
{
	Mat hsv,res_temp;
	Mat chanel[3];
	Mat ch_result[4];
	cvtColor(color_img,hsv,CV_BGR2HSV);
	split(hsv,chanel);
	// 	 	imshow("h",chanel[0]);
	// 	 	imshow("s",chanel[1]);
	// 		imshow("v",chanel[2]);
	threshold(chanel[0],ch_result[0],50,255,THRESH_BINARY);	//H>45
	// 		imshow("H>45",ch_result[0]);
	threshold(chanel[0],ch_result[1],70,255,THRESH_BINARY_INV);	//H<75
	// 		imshow("H<75",ch_result[1]);
	threshold(chanel[1],ch_result[2],80,255,THRESH_BINARY);	//S>200
	//imshow("S>200",ch_result[2]);
	bitwise_and(ch_result[0],ch_result[1],ch_result[0]);	//(H>45)&&(H<75) ->ch_result[0]
	//imshow("H>45,H<75",ch_result[0]);
	bitwise_and(ch_result[0],ch_result[2],ch_result[0]);	//(H>45)&&(H<75) && (S>200) ->ch_result[0]


	threshold(chanel[2],chanel[2],20,255,THRESH_BINARY);	//V>20;
	//imshow("V>20",chanel[2]);
	bitwise_and(chanel[2],ch_result[0],ch_result[0]);		//(H>45)&&(H<75) && (S>200) && (V>20) ->ch_result[0]


	//imshow("ch0",ch_result[0]);
	erode(ch_result[0],res_temp,Mat(4,4,CV_8UC1),Point(-1,-1),1);
	dilate(res_temp,res,Mat(6,6,CV_8UC1),Point(-1,-1),2);

	return countNonZero(res);
}