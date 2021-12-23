// PreProcess2.cpp: 实现文件
//

#include "pch.h"
#include "ImageProcess.h"
#include "PreProcess2.h"
#include "afxdialogex.h"
#include "ImageGeoTransform.h"
#include "ImageEnhance.h"
#include "CommonData.h"


// PreProcess2 对话框

IMPLEMENT_DYNAMIC(PreProcess2, CDialogEx)

PreProcess2::PreProcess2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGPreProcess2, pParent)
{

}

PreProcess2::~PreProcess2()
{
}

void PreProcess2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PreProcess2, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTONGeoTransform2, &PreProcess2::OnBnClickedButtongeotransform2)
	ON_BN_CLICKED(IDC_BUTTONImageEnhance2, &PreProcess2::OnBnClickedButtonimageenhance2)
	ON_BN_CLICKED(IDC_BUTTONAdjustGeotransform, &PreProcess2::OnBnClickedButtonadjustgeotransform)
	ON_BN_CLICKED(IDC_BUTTONAdjustImageEnhance, &PreProcess2::OnBnClickedButtonadjustimageenhance)
END_MESSAGE_MAP()


// PreProcess2 消息处理程序
//透视变换
void PreProcess2::OnBnClickedButtongeotransform2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	if (srcPoints[0] == Point2f(0, 0))
	{
		MessageBox(_T("请用鼠标左键标记透视变换的源点"));
		return;
	}
	//获取保存在yaml文件中的四个源点的坐标
	for (YAML::const_iterator it = config["Points"].begin(); it != config["Points"].end(); ++it)
	{
		string points;
		points = it->second.as<string>();
		int x = points.find(",", 1);
		srcPointsP[it->first.as<int>()].x = atof(points.substr(1, x - 1).c_str());
		srcPointsP[it->first.as<int>()].y = atof(points.substr(x + 1, points.length() - 1).c_str());
	}
	//透视变换目标点坐标
	dstPoints[0] = Point2f(0, 0);
	dstPoints[1] = Point2f(srcImage.cols, 0);
	dstPoints[2] = Point2f(srcImage.cols, srcImage.rows);
	dstPoints[3] = Point2f(0, srcImage.rows);

	Mat Perspective = getPerspectiveTransform(srcPointsP, dstPoints);//由四个点对计算透视变换矩阵  
	if (middleWindow1 == 0 && middleWindow2 == 0)
	{
		warpPerspective(srcImage, middleImage1, Perspective, srcImage.size());
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
		fout << "//透视变换\n";
		fout << "warpPerspective(srcImage, middleImage1, " << Perspective << ", " << "srcImage.size());\n";
	}
	else if(middleWindow2 >= middleWindow1)
	{
		warpPerspective(srcImage, middleImage1, Perspective, middleImage2.size());
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
		fout << "//透视变换\n";
		fout << "warpPerspective(middleImage2, middleImage1, " << Perspective << ", " << "middleImage2.size());\n";
	}
	else if(middleWindow1 > middleWindow2)
	{
		warpPerspective(srcImage, middleImage2, Perspective, middleImage1.size());
		imshow(MiddleWindowName2, middleImage2);
		resultMidWindow2.push_back(middleImage2);
		middleWindow2++;
		fout << "//透视变换\n";
		fout << "warpPerspective(middleImage1, middleImage2, " << Perspective << ", " << "middleImage1.size());\n";
	}
}

//图像增强  直方图均衡化
void PreProcess2::OnBnClickedButtonimageenhance2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	if(middleWindow1 == 0 && middleWindow2 == 0)
	{
		if (srcImage.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		cvtColor(srcImage, middleImage1, CV_BGR2GRAY);
		equalizeHist(middleImage1, middleImage1);
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
		fout << "//直方图均衡化\n";						
		fout << "equalizeHist(srcImage, middleImage1);\n";
	}
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
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
		fout << "//直方图均衡化\n";
		fout << "equalizeHist(middleImage2, middleImage1);\n";
	}
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
		resultMidWindow2.push_back(middleImage2);
		middleWindow2++;
		fout << "//直方图均衡化\n";
		fout << "equalizeHist(middleImage1, middleImage2);\n";
	}
	flagEnhance = 1;       //图像增强处理标志位  默认图像增强方式处理完后，如果效果不好，需要修改增强方式，则对原图进行图像增强处理
}

//调整几何变换界面
void PreProcess2::OnBnClickedButtonadjustgeotransform()
{
	// TODO: 在此添加控件通知处理程序代码
	ImageGeoTransform ImageGeoTransformDlg;
	ImageGeoTransformDlg.DoModal();
}

//调整图像增强界面
void PreProcess2::OnBnClickedButtonadjustimageenhance()
{
	// TODO: 在此添加控件通知处理程序代码
	ImageEnhance ImageEnhanceDlg;
	ImageEnhanceDlg.DoModal();
}
