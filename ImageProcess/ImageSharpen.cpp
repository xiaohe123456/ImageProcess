// ImageSharpen.cpp: 实现文件
//

#include "pch.h"
#include "ImageProcess.h"
#include "ImageSharpen.h"
#include "afxdialogex.h"
#include "CommonData.h"


// ImageSharpen 对话框

IMPLEMENT_DYNAMIC(ImageSharpen, CDialogEx)

ImageSharpen::ImageSharpen(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGImageSharpen, pParent)
{

}

ImageSharpen::~ImageSharpen()
{
}

void ImageSharpen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOSobelKernel, SobelKernel);
	DDX_Control(pDX, IDC_COMBOSobelScale, SobelScale);
	DDX_Control(pDX, IDC_COMBOSobelDelta, SobelDelta);
	DDX_Control(pDX, IDC_COMBOXorY, XorY);
	DDX_Control(pDX, IDC_COMBOLapKernel, LaplacianKernel);
}

BOOL ImageSharpen::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	int i = 1;
	//设置方框、均值和中值滤波器核控件的大小
	while (i < 10)
	{
		str.Format(_T("%d"), i);
		SobelKernel.AddString(str);
		LaplacianKernel.AddString(str);
		i += 2;
	}
	for (int j = 0; j < 10; j++)
	{
		str.Format(_T("%d"), j);
		SobelScale.AddString(str);
		SobelDelta.AddString(str);
	}
	LaplacianKernel.SetCurSel(2);
	SobelKernel.SetCurSel(1);
	SobelDelta.SetCurSel(0);
	SobelScale.SetCurSel(1);
	str.Format(_T("%d"), 0);
	XorY.AddString(str);
	str.Format(_T("%d"), 1);
	XorY.AddString(str);
	XorY.SetCurSel(0);
	return 0;
}


BEGIN_MESSAGE_MAP(ImageSharpen, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONSobel, &ImageSharpen::OnBnClickedButtonsobel)
	ON_BN_CLICKED(IDC_BUTTONLaplacian, &ImageSharpen::OnBnClickedButtonlaplacian)
END_MESSAGE_MAP()


// ImageSharpen 消息处理程序

//Sobel算子锐化图像
void ImageSharpen::OnBnClickedButtonsobel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	int index;
	CString sKize, sScale, sDelta, xy;
	index = SobelKernel.GetCurSel();
	SobelKernel.GetLBText(index, sKize);
	int ksize = _ttoi(sKize);				//核大小
	index = SobelDelta.GetCurSel();
	SobelDelta.GetLBText(index, sDelta);
	double delta = _ttof(sDelta);			//delta值
	index = SobelScale.GetCurSel();
	SobelScale.GetLBText(index, sScale);
	double scale = _ttof(sScale);			//scale值
	index = XorY.GetCurSel();
	XorY.GetLBText(index, xy);
	int direction = _ttoi(xy);   //0表示x方向  1表示y方向
	if (middleImage1.rows == 0 && middleImage2.rows == 0)
	{
		if (srcImage.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		GaussianBlur(srcImage, srcImage, Size(3, 3), 0, 0, BORDER_DEFAULT);
		cvtColor(srcImage, srcImage, COLOR_BGR2GRAY);
		if (direction == 0)
		{
			Sobel(srcImage, middleImage1, CV_16S, 1, 0, ksize, scale, delta);
			fout << "Sobel(srcImage, middleImage1, CV_16S, 1, 0, " << ksize << ", " << scale << ", "<< delta << ");\n";
		}	
		else
		{
			Sobel(srcImage, middleImage1, CV_16S, 0, 1, ksize, scale, delta);
			fout << "Sobel(srcImage, middleImage1, CV_16S, 0, 1, " << ksize << ", " << scale << ", " << delta << ");\n";
		}
		convertScaleAbs(middleImage1, middleImage1);
		imshow(MiddleWindowName1, middleImage1);	
	}
	else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	{
		if (middleImage2.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		GaussianBlur(middleImage2, middleImage2, Size(3, 3), 0, 0, BORDER_DEFAULT);
		cvtColor(middleImage2, middleImage2, COLOR_BGR2GRAY);
		if (direction == 0)
		{
			Sobel(middleImage2, middleImage1, CV_16S, 1, 0, ksize, scale, delta);
			fout << "Sobel(middleImage2, middleImage1, CV_16S, 1, 0, " << ksize << ", " << scale << ", " << delta << ");\n";
		}
		else
		{
			Sobel(middleImage2, middleImage1, CV_16S, 1, 0, ksize, scale, delta);
			fout << "Sobel(middleImage2, middleImage1, CV_16S, 1, 0, " << ksize << ", " << scale << ", " << delta << ");\n";
		}
		convertScaleAbs(middleImage1, middleImage1);
		imshow(MiddleWindowName1, middleImage1);
		middleImage1.rows = 0;
	}
	else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	{
		if (middleImage1.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		GaussianBlur(middleImage1, middleImage1, Size(3, 3), 0, 0, BORDER_DEFAULT);
		cvtColor(middleImage1, middleImage1, COLOR_BGR2GRAY);
		if (direction == 0)
		{
			Sobel(middleImage1, middleImage2, CV_16S, 1, 0, ksize, scale, delta);
			fout << "Sobel(middleImage1, middleImage2, CV_16S, 1, 0, " << ksize << ", " << scale << ", " << delta << ");\n";
		}
		else
		{
			Sobel(middleImage1, middleImage2, CV_16S, 1, 0, ksize, scale, delta);
			fout << "Sobel(middleImage1, middleImage2, CV_16S, 1, 0, " << ksize << ", " << scale << ", " << delta << ");\n";
		}
		convertScaleAbs(middleImage2, middleImage2);
		imshow(MiddleWindowName2, middleImage2);
		middleImage1.rows = 0;
	}
}

//拉普拉斯算子锐化图像
void ImageSharpen::OnBnClickedButtonlaplacian()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	int index;
	CString sKize;
	index = LaplacianKernel.GetCurSel();
	LaplacianKernel.GetLBText(index, sKize);
	int ksize = _ttoi(sKize);

	if (middleImage1.rows == 0 && middleImage2.rows == 0)
	{
		if (srcImage.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		GaussianBlur(srcImage, srcImage, Size(3, 3), 0, 0, BORDER_DEFAULT);
		cvtColor(srcImage, srcImage, COLOR_BGR2GRAY);
		Laplacian(srcImage, middleImage1, CV_16S, ksize);
		convertScaleAbs(middleImage1, middleImage1);
		imshow(MiddleWindowName1, middleImage1);
	}
	else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	{
		if (middleImage2.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		GaussianBlur(middleImage2, middleImage2, Size(3, 3), 0, 0, BORDER_DEFAULT);
		cvtColor(middleImage2, middleImage2, COLOR_BGR2GRAY);
		Laplacian(middleImage2, middleImage1, CV_16S, ksize);
		convertScaleAbs(middleImage1, middleImage1);
		imshow(MiddleWindowName1, middleImage1);
		middleImage1.rows = 0;
	}
	else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	{
		if (middleImage1.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		GaussianBlur(middleImage1, middleImage1, Size(3, 3), 0, 0, BORDER_DEFAULT);
		cvtColor(middleImage1, middleImage1, COLOR_BGR2GRAY);
		Laplacian(middleImage1, middleImage2, CV_16S, ksize);
		convertScaleAbs(middleImage2, middleImage2);
		imshow(MiddleWindowName2, middleImage2);
		middleImage1.rows = 0;
	}
}
