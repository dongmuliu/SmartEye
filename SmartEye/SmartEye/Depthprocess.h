#pragma once

#include <stdio.h>
#include <stdint.h>
#include <opencv2\opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <qmessagebox.h>
using namespace std;

#define bytecount 153600
#define Img_width   (320)
#define Img_height  (240)
#define MAXLINE 307200   
#define MAX_NUM_PIX	82656	//328 * 252
#define LOW_AMPLITUDE_V26 	32500   //强度过低值
#define OVER_EXPOSED_V26	32700	//过曝值
#define LOW_AMPLITUDE_V212	65300
#define OVER_EXPOSED_V212	65500
#define MAX_PHASE       30000.0 //温度矫正
#define MAX_DIST_VALUE 	30000 //最远距离值
#define OFFSET_PHASE_DEFAULT 0  //深度补偿值
#define IMG_B(img,y,x) img.at<Vec3b>(y,x)[0]
#define IMG_G(img,y,x) img.at<Vec3b>(y,x)[1]
#define IMG_R(img,y,x) img.at<Vec3b>(y,x)[2]

class Imagedepthprocess
{
public:
	Imagedepthprocess();
	~Imagedepthprocess();
	cv::Mat depthProcess();
	cv::Mat getDepth();
	unsigned char* ptr_buf_unsigned;
	cv::Mat _matimg_short;			//16bit数据
	cv::Mat _matimg_show;			//16bit数据
	cv::Mat _matimg_amp;			//16bit数据
	cv::Mat  img_color;				//伪彩色数据
	cv::Mat  img_amp;				//强度数据
	ushort realTempChip;			//相机温度
	int maxdepth=30000;				//映射最远距离    
	int mindepth=0;					//映射最近距离
	int saveimagestate = 0;			//保存图片状态
	int imagecount = 0;				//保存图片序号
	QString  savestr;				//保存路径
	QString saveAmpstr;
	ushort version;					//相机版本，主要伪彩色处理
	bool isHorizontalFlip = 0;		//图片水平翻转标志
	bool isVerticalFlip = 0;		//图片垂直翻转标志	
	bool isAmp = false;				//是否显示强度图
	bool isRawCalibration = false;		//开启畸变校正标志
	bool isHDR = false;					//是否开启HDR

	int offset = 0;					//深度偏移量
	int maxAmp = 500;
	void setConvertParameter(double fx, double fy, double cx, double cy, double k1, double k2, double p1, double p2, double k3);	//设置转换参数

private:
	void calibrate(ushort *img);							//滤波
	void imageAverageEightConnectivity(ushort *depthdata);  //均值滤波
	void calculationAddOffset(ushort *img);					//深度补偿
	int calculationCorrectDRNU(ushort * img);				//温度矫正
	void setColorImage();									//设置伪彩色图
	void setAmpImg(int maxAmp);
	void saveImage();										//保存深度图
	void imageInpainting();									//修复HDR图
	void undistImg(cv::Mat src, cv::Mat& dst);								//畸变校正
	uint16_t raw_dep;
	int realindex, realrow, realcol;
	//int		drnuLut[50][252][328];						//温度矫正用表
	//畸变校正参数
	double fx;
	double fy;
	double cx;
	double cy;
	double k1;
	double k2;
	double p1;
	double p2;
	double k3;
	cv::Mat map1, map2;										//畸变映射矩阵
	cv::Size imgSize = cv::Size(Img_width, Img_height);

};