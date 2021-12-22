#pragma once
#include "pch.h"
#include "yaml-cpp/yaml.h"
#include "resource.h"

extern Mat srcImage;					//ԭͼ��
extern Mat middleImage1;				//�м���1
extern Mat middleImage2;				//�м���2
extern Mat dstImage;					//Ŀ�Ľ��ͼ�� δʹ��
extern Mat imageFilter;					//ͼ���˲����
extern Mat imageEnhance;				//ͼ����ǿ���	
extern string OriginalWindowName;		//��ʾԭͼ�񴰿�
extern string MiddleWindowName1 ;		//��ʾ�м�������1
extern string MiddleWindowName2 ;		//��ʾ�м�������2

extern string pathName;		//��ȡͼ���·�����ļ���
extern CString str;			//yaml�ļ���

extern CFile file;			//����yaml�ļ�������ļ�����  ��ǰδʹ��

extern int flagFilter;			//ͼ���˲���־λ
extern int flagEnhance;			//ͼ����ǿ��־λ
extern int flagGeoTransform;	//ͼ�񼸺α任��־λ
extern int flagMouse;			//��������¼���־����������ƶ�ʱͼ����ʾ����
extern int middleWindow1;		//ͳ���м䴰��1��ʾ����Ĵ���
extern int middleWindow2;		//ͳ���м䴰��2��ʾ����Ĵ���

extern YAML::Node config;		//yaml�ļ��еĽ�㣬����͸�ӱ任ʱ���ĸ�Դ������
extern ofstream fout;			//��ͼ�����豣�浽yaml�ļ�������ļ���

extern Point2f srcPoints[4];		//ԭͼ�е��ĵ�,һ��������ά�㣨x��y��������
extern Point2f dstPoints[4];		//�任�е�Ŀ��� 
extern Point2f srcPointsP[4];		//͸�ӱ任�е�Դ��
extern Point2f srcPointsA[3];		//����任ԭͼ�е����� ,һ��������ά�㣨x��y��������
extern Point2f dstPointsA[3];		//Ŀ��ͼ�е�����  

extern int countL;					//͸�ӱ任ʱͳ�Ƶ����Ŀ  ������
extern int countR;					//����任ʱͳ�Ƶ����Ŀ  ����Ҽ�

void on_mouse(int event, int x, int y, int flags, void* ustc);   //����¼�  ��ȡ����
void onMouse(int event, int x, int y, int flags, void* ustc);    //����¼�  ʵ������϶�ͼ���ƶ�  δʹ��
extern int before_x;	//�ƶ�ǰ������
extern int before_y;
extern int current_x;	//�ƶ���ǰλ������
extern int current_y;
extern int tx;			//ƽ�ƾ���
extern int ty;

