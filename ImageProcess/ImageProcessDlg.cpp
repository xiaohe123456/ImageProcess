
// ImageProcessDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ImageProcess.h"
#include "ImageProcessDlg.h"
#include "afxdialogex.h"
#include "CommonData.h"
#include "yaml-cpp/yaml.h"
#include "ImageFilter.h"
#include "ImageEnhance.h"
#include "ImageGeoTransform.h"
#include "EdgeDetect.h"
#include "ImageSharpen.h"
#include "PreProcess1.h"
#include "PreProcess2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImageProcessDlg 对话框
//CommonData ComData;
CImageProcessDlg::CImageProcessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGEPROCESS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDERSmall, MinThreshold);
	DDX_Control(pDX, IDC_SLIDERBig, MaxThreshold);
}

BEGIN_MESSAGE_MAP(CImageProcessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONOpenImage, &CImageProcessDlg::OnBnClickedButtonopenimage)
	ON_BN_CLICKED(IDC_BUTTONSaveImage, &CImageProcessDlg::OnBnClickedButtonsaveimage)
	ON_BN_CLICKED(IDC_BUTTONImageEnhance, &CImageProcessDlg::OnBnClickedButtonimageenhance)
	ON_BN_CLICKED(IDC_BUTTONImageFilter, &CImageProcessDlg::OnBnClickedButtonimagefilter)
	ON_BN_CLICKED(IDC_BUTTONImageGeoTransform, &CImageProcessDlg::OnBnClickedButtonimagegeotransform)
	ON_BN_CLICKED(IDC_BUTTONImageGray, &CImageProcessDlg::OnBnClickedButtonimagegray)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERSmall, &CImageProcessDlg::OnNMCustomdrawSlidersmall)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERBig, &CImageProcessDlg::OnNMCustomdrawSliderbig)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTONBinary, &CImageProcessDlg::OnBnClickedButtonbinary)
	//ON_BN_CLICKED(IDC_BUTTONEdgeDetect, &CImageProcessDlg::OnBnClickedButtonedgedetect)
	ON_BN_CLICKED(IDC_BUTTONPreProcess1, &CImageProcessDlg::OnBnClickedButtonpreprocess1)
	ON_BN_CLICKED(IDC_BUTTONPreProcess2, &CImageProcessDlg::OnBnClickedButtonpreprocess2)
	//ON_BN_CLICKED(IDC_BUTTONImageSharpen, &CImageProcessDlg::OnBnClickedButtonimagesharpen)
	ON_BN_CLICKED(IDC_BUTTONNextImage, &CImageProcessDlg::OnBnClickedButtonnextimage)
	ON_WM_MOUSEWHEEL()

	ON_BN_CLICKED(IDC_BUTTONPreImage, &CImageProcessDlg::OnBnClickedButtonpreimage)
	ON_BN_CLICKED(IDC_BUTTONRevoke, &CImageProcessDlg::OnBnClickedButtonrevoke)
END_MESSAGE_MAP()

// CImageProcessDlg 消息处理程序

