#pragma once
#include "pch.h"
#include "yaml-cpp/yaml.h"
#include "resource.h"

extern Mat srcImage;					//原图像
extern Mat middleImage1;				//中间结果1
extern Mat middleImage2;				//中间结果2
extern Mat dstImage;					//目的结果图像 未使用
extern Mat imageFilter;					//图像滤波结果
extern Mat imageEnhance;				//图像增强结果	
extern string OriginalWindowName;		//显示原图像窗口
extern string MiddleWindowName1 ;		//显示中间结果窗口1
extern string MiddleWindowName2 ;		//显示中间结果窗口2

extern string pathName;		//读取图像的路径及文件名
extern CString str;			//yaml文件名

extern CFile file;			//保存yaml文件定义的文件参数  当前未使用

extern int flagFilter;			//图像滤波标志位
extern int flagEnhance;			//图像增强标志位
extern int flagGeoTransform;	//图像几何变换标志位
extern int flagMouse;			//鼠标缩放事件标志，避免鼠标移动时图像显示出错
extern int middleWindow1;		//统计中间窗口1显示结果的次数
extern int middleWindow2;		//统计中间窗口2显示结果的次数

extern YAML::Node config;		//yaml文件中的结点，保存透视变换时的四个源点坐标
extern ofstream fout;			//将图像处理步骤保存到yaml文件定义的文件流

extern Point2f srcPoints[4];		//原图中的四点,一个包含三维点（x，y）的数组
extern Point2f dstPoints[4];		//变换中的目标点 
extern Point2f srcPointsP[4];		//透视变换中的源点
extern Point2f srcPointsA[3];		//仿射变换原图中的三点 ,一个包含三维点（x，y）的数组
extern Point2f dstPointsA[3];		//目标图中的三点  

extern int countL;					//透视变换时统计点的数目  鼠标左键
extern int countR;					//仿射变换时统计点的数目  鼠标右键

void on_mouse(int event, int x, int y, int flags, void* ustc);   //鼠标事件  获取坐标
void onMouse(int event, int x, int y, int flags, void* ustc);    //鼠标事件  实现鼠标拖动图像移动  未使用
extern int before_x;	//移动前的坐标
extern int before_y;
extern int current_x;	//移动后当前位置坐标
extern int current_y;
extern int tx;			//平移距离
extern int ty;

