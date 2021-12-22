﻿// ImageEnhance.cpp: 实现文件
//

#include "pch.h"
#include "ImageProcess.h"
#include "ImageEnhance.h"
#include "afxdialogex.h"
#include "CommonData.h"

// ImageEnhance 对话框

IMPLEMENT_DYNAMIC(ImageEnhance, CDialogEx)

ImageEnhance::ImageEnhance(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGImageEnhance, pParent)
{

}

ImageEnhance::~ImageEnhance()
{
}

void ImageEnhance::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOK, LineK);
	DDX_Control(pDX, IDC_COMBOB, LineB);
	DDX_Control(pDX, IDC_COMBOCofficient, GammaCof);
	DDX_Control(pDX, IDC_COMBOIndex, GammaIndex);
	DDX_Control(pDX, IDC_COMBOContrastThresh, ContrastThresh);
	DDX_Control(pDX, IDC_COMBOGrid, EqualizeGrid);
	DDX_Control(pDX, IDC_COMBOLogC, LogC);
	DDX_Control(pDX, IDC_COMBOSegLineK1, SegLineK1);
	DDX_Control(pDX, IDC_COMBOSegLineB1, SegLineB1);
	DDX_Control(pDX, IDC_COMBOSegLineK2, SegLineK2);
	DDX_Control(pDX, IDC_COMBOSegLineB2, SegLineB2);
	DDX_Control(pDX, IDC_COMBOSegLineGray1, SegLineGray1);
	DDX_Control(pDX, IDC_COMBOSegLineGray2, SegLineGray2);
}



BEGIN_MESSAGE_MAP(ImageEnhance, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONLineEnhance, &ImageEnhance::OnBnClickedButtonlineenhance)
	ON_BN_CLICKED(IDC_BUTTONGammaEnhance, &ImageEnhance::OnBnClickedButtongammaenhance)

	ON_BN_CLICKED(IDC_BUTTONEquHist, &ImageEnhance::OnBnClickedButtonequhist)
	ON_BN_CLICKED(IDC_BUTTONLogEnhance, &ImageEnhance::OnBnClickedButtonlogenhance)
	ON_BN_CLICKED(IDC_BUTTONSegLine, &ImageEnhance::OnBnClickedButtonsegline)
	ON_BN_CLICKED(IDC_BUTTONCLAHE, &ImageEnhance::OnBnClickedButtonclahe)
END_MESSAGE_MAP()



BOOL ImageEnhance::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	int i = 0;
	//设置方框、均值和中值滤波器核控件的大小
	while (i < 255)
	{
		str.Format(_T("%d"), i);
		LineB.AddString(str);				//线性变换B
		ContrastThresh.AddString(str);		//自适应直方图均衡化对比度阈值
		EqualizeGrid.AddString(str);		//自适应直方图均衡化网格
		SegLineB1.AddString(str);			//分段线性变换B1
		SegLineB2.AddString(str);			//分段线性变换B2
		SegLineGray1.AddString(str);		//分段线性变换分段值1
		SegLineGray2.AddString(str);		//分段线性变换分段值2
		i += 1;
	}
	LineB.SetCurSel(0);
	SegLineB1.SetCurSel(0);
	SegLineB2.SetCurSel(0);
	ContrastThresh.SetCurSel(0);
	EqualizeGrid.SetCurSel(0);
	SegLineGray1.SetCurSel(0);
	SegLineGray2.SetCurSel(0);
	int k1;
	for (k1 = -10; k1 < 11; k1++)
	{
		str.Format(_T("%d"), k1);
		LineK.AddString(str);				//线性变换K             
		SegLineK1.AddString(str);			//分段线性变换K1
		SegLineK2.AddString(str);			//分段线性变换K2
	}
	double k2 = -0.9;
	while (k2 < 1)
	{
		str.Format(_T("%f"), k2);
		LineK.AddString(str);				//线性变换K
		SegLineK1.AddString(str);			//分段线性变换K1
		SegLineK2.AddString(str);			//分段线性变换K2
		k2 += 0.1;
	}
	LineK.SetCurSel(10);
	SegLineK1.SetCurSel(10);
	SegLineK2.SetCurSel(10);
	double gamma1 = 0.0;
	while (gamma1 < 1)
	{
		str.Format(_T("%f"), gamma1);
		GammaIndex.AddString(str);			//伽马变换指数 小于1
		gamma1 += 0.1;						
	}
	int gamma2 = 1;
	while (gamma2 <= 10)
	{
		str.Format(_T("%d"), gamma2);
		GammaIndex.AddString(str);			//伽马变换指数  大于1小于10
		GammaCof.AddString(str);			//伽马变换系数   一般设为1
		LogC.AddString(str);				//对数变换系数
		gamma2 += 1;
	}
	GammaCof.SetCurSel(0);
	GammaIndex.SetCurSel(10);
	LogC.SetCurSel(0);

	return 0;
}


