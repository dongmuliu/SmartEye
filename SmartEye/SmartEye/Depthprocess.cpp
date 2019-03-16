#include"Depthprocess.h"

Imagedepthprocess::Imagedepthprocess()
{
	_matimg_short.create(Img_height, Img_width, CV_16UC1);
	_matimg_show.create(Img_height, Img_width, CV_8UC1);
	 img_color.create(Img_height, Img_width, CV_8UC3);//����RGBͼ��
}
Imagedepthprocess::~Imagedepthprocess()
{

}
//������ݴ���
//���أ�mat����
Mat Imagedepthprocess::depthProcess()
{
	uint16_t fameDepthArray2[MAXLINE];
	for (int j = 0; j < bytecount / 2; j++)
	{
		raw_dep = ptr_buf_unsigned[j * 2 + 1] * 256 + ptr_buf_unsigned[2 * j];
		//cout << raw_dep << " ";
		realindex = bytecount / 2 - (j / Img_width + 1) * Img_width + j % Img_width;   //����
		realrow = Img_height - 1 - j / Img_width;
		realcol = j % Img_width;
		fameDepthArray2[realindex] = raw_dep;

	}
	uint16_t depth[240][320];
	for (int i = 0; i < 240; i++)
	{
		for (int j = 0; j < 320; j++)
		{
			depth[i][j] = fameDepthArray2[i * 320 + j];
		}
	}
	//16bitԭʼ����
	for (int i = 0; i < 240; i++)
	{
		for (int j = 0; j < 320; j++)
		{
			if (depth[i][j] > 30000)
			{
				depth[i][j] = 30000;
			}
			_matimg_short.at<ushort>(i, j) = depth[i][j];

		}

	}

	setColorImage();
	saveImage();

	return img_color.clone();
}
//��ȡ�������
//���أ�Mat����
Mat Imagedepthprocess::getDepth()
{
	return _matimg_short.clone();
}
//�˲�
//���룺ͼ����Ϣ��ָ��
void Imagedepthprocess::calibrate(ushort *img)
{
	//��ֵ�˲�
	imageAverageEightConnectivity(img);
	//�¶Ƚ���
	//calculationCorrectDRNU(img);
	//��Ȳ���
	calculationAddOffset(img);
}
//�˾�ֵ�˲�
//���룺 ���ͼ��ָ��
void  Imagedepthprocess::imageAverageEightConnectivity(ushort *depthdata)
{
	int pixelCounter;
	int nCols = 320;
	int nRowsPerHalf = 120;
	int size = 320 * 240;
	ushort actualFrame[MAX_NUM_PIX];
	int i, j, index;
	int pixdata;
	memcpy(actualFrame, depthdata, size * sizeof(ushort));
	// up side
	// dowm side
	// left side and right side
	// normal part
	for (i = 1; i < 239; i++) {
		for (j = 1; j < 319; j++){
			index = i * 320 + j;
			pixelCounter = 0;
			pixdata = 0;
			if (actualFrame[index] < 30000) {
				pixelCounter++;
				pixdata += actualFrame[index];
			}
			if (actualFrame[index - 1]  < 30000) {   // left
				pixdata += actualFrame[index - 1];
				pixelCounter++;
			}
			if (actualFrame[index + 1]  < 30000) {   // right
				pixdata += actualFrame[index + 1];
				pixelCounter++;
			}
			if (actualFrame[index - 321]  < 30000) {   // left up
				pixdata += actualFrame[index - 321];
				pixelCounter++;
			}
			if (actualFrame[index - 320]  < 30000) {   // up
				pixdata += actualFrame[index - 320];
				pixelCounter++;
			}
			if (actualFrame[index - 319]  < 30000) {   // right up
				pixdata += actualFrame[index - 319];
				pixelCounter++;
			}
			if (actualFrame[index + 319]  < 30000) {   // left down
				pixdata += actualFrame[index - 321];
				pixelCounter++;
			}
			if (actualFrame[index + 320]  < 30000) {   // down
				pixdata += actualFrame[index - 320];
				pixelCounter++;
			}
			if (actualFrame[index + 321]  < 30000) {   // right down
				pixdata += actualFrame[index - 319];
				pixelCounter++;
			}
			//�����Χ��Ч����С��6��Ϊ��Ч��
			if (pixelCounter < 6) {
				*(depthdata + index) = LOW_AMPLITUDE;
			}
			else {
				*(depthdata + index) = pixdata / pixelCounter;
			}
		}
	}

}
//��Ȳ���
//���룺ͼ����Ϣָ��
void  Imagedepthprocess::calculationAddOffset(ushort *img)
{
	int offset = 0;
	uint16_t maxDistanceCM = 0;
	int l = 0;
	offset = OFFSET_PHASE_DEFAULT;
	uint16_t val;
	uint16_t *pMem = img;
	int numPix = 320 * 240;

	for (l = 0; l<numPix; l++){
		val = pMem[l];
		if (val < 30000) { //if not low amplitude, not saturated and not  adc overflow
			pMem[l] = (val + offset) % MAX_DIST_VALUE;
		}
	}
}
//�¶�У��
//���룺ͼ����Ϣָ��
//���أ�0
int Imagedepthprocess::calculationCorrectDRNU(ushort *img)
{
	//int		gTempCal_Temp = 0;
	//double	gStepCalMM = 0;

	//int i, x, y, l;
	//double dist, tempDiff = 0;
	//int offset = 0;
	////printf("gOffsetDRNU = %d\n", offset);  //w
	//uint16_t maxDistanceMM = 150000000 / 12000;
	//double stepLSB = gStepCalMM * MAX_PHASE / maxDistanceMM;
	////printf("stepLSB = %2.4f\n", stepLSB);  //w

	//uint16_t *pMem = img;
	//int width = 320;
	//int height = 240;
	//tempDiff = realTempChip - gTempCal_Temp;
	//for (l = 0, y = 0; y< height; y++){
	//	for (x = 0; x < width; x++, l++){
	//		dist = pMem[l];

	//		if (dist < LOW_AMPLITUDE){	//correct if not saturated
	//			dist -= offset;

	//			if (dist<0)	//if phase jump
	//				dist += MAX_DIST_VALUE;

	//			i = (int)(round(dist / stepLSB));

	//			if (i<0) i = 0;
	//			else if (i >= 50) i = 49;

	//			dist = (double)pMem[l] - drnuLut[i][y][x];

	//			dist -= tempDiff * 3.12262;	// 0.312262 = 15.6131 / 50

	//			pMem[l] = (uint16_t)round(dist);

	//		}	//end if LOW_AMPLITUDE
	//	}	//end width
	//}	//end height
	////printf(" pMem = %d, %d, %d\n", pMem[1300], pMem[1301], pMem[1302]);
	return 0;
}
//����α��ɫ����
void Imagedepthprocess::setColorImage()
{
	Mat depthzip = _matimg_short.clone();
	double interdepth = 255.0 / (maxdepth - mindepth);
	//����ѹ����0��255�ռ�
	for (int i = 0; i < 240; i++)
	{
		for (int j = 0; j < 320; j++)
		{
			if (depthzip.at<ushort>(i, j) > maxdepth)
			{
				depthzip.at<ushort>(i, j) = maxdepth;
			}
			else if (depthzip.at<ushort>(i, j) < mindepth)
			{
				depthzip.at<ushort>(i, j) = mindepth;
			}
			_matimg_show.at<uchar>(i, j) = (uchar)((depthzip.at<ushort>(i, j) - mindepth)*interdepth);
		}
	}
	//���ݾ�������α��ɫ
	
	ushort img_tmp;
	for (int y = 0; y < Img_height; y++)
	{
		for (int x = 0; x < Img_width; x++)
		{
			img_tmp = _matimg_show.at<uchar>(y, x);
			if (img_tmp < 51)
			{
				IMG_B(img_color, y, x) = 0;
				IMG_G(img_color, y, x) = img_tmp * 5;
				IMG_R(img_color, y, x) = 0;
			}
			else if (img_tmp < 102)
			{
				img_tmp -= 51;
				IMG_B(img_color, y, x) = 255 - img_tmp * 5;
				IMG_G(img_color, y, x) = 255;
				IMG_R(img_color, y, x) = 0;
			}
			else if (img_tmp < 153)
			{
				img_tmp -= 102;
				IMG_B(img_color, y, x) = 0;
				IMG_G(img_color, y, x) = 255;
				IMG_R(img_color, y, x) = img_tmp * 5;
			}
			else if (img_tmp <= 204)
			{
				img_tmp -= 153;
				IMG_B(img_color, y, x) = 0;
				IMG_G(img_color, y, x) = 255 - uchar(128.0*img_tmp / 51 + 0.5);
				IMG_R(img_color, y, x) = 255;
			}
			else
			{
				img_tmp -= 204;
				IMG_B(img_color, y, x) = 0;
				IMG_G(img_color, y, x) = 127 - uchar(127.0*img_tmp / 51 + 0.5);
				IMG_R(img_color, y, x) = 255;
			}
		}
	}
	
}
//�������ͼ
void Imagedepthprocess::saveImage()
{
	string fileassave = string(savestr.toLocal8Bit());
	if (saveimagestate == 1)
	{
		imwrite(fileassave+"/"+to_string(imagecount) + ".png", _matimg_short);
		imagecount++;
	}
	else
	{
		imagecount = 0;
	}
}