BOOL CImageProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//滑动二值化设置初始值以及范围
	MinThreshold.SetRange(0, 255);
	MinThreshold.SetPos(128);
	MaxThreshold.SetRange(0, 255);
	MaxThreshold.SetPos(255);

	// TODO: 在此添加额外的初始化代码
	//嵌套OpenCV窗口显示图像  原图像
	CRect rect;
	GetDlgItem(IDC_ShowOriImg)->GetWindowRect(rect);
	namedWindow(OriginalWindowName, CV_WINDOW_KEEPRATIO);   //创建OpenCV窗口 可以改变大小 图像适应picture control大小
	resizeWindow(OriginalWindowName, rect.Width(), rect.Height());
	setMouseCallback(OriginalWindowName, on_mouse, 0);   //鼠标回调函数  获取透视变换中四个点的坐标
	//setMouseCallback(OriginalWindowName, onMouse, 0);   //鼠标回调函数 实现鼠标拖动图像移动
	HWND hWnd = (HWND)cvGetWindowHandle(OriginalWindowName.c_str());//嵌套opencv窗口
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_ShowOriImg)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	//嵌套OpenCV窗口显示图像  中间结果1
	CRect rect1;
	GetDlgItem(IDC_ShowMidImage1)->GetWindowRect(rect1);
	namedWindow(MiddleWindowName1, CV_WINDOW_AUTOSIZE);   //创建OpenCV窗口 可以改变大小 图像适应picture control大小
	resizeWindow(MiddleWindowName1, rect1.Width(), rect1.Height());
	setMouseCallback(MiddleWindowName1, on_mouse, 0);   //鼠标回调函数
	HWND hWnd1 = (HWND)cvGetWindowHandle(MiddleWindowName1.c_str());//嵌套opencv窗口
	HWND hParent1 = ::GetParent(hWnd1);
	::SetParent(hWnd1, GetDlgItem(IDC_ShowMidImage1)->m_hWnd);
	::ShowWindow(hParent1, SW_HIDE);

	//嵌套OpenCV窗口显示图像  中间结果2
	CRect rect2;
	GetDlgItem(IDC_ShowMidImage2)->GetWindowRect(rect2);
	namedWindow(MiddleWindowName2, CV_WINDOW_AUTOSIZE);   //创建OpenCV窗口 可以改变大小 图像适应picture control大小
	resizeWindow(MiddleWindowName2, rect2.Width(), rect2.Height());
	setMouseCallback(MiddleWindowName2, on_mouse, 0);   //鼠标回调函数
	HWND hWnd2 = (HWND)cvGetWindowHandle(MiddleWindowName2.c_str());//嵌套opencv窗口
	HWND hParent2 = ::GetParent(hWnd2);
	::SetParent(hWnd2, GetDlgItem(IDC_ShowMidImage2)->m_hWnd);
	::ShowWindow(hParent2, SW_HIDE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CImageProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CImageProcessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CImageProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//读取图像
void CImageProcessDlg::OnBnClickedButtonopenimage()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, _T("*.jpg"), NULL,OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.png; *.bmp; *.jpg) |*.png;*.bmp;*.jpg|All Files (*.*)|*.*||"), NULL);
	//打开文件对话框的标题名
	dlg.m_ofn.lpstrTitle = _T("打开图像 ");
	if (IDOK == dlg.DoModal())
		mPath = dlg.GetPathName();		//获取文件名
	else
		return;
	mPath2 = dlg.GetFolderPath();		//获取文件夹路径
	int iEndPos = 0;
	iEndPos = mPath.ReverseFind('\\');	//找到CString对象中与要求的字符匹配的最后一个字符的索引
	mPath1 = mPath.Left(iEndPos);		//返回由最左边的'iEndPos'字符组成的子字符串
	HANDLE file;						//文件句柄
	WIN32_FIND_DATA fileData;			//文件查找结构体
	mFileList.clear();			
	mPath1 += "\\*.jpg";
	file = FindFirstFile(mPath1.GetBuffer(), &fileData);	//查找mPath1路径下的第一个文件
	mFileList.push_back(fileData.cFileName);				//保存至文件名列表
	bool bState = false;
	bState = FindNextFile(file, &fileData);					//查找文件状态
	//获取所有文件名并保存
	while (bState)
	{
		mFileList.push_back(fileData.cFileName);
		bState = FindNextFile(file, &fileData);
	}
	numImageMax = mFileList.size();					//当前路径下文件数目
	int length = mFileList.at(0).GetLength();		//文件名长度
	CString imageName = mPath.Right(length);		//获取文件名
	for (int i = 0; i < numImageMax; i++)
	{
		if (imageName.Compare(mFileList.at(i)) == 0)	//在文件名列表中找到与当前文件名相同的序号
			numImageNow = i;
	}
	//读取Mat类型的图像
	pathName = CW2A(mPath.GetString());
	srcImage = imread(pathName);
	dstImage = srcImage;
	resultMidWindow1.push_back(srcImage);			//将原图保存到结果图像向量的起始位置
	resultMidWindow2.push_back(srcImage);
	//为了保证图像与picture控件大小相适应
	CRect rect;
	GetDlgItem(IDC_ShowOriImg)->GetClientRect(&rect);
	resize(srcImage, srcImage, Size(rect.Width(), rect.Height()));
	imshow(OriginalWindowName, srcImage);
}

