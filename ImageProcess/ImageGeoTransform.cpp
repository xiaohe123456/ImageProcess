// ImageGeoTransform.cpp: 实现文件
//

#include "pch.h"
#include "ImageProcess.h"
#include "ImageGeoTransform.h"
#include "afxdialogex.h"
#include "CommonData.h"
#include "resource.h"

// ImageGeoTransform 对话框
IMPLEMENT_DYNAMIC(ImageGeoTransform, CDialogEx)
ImageGeoTransform::ImageGeoTransform(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGGeoTransform, pParent)
{

}

ImageGeoTransform::~ImageGeoTransform()
{
}

void ImageGeoTransform::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOImageMoveX, MoveX);
	DDX_Control(pDX, IDC_COMBOImageMoveY, MoveY);
	DDX_Control(pDX, IDC_COMBOImageRotateAngle, RotateAngle);
	DDX_Control(pDX, IDC_COMBOImageScale, ImageScale);
	DDX_Control(pDX, IDC_COMBOImageSimilarityAngle, SimilarityAngle);
	DDX_Control(pDX, IDC_COMBOImageSimilarityScale, SimilarityScale);
	DDX_Control(pDX, IDC_COMBOImageFlip, FlipCode);
}

BEGIN_MESSAGE_MAP(ImageGeoTransform, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONImageRotate, &ImageGeoTransform::OnBnClickedButtonimagerotate)
	ON_BN_CLICKED(IDC_BUTTONImageMove, &ImageGeoTransform::OnBnClickedButtonimagemove)
	ON_BN_CLICKED(IDC_BUTTONImageScale, &ImageGeoTransform::OnBnClickedButtonimagescale)
	ON_BN_CLICKED(IDC_BUTTONImageSimilarity, &ImageGeoTransform::OnBnClickedButtonimagesimilarity)
	ON_BN_CLICKED(IDC_BUTTONImagePerpective, &ImageGeoTransform::OnBnClickedButtonimageperpective)
	ON_BN_CLICKED(IDC_BUTTONImageFlip, &ImageGeoTransform::OnBnClickedButtonimageflip)
	ON_BN_CLICKED(IDC_BUTTONImageAffine, &ImageGeoTransform::OnBnClickedButtonimageaffine)
END_MESSAGE_MAP()

//初始化几何变换中的变量，设置初值
BOOL ImageGeoTransform::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	int i = -255;
	while (i < 255)
	{
		str.Format(_T("%d"), i);
		MoveX.AddString(str);
		MoveY.AddString(str);
		RotateAngle.AddString(str);
		SimilarityAngle.AddString(str);
		SimilarityScale.AddString(str);
		i += 1;
	}
	double j = 0;
	while (j < 5)
	{
		str.Format(_T("%f"), j);
		ImageScale.AddString(str);
		j += 0.1;
	}
	for (int flip = -1; flip < 2; flip++)
	{
		str.Format(_T("%d"), flip);
		FlipCode.AddString(str);
	}
	MoveX.SetCurSel(255);
	MoveY.SetCurSel(255);
	RotateAngle.SetCurSel(255);
	ImageScale.SetCurSel(10);
	SimilarityAngle.SetCurSel(255);
	SimilarityScale.SetCurSel(255);//设置初值
	FlipCode.SetCurSel(1);
	return 0;
}


