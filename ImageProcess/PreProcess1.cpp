// PreProcess1.cpp: 实现文件
//

#include "pch.h"
#include "ImageProcess.h"
#include "PreProcess1.h"
#include "afxdialogex.h"
#include "ImageFilter.h"
#include "ImageEnhance.h"
#include "CommonData.h"


// PreProcess1 对话框

IMPLEMENT_DYNAMIC(PreProcess1, CDialogEx)

PreProcess1::PreProcess1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGPreProcess1, pParent)
{

}

PreProcess1::~PreProcess1()
{
}

void PreProcess1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PreProcess1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONImageFilter1, &PreProcess1::OnBnClickedButtonimagefilter1)
	ON_BN_CLICKED(IDC_BUTTONImageEnhance1, &PreProcess1::OnBnClickedButtonimageenhance1)
	ON_BN_CLICKED(IDC_BUTTONAdjustFilter1, &PreProcess1::OnBnClickedButtonadjustfilter)
	ON_BN_CLICKED(IDC_BUTTONEnhanceAdjust1, &PreProcess1::OnBnClickedButtonenhanceadjust1)
END_MESSAGE_MAP()


// PreProcess1 消息处理程序

//默认的图像滤波方法  均值滤波
void PreProcess1::OnBnClickedButtonimagefilter1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	int ksize = 5;
	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if (middleWindow1 == 0 && middleWindow2 == 0)
	{
		blur(srcImage, middleImage1, Size(ksize, ksize));
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		imageFilter = middleImage1;
		fout << "blur(srcImage, middleImage1, Size(" << ksize << ", " << ksize << "));\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if(middleWindow2 >= middleWindow1)
	{
		blur(middleImage2, middleImage1, Size(ksize, ksize));
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		imageFilter = middleImage1;
		//middleImage2.rows = 0;
		fout << "blur(middleImage2, middleImage1, Size(" << ksize << ", " << ksize << "));\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if(middleWindow1 > middleWindow2)
	{
		blur(middleImage1, middleImage2, Size(11, 11));
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		imageFilter = middleImage2;
		//middleImage1.rows = 0;
		fout << "blur(middleImage1, middleImage2, Size(" << ksize << ", " << ksize << "));\n";
	}
	flagFilter = 1;       //滤波处理标志位  默认滤波处理完后，如果效果不好，需要修改滤波方式，则对原图进行滤波处理
}

//默认的图像增强方式  直方图均衡化
void PreProcess1::OnBnClickedButtonimageenhance1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if(middleWindow1 == 0 && middleWindow2 == 0)
	{
		if (srcImage.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		cvtColor(srcImage, srcImage, CV_BGR2GRAY);
		equalizeHist(srcImage, middleImage1);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		fout << "//直方图均衡化\n";						//注释
		fout << "equalizeHist(srcImage, middleImage1);\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if(middleWindow2 >= middleWindow1)
	{
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
		fout << "//直方图均衡化\n";
		fout << "equalizeHist(middleImage2, middleImage1);\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if(middleWindow1 > middleWindow2)
	{
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
		fout << "//直方图均衡化\n";
		fout << "equalizeHist(middleImage1, middleImage2);\n";
	}
	flagEnhance = 1;       //图像增强处理标志位  默认图像增强方式处理完后，如果效果不好，需要修改增强方式，则对原图进行图像增强处理
}


//图像滤波调整
void PreProcess1::OnBnClickedButtonadjustfilter()
{
	// TODO: 在此添加控件通知处理程序代码
	ImageFilter ImageFilterDlg;
	ImageFilterDlg.DoModal();

}

//图像增强调整
void PreProcess1::OnBnClickedButtonenhanceadjust1()
{
	// TODO: 在此添加控件通知处理程序代码
	ImageEnhance ImageEnhanceDlg;
	ImageEnhanceDlg.DoModal();
}