//通过按钮获取上一张图像
void CImageProcessDlg::OnBnClickedButtonpreimage()
{
	// TODO: 在此添加控件通知处理程序代码
	--numImageNow;
	if (numImageNow < 0)
	{
		MessageBox(_T("这是第一张图像，请向后查看"));
	}
	else
	{
		if (numImageNow >= numImageMax)
			numImageNow = numImageMax - 1;
		pathName = CW2A(mPath2.GetString());
		pathName = pathName + "\\" + CW2A(mFileList.at(numImageNow).GetString());
		srcImage = imread(pathName);
		dstImage = srcImage;
		resultMidWindow1.push_back(srcImage);			//将原图保存到结果图像向量的起始位置
		resultMidWindow2.push_back(srcImage);
		//为了保证图像与picture控件大小相适应
		CRect rect;
		GetDlgItem(IDC_ShowOriImg)->GetClientRect(&rect);
		resize(srcImage, srcImage, Size(rect.Width(), rect.Height()));
		imshow(OriginalWindowName, srcImage);
	}
}

//通过按钮获取下一张图像
void CImageProcessDlg::OnBnClickedButtonnextimage()
{
	// TODO: 在此添加控件通知处理程序代码
	++numImageNow;
	if (numImageNow >= numImageMax)
	{
		MessageBox(_T("这是最后一张图像，请向前查看"));
	}
	else
	{
		if (numImageNow < 0)
			numImageNow = 0;
		pathName = CW2A(mPath2.GetString());
		pathName = pathName + "\\" + CW2A(mFileList.at(numImageNow).GetString());
		srcImage = imread(pathName);
		dstImage = srcImage;
		resultMidWindow1.push_back(srcImage);			//将原图保存到结果图像向量的起始位置
		resultMidWindow2.push_back(srcImage);
		//为了保证图像与picture控件大小相适应
		CRect rect;
		GetDlgItem(IDC_ShowOriImg)->GetClientRect(&rect);
		resize(srcImage, srcImage, Size(rect.Width(), rect.Height()));
		imshow(OriginalWindowName, srcImage);
	}
}



//保存图像结果
void CImageProcessDlg::OnBnClickedButtonsaveimage()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, _T("*.jpg"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.png; *.bmp; *.jpg) |*.png;*.bmp;*.jpg|All Files (*.*)|*.*||"), NULL);
	//打开文件对话框的标题名
	dlg.m_ofn.lpstrTitle = _T("保存图像 ");
	if (dlg.DoModal() != IDOK)
		return;
	CString  mPath1 = dlg.GetPathName();
	string mPath(CW2A(mPath1.GetString()));
	if (middleWindow2 == 0 && middleWindow1 == 0)
		imwrite(mPath, middleImage1);
	else if (middleWindow2 >= middleWindow1)
		imwrite(mPath, middleImage2);
	else if (middleWindow1 > middleWindow2)
		imwrite(mPath, middleImage1);
	else
		MessageBox(_T("error"));
	fs_write.release();
}