// ImageEnhance 消息处理程序
//线性增强
void ImageEnhanceLine(Mat src, Mat& dst, double kk, double bb)
{
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			if (src.channels() == 3)
			{
				for (int c = 0; c < 3; c++)
					dst.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(kk * (src.at<Vec3b>(i, j)[c]) + bb);

			}
		}
	}

}

//图像对比度增强 （线性）
void ImageEnhance::OnBnClickedButtonlineenhance()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	CString k;  // 线性变换斜率  调整对比度  大于0对比度增大  小于0对比度减小
	CString b;  //截距  调整亮度  大于0增加亮度   小于0减小亮度
	int index;
	index = LineB.GetCurSel();
	LineB.GetLBText(index, k);
	index = LineK.GetCurSel();
	LineK.GetLBText(index, b);
	double kk = _ttof(k);  //CString 转为double类型
	double bb = _ttof(b);
	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if (middleWindow1 == 0 && middleWindow2 == 0)
	{
		middleImage1 = Mat::zeros(Size(srcImage.cols, srcImage.rows), srcImage.type());
		ImageEnhanceLine(srcImage, middleImage1, kk, bb);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		fout << "ImageEnhanceLine(srcImage, middleImage1, " << kk << ", " << bb << ");\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if (middleWindow2 >= middleWindow1)
	{
		if (flagEnhance == 1 && flagFilter == 0)
			middleImage2 = srcImage;
		if (flagEnhance == 1 && flagFilter == 1)
			middleImage2 = imageFilter;
		middleImage1 = Mat::zeros(Size(srcImage.cols, srcImage.rows), srcImage.type());
		ImageEnhanceLine(middleImage2, middleImage1, kk, bb);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		//middleImage2.rows = 0;

		fout << "ImageEnhanceLine(middleImage2, middleImage1, " << kk << ", " << bb << ");\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if (middleWindow1 > middleWindow2)
	{
		if (flagEnhance == 1 && flagFilter == 0)
			middleImage1 = srcImage;
		if (flagEnhance == 1 && flagFilter == 1)
			middleImage1 = imageFilter;
		middleImage2 = Mat::zeros(Size(srcImage.cols, srcImage.rows), srcImage.type());
		ImageEnhanceLine(middleImage1, middleImage2, kk, bb);
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		//middleImage1.rows = 0;

		fout << "ImageEnhanceLine(middleImage1, middleImage2, " << kk << ", " << bb << ");\n";
	}
	flagEnhance = 1;
}


void GammaEnhance(Mat src, Mat& dst, double dc, double dGamma)
{
	src.convertTo(dst, CV_64F, 1.0 / 255, 0);  //归一化
	pow(dst, dGamma, dst);

}

/*
* 伽马变换对于图像对比度偏低，并且整体亮度值偏高的情况图像增强效果明显，通过调节gamma值
* gamma<1时提高图像暗区域的对比度，而降低亮区域的对比度；gamma>1时提高亮区域的对比度，降低暗区域的对比度
* 对于灰度级整体偏暗的图像，可以用gamma<1增大灰度动态范围，对于灰度级整体偏亮的图像，可以用gamma>1增大灰度动态范围
*/
void ImageEnhance::OnBnClickedButtongammaenhance()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	CString c;
	CString g;
	int index;
	index = GammaCof.GetCurSel();
	GammaCof.GetLBText(index, c);
	index = GammaIndex.GetCurSel();
	GammaIndex.GetLBText(index, g);
	double dc = _ttof(c);     //伽马变换系数
	double dGamma = _ttof(g);   //指数

	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if(middleWindow1 == 0 && middleWindow2 ==0)
	{
		GammaEnhance(srcImage, middleImage1, dc, dGamma);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		fout << "GammaEnhance(srcImage, middleImage1, " << dc << ", " << dGamma << ");\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if (middleWindow2 >= middleWindow1)
	{
		if (flagEnhance == 1 && flagFilter == 0)
			middleImage2 = srcImage;
		if (flagEnhance == 1 && flagFilter == 1)
			middleImage2 = imageFilter;
		GammaEnhance(middleImage2, middleImage1, dc, dGamma);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		//middleImage2.rows = 0;

		fout << "GammaEnhance(middleImage2, middleImage1, " << dc << ", " << dGamma << ");\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if(middleWindow1 > middleWindow2)
	{
		if (flagEnhance == 1 && flagFilter == 0)
			middleImage1 = srcImage;
		if (flagEnhance == 1 && flagFilter == 1)
			middleImage1 = imageFilter;
		GammaEnhance(middleImage1, middleImage2, dc, dGamma);
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		//middleImage1.rows = 0;

		fout << "GammaEnhance(middleImage1, middleImage2, " << dc << ", " << dGamma << ");\n";
	}
	flagEnhance = 1;
}

//直方图均衡化
void ImageEnhance::OnBnClickedButtonequhist()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if (middleWindow1 == 0 && middleWindow2 == 0)
	{
		if (srcImage.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		cvtColor(srcImage, middleImage1, CV_BGR2GRAY);
		equalizeHist(middleImage1, middleImage1);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		fout << "//直方图均衡化\n";						//注释
		fout << "equalizeHist(middleImage1, middleImage1);\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if(middleWindow2 >= middleWindow1)
	{
		if (flagEnhance == 1 && flagFilter == 0)
			middleImage2 = srcImage;
		if (flagEnhance == 1 && flagFilter == 1)
			middleImage2 = imageFilter;
		if (middleImage2.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		cvtColor(middleImage2, middleImage2, CV_BGR2GRAY);
		equalizeHist(middleImage2, middleImage1);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		//middleImage2.rows = 0;
		fout << "//直方图均衡化\n";						//注释
		fout << "equalizeHist(middleImage1, middleImage1);\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if (middleWindow1 > middleWindow2)
	{
		if (flagEnhance == 1 && flagFilter == 0)
			middleImage1 = srcImage;
		if (flagEnhance == 1 && flagFilter == 1)
			middleImage1 = imageFilter;
		if (middleImage1.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		cvtColor(middleImage1, middleImage1, CV_BGR2GRAY);
		equalizeHist(middleImage1, middleImage2);
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		//middleImage1.rows = 0;
		fout << "//直方图均衡化\n";						//注释
		fout << "equalizeHist(middleImage2, middleImage2);\n";
	}
	flagEnhance = 1;
}


void LogEnhance(Mat src, Mat& dst, double c)
{
	add(src, Scalar(1.0), src);
	src.convertTo(src, CV_32F);
	log(src, dst);
	dst = c * dst;
	normalize(dst, dst, 0, 255, NORM_MINMAX);
	convertScaleAbs(dst, dst);
}

//对数变化增强
void ImageEnhance::OnBnClickedButtonlogenhance()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	CString c;
	int index = LogC.GetCurSel();
	LogC.GetLBText(index, c);
	double dc = _ttof(c);     //对数变换系数
	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if (middleWindow1 == 0 && middleWindow2 == 0)
	{
		middleImage1 = Mat::zeros(Size(srcImage.rows, srcImage.cols), srcImage.type());
		LogEnhance(srcImage, middleImage1, dc);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		fout << "LogEnhance(srcImage, middleImage1, " << dc << ");\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if(middleWindow2 >= middleWindow1)
	{
		if (flagEnhance == 1 && flagFilter == 0)
			middleImage2 = srcImage;
		if (flagEnhance == 1 && flagFilter == 1)
			middleImage2 = imageFilter;
		LogEnhance(middleImage2, middleImage1, dc);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		//middleImage2.rows = 0;

		fout << "LogEnhance(middleImage2, middleImage1, " << dc << ");\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if(middleWindow1 > middleWindow2)
	{
		if (flagEnhance == 1 && flagFilter == 0)
			middleImage1 = srcImage;
		if (flagEnhance == 1 && flagFilter == 1)
			middleImage1 = imageFilter;
		LogEnhance(middleImage1, middleImage2, dc);
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		//middleImage1.rows = 0;

		fout << "LogEnhance(middleImage1, middleImage2, " << dc << ");\n";
	}
	flagEnhance = 1;
}


void SegLineEnhance(Mat src, Mat& dst, double k1, double k2, double b1, double b2, double seg1, double seg2)
{
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			if (src.at<Vec3b>(i, j)[0] < seg1)
				dst.at<Vec3b>(i, j) = saturate_cast<uchar>(k1 * (src.at<Vec3b>(i, j)[0]) + b1);
			else if (src.at<Vec3b>(i, j)[0] < seg2 && src.at<Vec3b>(i, j)[0] >= seg1)
				dst.at<Vec3b>(i, j) = saturate_cast<uchar>(k2 * (src.at<Vec3b>(i, j)[0]) + b2);
			else
				dst.at<Vec3b>(i, j) = src.at<Vec3b>(i, j);
		}
	}
}

//分段线性增强
void ImageEnhance::OnBnClickedButtonsegline()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	CString k1, b1;
	CString k2, b2;
	CString seg1, seg2;
	int index;
	index = SegLineK1.GetCurSel();
	SegLineK1.GetLBText(index, k1);
	index = SegLineB1.GetCurSel();
	SegLineB1.GetLBText(index, b1);
	index = SegLineK2.GetCurSel();
	SegLineK2.GetLBText(index, k2);
	index = SegLineB2.GetCurSel();
	SegLineB2.GetLBText(index, b2);
	index = SegLineGray1.GetCurSel();
	SegLineGray1.GetLBText(index, seg1);
	index = SegLineGray2.GetCurSel();
	SegLineGray2.GetLBText(index, seg2);

	double dk1 = _ttof(k1);     //分段变换斜率k1
	double db1 = _ttof(b1);     //分段变换焦距b1
	double dk2 = _ttof(k2);     //分段变换斜率k2
	double db2 = _ttof(b2);     //分段变换焦距b2
	double dSeg1 = _ttof(seg1);     //分段变换分段值1
	double dSeg2 = _ttof(seg2);     //分段变换分段值2

	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if (middleWindow1 ==0 && middleWindow2 == 0)
	{
		middleImage1 = Mat::zeros(Size(srcImage.cols, srcImage.rows), srcImage.type());
		SegLineEnhance(srcImage, middleImage1, dk1, dk2, db1, db2, dSeg1, dSeg2);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		fout << "SegLineEnhance(srcImage, middleImage1, " << dk1 << ", " << dk2 << ", " << db1 << ", " << db2 << ", " << dSeg1 << ", " << dSeg2 << ");\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if (middleWindow2 >= middleWindow1)
	{
		if (flagEnhance == 1 && flagFilter == 0)
			middleImage2 = srcImage;
		if (flagEnhance == 1 && flagFilter == 1)
			middleImage2 = imageFilter;
		middleImage1 = Mat::zeros(Size(middleImage2.cols, middleImage2.rows), srcImage.type());
		SegLineEnhance(middleImage2, middleImage1, dk1, dk2, db1, db2, dSeg1, dSeg2);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		//middleImage2.rows = 0;

		fout << "SegLineEnhance(middleImage2, middleImage1, " << dk1 << ", " << dk2 << ", " << db1 << ", " << db2 << ", " << dSeg1 << ", " << dSeg2 << ");\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if (middleWindow1 > middleWindow2)
	{
		if (flagEnhance == 1 && flagFilter == 0)
			middleImage1 = srcImage;
		if (flagEnhance == 1 && flagFilter == 1)
			middleImage1 = imageFilter;
		middleImage2 = Mat::zeros(Size(middleImage1.cols, middleImage1.rows), srcImage.type());
		SegLineEnhance(middleImage1, middleImage2, dk1, dk2, db1, db2, dSeg1, dSeg2);
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		//middleImage1.rows = 0;

		fout << "SegLineEnhance(middleImage1, middleImage2, " << dk1 << ", " << dk2 << ", " << db1 << ", " << db2 << ", " << dSeg1 << ", " << dSeg2 << ");\n";
	}
	flagEnhance = 1;
}

//自适应直方图均衡化
void ImageEnhance::OnBnClickedButtonclahe()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	CString threshold;
	CString grid;
	int index;
	index = ContrastThresh.GetCurSel();
	ContrastThresh.GetLBText(index, threshold);
	index = EqualizeGrid.GetCurSel();
	EqualizeGrid.GetLBText(index, grid);
	double dThreshold = _ttof(threshold);     //对比度阈值
	double dGrid = _ttof(grid);     //对比度阈值
	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if (middleWindow1 == 0 && middleWindow2 == 0)
	{
		if (srcImage.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		cvtColor(srcImage, srcImage, CV_BGR2GRAY);
		Ptr<CLAHE> clathe = createCLAHE(dThreshold, Size(dGrid, dGrid));
		clathe->apply(srcImage, middleImage1);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		fout << "//自适应直方图均衡化\n";  //注释
		fout << "Ptr<CLAHE> clathe = createCLAHE(" << dThreshold << ", Size(" << dGrid << ", " << dGrid << "));\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if (middleWindow2 >= middleWindow1)
	{
		if (flagEnhance == 1 && flagFilter == 0)
			middleImage2 = srcImage;
		if (flagEnhance == 1 && flagFilter == 1)
			middleImage2 = imageFilter;
		if (middleImage2.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		cvtColor(middleImage2, middleImage2, CV_BGR2GRAY);
		Ptr<CLAHE> clathe = createCLAHE(dThreshold, Size(dGrid, dGrid));
		clathe->apply(middleImage2, middleImage1);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		//middleImage2.rows = 0;

		fout << "//自适应直方图均衡化\n";  //注释
		fout << "Ptr<CLAHE> clathe = createCLAHE(" << dThreshold << ", Size(" << dGrid << ", " << dGrid << "));\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if(middleWindow1 > middleWindow2)
	{
		if (flagEnhance == 1 && flagFilter == 0)
			middleImage1 = srcImage;
		if (flagEnhance == 1 && flagFilter == 1)
			middleImage1 = imageFilter;
		if (middleImage1.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		cvtColor(middleImage1, middleImage1, CV_BGR2GRAY);
		Ptr<CLAHE> clathe = createCLAHE(dThreshold, Size(dGrid, dGrid));
		clathe->apply(middleImage1, middleImage2);
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		//middleImage1.rows = 0;

		fout << "//自适应直方图均衡化\n";  //注释
		fout << "Ptr<CLAHE> clathe = createCLAHE(" << dThreshold << ", Size(" << dGrid << ", " << dGrid << "));\n";
	}
	flagEnhance = 1;
}
