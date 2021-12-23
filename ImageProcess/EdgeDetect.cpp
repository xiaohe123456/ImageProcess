// EdgeDetect.cpp: 实现文件
//

#include "pch.h"
#include "ImageProcess.h"
#include "EdgeDetect.h"
#include "afxdialogex.h"
#include "CommonData.h"


// EdgeDetect 对话框

IMPLEMENT_DYNAMIC(EdgeDetect, CDialogEx)

EdgeDetect::EdgeDetect(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGEdgeDetect, pParent)
{

}

EdgeDetect::~EdgeDetect()
{
}

void EdgeDetect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(EdgeDetect, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONCanny, &EdgeDetect::OnBnClickedButtoncanny)
END_MESSAGE_MAP()


// EdgeDetect 消息处理程序

//Canny边缘检测
void EdgeDetect::OnBnClickedButtoncanny()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	int lowThresh = GetDlgItemInt(IDC_EDITCannyLowThresh);		//Canny边缘检测低阈值
	int highThresh = GetDlgItemInt(IDC_EDITCannyHighThresh);	//高阈值
	if (middleWindow1 == 0 &&middleWindow2 == 0)
	{
		if (srcImage.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		cvtColor(srcImage, middleImage1, CV_BGR2GRAY);
		Canny(srcImage, middleImage1, lowThresh, highThresh, 3);
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
		fout << "Canny(srcImage, middleImage1, " << lowThresh << ", " << highThresh << ", 3);\n";
	}
	else if (middleWindow2 >= middleWindow1)
	{
		if (middleImage2.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		cvtColor(middleImage2, middleImage2, CV_BGR2GRAY);
		Canny(middleImage2, middleImage1, lowThresh, highThresh, 3);
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
		fout << "Canny(middleImage2, middleImage1, " << lowThresh << ", " << highThresh << ", 3);\n";
	}
	else if (middleWindow1 > middleWindow2)
	{
		if (middleImage1.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		cvtColor(middleImage1, middleImage1, CV_BGR2GRAY);
		Canny(middleImage1, middleImage2, lowThresh, highThresh, 3);
		imshow(MiddleWindowName2, middleImage2);
		resultMidWindow2.push_back(middleImage2);
		middleWindow2++;
		fout << "Canny(middleImage1, middleImage2, " << lowThresh << ", " << highThresh << ", 3);\n";
	}
}