//灰度化
void CImageProcessDlg::OnBnClickedButtonimagegray()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	if(middleWindow1 == 0 && middleWindow2 ==0)
	{
		if (srcImage.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		cvtColor(srcImage, middleImage1, CV_BGR2GRAY);
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if ((middleWindow1 == middleWindow2) || (middleWindow2 > middleWindow1))
	{
		if (middleImage2.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		cvtColor(middleImage2, middleImage1, CV_BGR2GRAY);
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if(middleWindow1 > middleWindow2)
	{
		if (middleImage1.channels() != 3)
		{
			MessageBox(_T("当前图像为单通道图像，请选彩色图像"));
			return;
		}
		cvtColor(middleImage1, middleImage2, CV_BGR2GRAY);
		imshow(MiddleWindowName2, middleImage2);
		resultMidWindow2.push_back(middleImage2);
	}
	fs_write << "Operation" << "{";
	fs_write << "function" << "cvtColor" << "effect" << "image graying";
	fs_write << "}";
}

//按钮 二值化  
void CImageProcessDlg::OnBnClickedButtonbinary()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	int thresholdMin = MinThreshold.GetPos();
	int thresholdMax = MaxThreshold.GetPos();
	if (middleWindow1 == 0 && middleWindow2 == 0)
	{
		threshold(srcImage, middleImage1, thresholdMin, thresholdMax, THRESH_BINARY);
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if ((middleWindow1 == middleWindow2) || (middleWindow2 > middleWindow1))
	{
		threshold(middleImage2, middleImage1, thresholdMin, thresholdMax, THRESH_BINARY);
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if (middleWindow1 > middleWindow2)
	{
		threshold(middleImage1, middleImage2, thresholdMin, thresholdMax, THRESH_BINARY);
		imshow(MiddleWindowName2, middleImage2);
		resultMidWindow2.push_back(middleImage2);
		middleWindow2++;
	}
	fs_write << "Operation" << "{";
	fs_write << "function" << "threshold" << "effect" << "image binarization";
	fs_write << "}";
	fs_write << "Param" << "{";
	fs_write << "thresholdMin" << thresholdMin << "thresholdMax" << thresholdMax;
	fs_write << "}";
}


//滑动控件，获取二值化中的小阈值
void CImageProcessDlg::OnNMCustomdrawSlidersmall(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nPos = MinThreshold.GetPos();
	int thresholdMin = nPos;
	CString threshMin;
	threshMin.Format(_T("%d"), nPos);
	SetDlgItemText(IDC_EDITBinaryMin, threshMin);
}

//滑动控件，获取二值化中的大阈值
void CImageProcessDlg::OnNMCustomdrawSliderbig(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nPos = MaxThreshold.GetPos();
	int thresholdMax = nPos;
	CString threshMax;
	threshMax.Format(_T("%d"), nPos);
	SetDlgItemText(IDC_EDITBinaryMax, threshMax);
}

//滑动控件的同时，对图像做二值化
void CImageProcessDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//获取滑条上的小阈值和大阈值
	int thresholdMin = MinThreshold.GetPos();
	int thresholdMax = MaxThreshold.GetPos();
	threshold(srcImage, middleImage1, thresholdMin, thresholdMax, THRESH_BINARY);
	imshow(MiddleWindowName1, middleImage1);

	fs_write << "Operation" << "{";
	fs_write << "function" << "threshold" << "effect" << "image binarization";
	fs_write << "}";
	fs_write << "Param" << "{";
	fs_write << "thresholdMin" << thresholdMin << "thresholdMax" << thresholdMax;
	fs_write << "}"; 
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

//图像增强
void CImageProcessDlg::OnBnClickedButtonimageenhance()
{
	// TODO: 在此添加控件通知处理程序代码
	ImageEnhance ImageEnhanceDlg;
	ImageEnhanceDlg.DoModal();
}

//图像滤波
void CImageProcessDlg::OnBnClickedButtonimagefilter()
{
	// TODO: 在此添加控件通知处理程序代码
	ImageFilter ImageFilterDlg;
	ImageFilterDlg.DoModal();
}

//几何变换
void CImageProcessDlg::OnBnClickedButtonimagegeotransform()
{
	// TODO: 在此添加控件通知处理程序代码
	ImageGeoTransform ImageGeoTransformDlg;
	ImageGeoTransformDlg.DoModal();
}

//图像处理1
void CImageProcessDlg::OnBnClickedButtonpreprocess1()
{
	// TODO: 在此添加控件通知处理程序代码
	PreProcess1 PreProcess1Dlg;
	PreProcess1Dlg.DoModal();
}

//图像处理2
void CImageProcessDlg::OnBnClickedButtonpreprocess2()
{
	// TODO: 在此添加控件通知处理程序代码
	PreProcess2 PreProcess2Dlg;
	PreProcess2Dlg.DoModal();
}

//opencv鼠标相应函数  获取仿射变换或透视变换的坐标
void on_mouse(int event, int x, int y, int flags, void* ustc)
{
	char locate[20];
	Mat img1;
	
	HWND hWnd = (HWND)cvGetWindowHandle(OriginalWindowName.c_str());//嵌套opencv窗口
	HWND hParent = ::GetParent(hWnd);			//获取父窗口句柄 为了使用MessageBox
	if (srcImage.empty())
	{
		MessageBox(hParent, _T("请加载图像"),TEXT("Error"),MB_OK);
		return;
	}
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);///初始化字体
	//鼠标左键事件   透视变换
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		srcPoints[countL] = Point(x, y);
		sprintf_s(locate, "(%d,%d)", x, y);  //将数据格式化输出到字符串
		//putText(srcImage, locate, Point(x, y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 0, 255));//显示坐标
		circle(srcImage, Point(x, y), 3, CV_RGB(0, 0, 255));//划圆
		imshow(OriginalWindowName, srcImage);
		countL++;
	}
	//鼠标移动事件，实时显示坐标
	if (event == CV_EVENT_MOUSEMOVE)
	{
		img1 = srcImage.clone();
		sprintf_s(locate, "(%d,%d)", x, y);
		putText(img1, locate, Point(x, y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 0, 255));
		if (flagMouse == 1)			//表示执行过鼠标缩放事件  显示缩放后的结果
		{
			imshow(OriginalWindowName, dstImage);
		}
		else
		{
			imshow(OriginalWindowName, img1);
		}	
	}
	//鼠标右键事件  仿射变换
	if (event == CV_EVENT_RBUTTONDOWN)
	{
		if (countR < 3)				//仿射变换源点
			srcPointsA[countR] = Point(x, y);
		else						//仿射变换目标点
			dstPointsA[countR - 3] = Point(x, y);
		sprintf_s(locate, "(%d,%d)", x, y);  //将数据格式化输出到字符串
		putText(srcImage, locate, Point(x, y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255, 0, 0));//显示坐标
		circle(srcImage, Point(x, y), 5, CV_RGB(255, 0, 0));//划圆
		imshow(OriginalWindowName, srcImage);
		countR++;
	}
	//鼠标中键事件   重新初始化点的数目
	if (event == CV_EVENT_MBUTTONDOWN)
	{
		countL = 0;      
		countR = 0;
		middleWindow1 = 0;
		middleWindow2 = 0;
		for (int i = 0; i < 4; i++)
			srcPoints[i] = Point2f(0, 0);
		for (int i = 0; i < 3; i++)
		{
			srcPointsA[i] = Point2f(0, 0);
			dstPointsA[i] = Point2f(0, 0);
		}
			
		MessageBox(hParent, _T("请重新选点"), TEXT("Attention"), MB_OK);
		//显示未标记圆的原图
		srcImage = imread(pathName);
		//在外部类获取控件句柄，首先获取主框架的句柄
		HWND hWnd1 = theApp.GetMainWnd()->m_hWnd;    //获取对话框句柄
		CWnd* pWnd = CWnd::FromHandle(GetDlgItem(hWnd1, IDC_ShowOriImg));
		CRect rect;
		pWnd->GetClientRect(&rect);
		resize(srcImage, srcImage, Size(rect.Width(), rect.Height()));
		imshow(OriginalWindowName, srcImage);
		fs_write.release(); //关闭后再打开是为了清空前一次透视变换得到的矩阵，保证在yaml文件
		fs_write.open(str,1,"UTF-8"); //中只保存最新选中的透视变换中的源点		
	}
}

//图像锐化
//void CImageProcessDlg::OnBnClickedButtonimagesharpen()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	ImageSharpen ImageSharpenDlg;
//	ImageSharpenDlg.DoModal();
//}

//边缘检测
//void CImageProcessDlg::OnBnClickedButtonedgedetect()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	EdgeDetect EdgeDetectDlg;
//	EdgeDetectDlg.DoModal();
//}

//鼠标滑轮缩放图像
BOOL CImageProcessDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetDlgItem(IDC_ShowOriImg)->ShowWindow(FALSE);
	GetDlgItem(IDC_ShowOriImg)->ShowWindow(TRUE);
	GetDlgItem(IDC_ShowMidImage1)->ShowWindow(FALSE);
	GetDlgItem(IDC_ShowMidImage1)->ShowWindow(TRUE);
	GetDlgItem(IDC_ShowMidImage2)->ShowWindow(FALSE);
	GetDlgItem(IDC_ShowMidImage2)->ShowWindow(TRUE);
	UpdateData(TRUE);

	if (!srcImage.empty())
	{
		CRect rect;
		GetDlgItem(IDC_ShowOriImg)->GetWindowRect(&rect);//获取MFC图像显示区域
		Point p(pt.x, pt.y);//opencv鼠标坐标(相对屏幕)
		Rect r(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);//opencv窗口显示区域
		if (r.contains(p)) //鼠标放在图片窗口
		{
			if (zDelta > 0)//放大图片
			{
				Zoom = Zoom + Radio;
				if (Zoom < 5)
				{
					dstImage = Mat(Size(srcImage.cols * Zoom, srcImage.rows * Zoom), CV_8UC3);
					resize(srcImage, dstImage, dstImage.size(), INTER_LINEAR); //按比例放大图片
				}
				else
				{
					MessageBox(_T("图像过大，请缩小"));
					IContinue;
				}
			}
			else if (zDelta < 0)//缩小图片
			{
				Zoom = Zoom - Radio;
				if (Zoom > 0.01) {			//保证图像不能太小，可以继续缩小  否则给出提示
					dstImage = Mat(Size(srcImage.cols * Zoom, srcImage.rows * Zoom), CV_8UC3);
					resize(srcImage, dstImage, dstImage.size(), INTER_LINEAR); //按比例放大图片
				}
				else {
					MessageBox(_T("图像太小，请放大"));
					IContinue;
				}
			}
			imshow(OriginalWindowName, dstImage);//显示缩放后图片
		}
	}
	flagMouse = 1;
	UpdateData(FALSE);
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

//鼠标拖动图像移动  两个鼠标回调函数只能用一个，这个不使用
void onMouse(int event, int x, int y, int flags, void* ustc)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (event == CV_EVENT_RBUTTONDOWN)
	{
		before_x = x;			//起始点坐标
		before_y = y;
	}
	if (event == CV_EVENT_RBUTTONUP)
	{
		current_x = x;			//平移后当前位置坐标
		current_y = y;
		tx = (current_x - before_x);	//平移距离
		ty = (current_y - before_y);
		int dst_rows = srcImage.rows;//图像高度
		int dst_cols = srcImage.cols;//图像宽度
		if (srcImage.channels() == 1) {
			dstImage = cv::Mat::zeros(dst_rows, dst_cols, CV_8UC1); //灰度图初始
		}
		else {
			dstImage = cv::Mat::zeros(dst_rows, dst_cols, CV_8UC3); //RGB图初始
		}
		cv::Mat T = (cv::Mat_<double>(3, 3) << 1, 0, 0, 0, 1, 0, tx, ty, 1); //平移变换矩阵
		cv::Mat T_inv = T.inv(); // 求逆矩阵
		for (int i = 0; i < dstImage.rows; i++) 
		{
			for (int j = 0; j < dstImage.cols; j++) 
			{
				cv::Mat dst_coordinate = (cv::Mat_<double>(1, 3) << j, i, 1);
				cv::Mat src_coordinate = dst_coordinate * T_inv;
				double v = src_coordinate.at<double>(0, 0); // 原图像的横坐标，列，宽
				double w = src_coordinate.at<double>(0, 1); // 原图像的纵坐标，行，高

				/*双线性插值*/
				// 判断是否越界
				if (v >= 0 && w >= 0 && v <= srcImage.cols - 1 && w <= srcImage.rows - 1) 
				{
					int top = floor(w), bottom = ceil(w), left = floor(v), right = ceil(v); //与映射到原图坐标相邻的四个像素点的坐标
					double pw = w - top; //pw为坐标 行 的小数部分(坐标偏差)
					double pv = v - left; //pv为坐标 列 的小数部分(坐标偏差)
					if (srcImage.channels() == 1)
					{
						//灰度图像
						dstImage.at<uchar>(i, j) = (1 - pw) * (1 - pv) * srcImage.at<uchar>(top, left) + (1 - pw) * pv * srcImage.at<uchar>(top, right) + pw * (1 - pv) * srcImage.at<uchar>(bottom, left) + pw * pv * srcImage.at<uchar>(bottom, right);
					}
					else 
					{
						//彩色图像
						dstImage.at<Vec3b>(i, j)[0] = (1 - pw) * (1 - pv) * srcImage.at<Vec3b>(top, left)[0] + (1 - pw) * pv * srcImage.at<Vec3b>(top, right)[0] + pw * (1 - pv) * srcImage.at<Vec3b>(bottom, left)[0] + pw * pv * srcImage.at<Vec3b>(bottom, right)[0];
						dstImage.at<Vec3b>(i, j)[1] = (1 - pw) * (1 - pv) * srcImage.at<Vec3b>(top, left)[1] + (1 - pw) * pv * srcImage.at<Vec3b>(top, right)[1] + pw * (1 - pv) * srcImage.at<Vec3b>(bottom, left)[1] + pw * pv * srcImage.at<Vec3b>(bottom, right)[1];
						dstImage.at<Vec3b>(i, j)[2] = (1 - pw) * (1 - pv) * srcImage.at<Vec3b>(top, left)[2] + (1 - pw) * pv * srcImage.at<Vec3b>(top, right)[2] + pw * (1 - pv) * srcImage.at<Vec3b>(bottom, left)[2] + pw * pv * srcImage.at<Vec3b>(bottom, right)[2];
					}
				}
			}
		}
	imshow(OriginalWindowName, dstImage);
	}
}


/*
撤销操作：
1.定义变量middleWindow1和middleWindow2两个变量，分别表示中间窗口1和2显示结果的次数
resultMidWindow1和resultMidWindow2表示窗口结果1和2保存的结果图像，加载图像时将原图分别放到
resultMidWindow1和resultMidWindow2中，保证middleWindow1和结果向量resultMidWindow1中的结果序号对应
2.当middleWindow1 > middleWindow2且middleWindow2不为0时，表示在最后一次操作中，窗口2是上一次结果图，
窗口1是当前结果图，撤销操作就是将窗口1恢复为窗口2中的图像，并将middleWindow1减1且窗口2结果向量
移除最后一个元素（如果再点撤销的话，窗口2将显示再前一步的结果，需将前一次结果移除）
3.当middleWindow1 > middleWindow2且middleWindow2为0时，表示只对图像做了一步处理操作，撤销处理则只是
将窗口1显示为原图即可，即显示resultMidWindow2的元素（此时resultMidWindow2只有一个原图），并将
middleWindow1减1
4.当middleWindow1 < middleWindow2或middleWindow1 = middleWindow2时，表示在最后一次操作中，窗口1是
上一次结果图，窗口2是当前结果图，撤销操作则将窗口2恢复为窗口1中的图像，并将middleWindow2减1且
窗口1结果向量移除最后一个元素
*/
void CImageProcessDlg::OnBnClickedButtonrevoke()
{
	// TODO: 在此添加控件通知处理程序代码
	if (((middleWindow1 == middleWindow2) || (middleWindow1 < middleWindow2)) && (middleWindow1 > 0 && middleWindow2 > 0))
	{
		imshow(MiddleWindowName2, resultMidWindow1[middleWindow1]);
		resultMidWindow1.pop_back();
		middleWindow2--;
	}
	else if ((middleWindow1 > middleWindow2) && (middleWindow2 != 0) && (middleWindow1 >= 0 && middleWindow2 >= 0))
	{
		imshow(MiddleWindowName1, resultMidWindow2[middleWindow2]);
		resultMidWindow2.pop_back();
		middleWindow1--;
	}
	else if ((middleWindow1 > middleWindow2) && (middleWindow2 == 0) && (middleWindow1 >= 0 && middleWindow2 >= 0))  //只做了一步操作
	{
		//为了保证图像与picture控件大小相适应
		CRect rect;
		GetDlgItem(IDC_ShowMidImage1)->GetClientRect(&rect);
		resize(resultMidWindow2[middleWindow2], resultMidWindow2[middleWindow2], Size(rect.Width(), rect.Height()));
		imshow(MiddleWindowName1, resultMidWindow2[middleWindow2]);
		resultMidWindow1.pop_back();
		middleWindow1--;
	}
	else if (middleWindow1 == 0 && middleWindow2 == 0)
	{
		MessageBox(_T("所有操作均已撤销，请勿再点撤销按钮"));
		return;
	}
}
