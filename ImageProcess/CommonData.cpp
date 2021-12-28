#include "pch.h"
#include "CommonData.h"


Mat srcImage;
Mat middleImage1;
Mat middleImage2;
Mat dstImage;
Mat imageFilter;				
Mat imageEnhance;	

vector<Mat> resultMidWindow1;
vector<Mat> resultMidWindow2;

string OriginalWindowName = "ImageShowOriginal";  //��ʾԭͼ�񴰿�
string MiddleWindowName1 = "ImageShowMiddle1";    //��ʾ�м�������1
string MiddleWindowName2 = "ImageShowMiddle2";    //��ʾ�м�������2

string pathName;								//��ȡͼ��ʱ��ͼ��·����ͼ���ļ���

char* str = "../operation.yaml";				//yaml�ļ���
FileStorage fs_write(str,FileStorage::WRITE);

int flagFilter = 0;
int flagEnhance = 0;
int flagGeoTransform = 0;
int flagMouse = 0;
int middleWindow1 = 0;
int middleWindow2 = 0;

int countL = 0;			//͸�ӱ任ʱͳ�Ƶ����Ŀ  ������
int countR = 0;			//����任ʱͳ�Ƶ����Ŀ  ����Ҽ�

Point2f srcPoints[4];	//͸�ӱ任��Դ��,һ��������ά�㣨x��y�������飬����x��y�Ǹ�������
Point2f dstPoints[4];	//͸�ӱ任��Ŀ�ĵ� 
Point2f srcPointsP[4];	//͸�ӱ任��Դ��
Point2f srcPointsA[3];	//����任��Դ�� ,һ��������ά�㣨x��y�������飬����x��y�Ǹ�������
Point2f dstPointsA[3];	//����任��Ŀ�ĵ�  


int before_x = 0;
int before_y = 0;
int current_x = 0;
int current_y = 0;
int tx = 0;
int ty = 0;


