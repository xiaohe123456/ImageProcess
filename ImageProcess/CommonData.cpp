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

string OriginalWindowName = "ImageShowOriginal";  //显示原图像窗口
string MiddleWindowName1 = "ImageShowMiddle1";    //显示中间结果窗口1
string MiddleWindowName2 = "ImageShowMiddle2";    //显示中间结果窗口2

string pathName;								//读取图像时的图像路径及图像文件名

char* str = "../operation.yaml";				//yaml文件名
FileStorage fs_write(str,FileStorage::WRITE);

int flagFilter = 0;
int flagEnhance = 0;
int flagGeoTransform = 0;
int flagMouse = 0;
int middleWindow1 = 0;
int middleWindow2 = 0;

int countL = 0;			//透视变换时统计点的数目  鼠标左键
int countR = 0;			//仿射变换时统计点的数目  鼠标右键

Point2f srcPoints[4];	//透视变换的源点,一个包含三维点（x，y）的数组，其中x、y是浮点型数
Point2f dstPoints[4];	//透视变换的目的点 
Point2f srcPointsP[4];	//透视变换的源点
Point2f srcPointsA[3];	//仿射变换的源点 ,一个包含三维点（x，y）的数组，其中x、y是浮点型数
Point2f dstPointsA[3];	//仿射变换的目的点  


int before_x = 0;
int before_y = 0;
int current_x = 0;
int current_y = 0;
int tx = 0;
int ty = 0;