// ImageGeoTransform 消息处理程序
//平移
void ImageGeoTransform::OnBnClickedButtonimagemove()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	CWnd* cWnd = GetParent();  //获取父窗口句柄
	//刷新picture控件 每次显示均只有当前的图像
	cWnd->GetDlgItem(IDC_ShowMidImage1)->ShowWindow(FALSE);
	cWnd->GetDlgItem(IDC_ShowMidImage1)->ShowWindow(TRUE);
	cWnd->GetDlgItem(IDC_ShowMidImage2)->ShowWindow(FALSE);
	cWnd->GetDlgItem(IDC_ShowMidImage2)->ShowWindow(TRUE);
	//获取X方向与Y方向平移的距离
	CString  moveX, moveY;
	int index;
	index = MoveX.GetCurSel();
	MoveX.GetLBText(index, moveX);  //获取控件索引对应的值
	double dMoveX = _ttof(moveX);
	index = MoveY.GetCurSel();
	MoveY.GetLBText(index, moveY);  //获取控件索引对应的值
	double dMoveY = _ttof(moveY);

	//平移矩阵
	Mat Move = Mat::zeros(2, 3, CV_32FC1);
	Move.at<float>(0, 0) = 1;
	Move.at<float>(0, 2) = dMoveX;   //水平平移量
	Move.at<float>(1, 1) = 1;
	Move.at<float>(1, 2) = dMoveY;   //垂直平移量  平移参数应由外部传入

	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if(middleWindow1 == 0 && middleWindow2 == 0)
	{
		warpAffine(srcImage, middleImage1, Move, srcImage.size());
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		fout << "//平移\n";
		fout << "warpAffine(srcImage, middleImage1, " << Move << "," << "srcImage.size());\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if(middleWindow2 >= middleWindow1)
	{
		warpAffine(middleImage2, middleImage1, Move, middleImage2.size());
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		//middleImage2.rows = 0;

		fout << "//平移\n";
		fout << "warpAffine(middleImage2, middleImage1, " << Move << ", " << "middleImage2.size());\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if(middleWindow1 > middleWindow2)
	{
		warpAffine(middleImage1, middleImage2, Move, middleImage1.size());
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		//middleImage1.rows = 0;

		fout << "//平移\n";
		fout << "warpAffine(middleImage1, middleImage2, " << Move << ", " << "middleImage1.size());\n";
	}
}


//图像旋转
void ImageGeoTransform::OnBnClickedButtonimagerotate()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	CWnd* cWnd = GetParent();  //获取父窗口句柄
	//刷新picture控件 每次显示均只有当前的图像
	cWnd->GetDlgItem(IDC_ShowMidImage1)->ShowWindow(FALSE);
	cWnd->GetDlgItem(IDC_ShowMidImage1)->ShowWindow(TRUE);
	cWnd->GetDlgItem(IDC_ShowMidImage2)->ShowWindow(FALSE);
	cWnd->GetDlgItem(IDC_ShowMidImage2)->ShowWindow(TRUE);
	//旋转角度  作为开放参数
	CString  angle;                 
	int index = RotateAngle.GetCurSel();
	RotateAngle.GetLBText(index, angle);
	double dAngle = _ttof(angle);

	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if(middleWindow1 == 0 && middleWindow2 == 0)
	{
		Point2f center(srcImage.cols / 2.0, srcImage.rows / 2.0);
		Mat Rotate = getRotationMatrix2D(center, dAngle, 1.0);
		//使旋转后的图像没有缺失
		Rect2f bbox = RotatedRect(Point2f(), srcImage.size(), dAngle).boundingRect2f();
		Rotate.at<double>(0, 2) += bbox.width / 2.0 - srcImage.cols / 2.0;
		Rotate.at<double>(1, 2) += bbox.height / 2.0 - srcImage.rows / 2.0;

		warpAffine(srcImage, middleImage1, Rotate, bbox.size());
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		fout << "//旋转\n";
		fout << "warpAffine(srcImage, middleImage2, " << Rotate << ", " << "srcImage.size());\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if(middleWindow2 >= middleWindow1)
	{
		Point2f center(middleImage2.cols / 2, middleImage2.rows / 2);
		Mat Rotate = getRotationMatrix2D(center, dAngle, 1.0);

		Rect2f bbox = RotatedRect(Point2f(), middleImage2.size(), dAngle).boundingRect2f();
		Rotate.at<double>(0, 2) += bbox.width / 2.0 - middleImage2.cols / 2.0;
		Rotate.at<double>(1, 2) += bbox.height / 2.0 - middleImage2.rows / 2.0;

		warpAffine(middleImage2, middleImage1, Rotate, bbox.size());
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		//middleImage2.rows = 0;

		fout << "//旋转\n";
		fout << "warpAffine(middleImage2, middleImage1, " << Rotate << ", " << "middleImage2.size());\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if(middleWindow1 > middleWindow2)
	{
		Point2f center(middleImage1.cols / 2, middleImage1.rows / 2);
		Mat Rotate = getRotationMatrix2D(center, dAngle, 1.0);

		Rect2f bbox = RotatedRect(Point2f(), middleImage1.size(), dAngle).boundingRect2f();
		Rotate.at<double>(0, 2) += bbox.width / 2.0 - middleImage1.cols / 2.0;
		Rotate.at<double>(1, 2) += bbox.height / 2.0 - middleImage1.rows / 2.0;

		warpAffine(middleImage1, middleImage2, Rotate, bbox.size());
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		//middleImage1.rows = 0;

		fout << "//旋转\n";
		fout << "warpAffine(middleImage1, middleImage2, " << Rotate << ", " << "middleImage1.size());\n";
	}
}

//图像翻转
void ImageGeoTransform::OnBnClickedButtonimageflip()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	CWnd* cWnd = GetParent();  //获取父窗口句柄
	//刷新picture控件 每次显示均只有当前的图像
	cWnd->GetDlgItem(IDC_ShowMidImage1)->ShowWindow(FALSE);
	cWnd->GetDlgItem(IDC_ShowMidImage1)->ShowWindow(TRUE);
	cWnd->GetDlgItem(IDC_ShowMidImage2)->ShowWindow(FALSE);
	cWnd->GetDlgItem(IDC_ShowMidImage2)->ShowWindow(TRUE);
	//翻转方式 0垂直翻转  大于0水平翻转 小于0同时进行垂直和水平翻转
	CString  flipCode;
	int index = FlipCode.GetCurSel();
	FlipCode.GetLBText(index, flipCode);
	int dFlipCode = _ttoi(flipCode);
	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if (middleWindow1 == 0 && middleWindow2 == 0)
	{
		flip(srcImage, middleImage1, dFlipCode);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		fout << "//图像翻转\n";
		fout << "flip(srcImage, middleImage1, " << dFlipCode << "); \n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if(middleWindow2 >= middleWindow1)
	{
		flip(middleImage2, middleImage1, dFlipCode);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		//middleImage2.rows = 0;
		fout << "//图像翻转\n";
		fout << "flip(middleImage2, middleImage1, " << dFlipCode << "); \n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if(middleWindow1 > middleWindow2)
	{
		flip(middleImage1, middleImage2, dFlipCode);
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		//middleImage1.rows = 0;

		fout << "//图像翻转\n";
		fout << "flip(middleImage1, middleImage2, " << dFlipCode << "); \n";
	}
}

//图像缩放
void ImageGeoTransform::OnBnClickedButtonimagescale()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	//缩放比例
	CString  scale;
	int index = ImageScale.GetCurSel();
	ImageScale.GetLBText(index, scale);
	double dScale = _ttof(scale);

	CWnd* cWnd = GetParent();  //获取父窗口句柄
	//刷新picture控件 每次显示均只有当前的图像
	cWnd->GetDlgItem(IDC_ShowMidImage1)->ShowWindow(FALSE);
	cWnd->GetDlgItem(IDC_ShowMidImage1)->ShowWindow(TRUE);
	cWnd->GetDlgItem(IDC_ShowMidImage2)->ShowWindow(FALSE);
	cWnd->GetDlgItem(IDC_ShowMidImage2)->ShowWindow(TRUE);

	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if(middleWindow1 == 0 && middleWindow2 == 0)
	{
		resize(srcImage, middleImage1, Size(round(srcImage.cols * dScale), round(srcImage.rows * dScale)), 0, 0, CV_INTER_LINEAR);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		fout << "resize(srcImage, middleImage1, Size(round(srcImage.cols * " << dScale << "), " << "round(srcImage.rows * " << dScale << ")), " << "0, 0, CV_INTER_LINEAR);\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if(middleWindow2 >= middleWindow1)
	{
		resize(middleImage2, middleImage1, Size(round(middleImage2.cols * dScale), round(middleImage2.rows * dScale)), 0, 0, CV_INTER_LINEAR);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		//middleImage2.rows = 0;

		fout << "resize(middleImage2, middleImage1, Size(round(middleImage2.cols * " << dScale << "), " << "round(middleImage2.rows * " << dScale << ")), " << "0, 0, CV_INTER_LINEAR);\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if(middleWindow1 > middleWindow2)
	{
		resize(middleImage1, middleImage2, Size(round(middleImage1.cols * dScale), round(middleImage1.rows * dScale)), 0, 0, CV_INTER_LINEAR);
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		//middleImage1.rows = 0;
		fout << "resize(middleImage1, middleImage2, Size(round(middleImage1.cols * " << dScale << "), " << "round(middleImage1.rows * " << dScale << ")), " << "0, 0, CV_INTER_LINEAR);\n";
	}
}

//相似变换
void ImageGeoTransform::OnBnClickedButtonimagesimilarity()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	//参数 旋转角度与缩放比例
	CString  angle;
	int index;
	index = SimilarityAngle.GetCurSel();
	SimilarityAngle.GetLBText(index, angle);
	double dAngle = _ttof(angle);
	CString  scale;
	index = SimilarityScale.GetCurSel();
	SimilarityScale.GetLBText(index, scale);
	double dScale = _ttof(scale);
	
	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if(middleWindow1 == 0 && middleWindow2 == 0)
	{
		Point2f center = Point2f(srcImage.cols / 2, srcImage.rows / 2);
		Mat Sim = getRotationMatrix2D(center, dAngle, dScale);
		warpAffine(srcImage, middleImage1, Sim, srcImage.size());
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		fout << "//相似变换\n";
		fout << "warpAffine(srcImage, middleImage1, " << Sim << ", " << "srcImage.size());\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if(middleWindow2 >= middleWindow1)
	{
		Point2f center = Point2f(middleImage2.cols / 2, middleImage2.rows / 2);
		Mat Sim = getRotationMatrix2D(center, dAngle, dScale);
		warpAffine(middleImage2, middleImage1, Sim, middleImage2.size());
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		//middleImage2.rows = 0;

		fout << "//相似变换\n";
		fout << "warpAffine(middleImage2, middleImage1, " << Sim << ", " << "middleImage2.size());\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if(middleWindow1 > middleWindow2)
	{
		Point2f center = Point2f(middleImage1.cols / 2, middleImage1.rows / 2);
		Mat Sim = getRotationMatrix2D(center, dAngle, dScale);
		warpAffine(middleImage1, middleImage2, Sim, middleImage1.size());
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		//middleImage1.rows = 0;

		fout << "//相似变换\n";
		fout << "warpAffine(middleImage1, middleImage2, " << Sim << ", " << "middleImage1.size());\n";
	}
}

//透视变换
void ImageGeoTransform::OnBnClickedButtonimageperpective()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	if(srcPoints[0] == Point2f(0, 0))
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
	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if(middleWindow1 == 0 && middleWindow2 == 0)
	{
		warpPerspective(srcImage, middleImage1, Perspective, srcImage.size());
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		fout << "//透视变换\n";
		fout << "warpPerspective(srcImage, middleImage1, " << Perspective << ", " << "srcImage.size());\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if(middleWindow2 >= middleWindow1)
	{
		warpPerspective(srcImage, middleImage1, Perspective, middleImage2.size());
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		//middleImage2.rows = 0;

		fout << "//透视变换\n";
		fout << "warpPerspective(middleImage2, middleImage1, " << Perspective << ", " << "middleImage2.size());\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if(middleWindow1 > middleWindow2)
	{
		warpPerspective(srcImage, middleImage2, Perspective, middleImage1.size());
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		//middleImage1.rows = 0;

		fout << "//透视变换\n";
		fout << "warpPerspective(middleImage1, middleImage2, " << Perspective << ", " << "middleImage1.size());\n";
	}
}

//仿射变换
void ImageGeoTransform::OnBnClickedButtonimageaffine()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	if(srcPointsA[0] == Point2f(0, 0) || dstPointsA[0] == Point2f(0, 0))
	{
		MessageBox(_T("请用鼠标右键标记仿射变换的源点和目的点"));
		return;
	}
	Mat Affine = getAffineTransform(srcPointsA, dstPointsA);//由三个点对计算透视变换矩阵  
	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if (middleWindow1 == 0 && middleWindow2 == 0)
	{
		warpAffine(srcImage, middleImage1, Affine, srcImage.size());
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		fout << "//仿射变换\n";
		fout << "warpAffine(srcImage, middleImage1, " << Affine << ", " << "srcImage.size());\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if(middleWindow2 >= middleWindow1)
	{
		warpAffine(middleImage2, middleImage1, Affine, middleImage2.size());
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		//middleImage2.rows = 0;

		fout << "//仿射变换\n";
		fout << "warpAffine(middleImage2, middleImage1, " << Affine << ", " << "middleImage2.size());\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if (middleWindow1 > middleWindow2)
	{
		warpAffine(middleImage1, middleImage2, Affine, middleImage1.size());
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		//middleImage1.rows = 0;

		fout << "//仿射变换\n";
		fout << "warpAffine(middleImage1, middleImage2, " << Affine << ", " << "middleImage1.size());\n";
	}
}
