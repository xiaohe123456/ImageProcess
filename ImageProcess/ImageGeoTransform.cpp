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
	MoveX.SetCurSel(255);				//设置初值
	MoveY.SetCurSel(255);
	RotateAngle.SetCurSel(255);
	ImageScale.SetCurSel(10);
	SimilarityAngle.SetCurSel(255);
	SimilarityScale.SetCurSel(255);
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

	if(middleWindow1 == 0 && middleWindow2 == 0)
	{
		warpAffine(srcImage, middleImage1, Move, srcImage.size());
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if(middleWindow2 >= middleWindow1)
	{
		warpAffine(middleImage2, middleImage1, Move, middleImage2.size());
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if(middleWindow1 > middleWindow2)
	{
		warpAffine(middleImage1, middleImage2, Move, middleImage1.size());
		imshow(MiddleWindowName2, middleImage2);
		resultMidWindow2.push_back(middleImage2);
		middleWindow2++;
	}
	fs_write << "Operation" << "{";
	fs_write << "function" << "warpAffine";
	fs_write << "effect" << "image  rotate";
	fs_write << "}";
	Move.convertTo(Move, CV_32F);  //平移矩阵格式转换  方便只将变换矩阵保存到yaml文件
	float out[3][3];
	fs_write << "Param" << "{";
	fs_write << "image size" << "{";
	fs_write << "rows" << srcImage.rows;
	fs_write << "cols" << srcImage.cols << "}";
	fs_write << "move x" << dMoveX;
	fs_write << "move y" << dMoveY;
	fs_write << "Matrix" << "[:";			//平移矩阵
	for (int i = 0; i < Move.rows; i++)
	{
		for (int j = 0; j < Move.cols; j++)
		{
			out[i][j] = Move.at<float>(i, j);
			fs_write << out[i][j];
		}
	}
	fs_write << "]";
	fs_write << "Size" << "{";				//平移矩阵大小
	fs_write << "rows" << Move.rows << "cols" << Move.cols << "}";
	fs_write << "}";
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
	Mat Rotate;
	if(middleWindow1 == 0 && middleWindow2 == 0)
	{
		Point2f center(srcImage.cols / 2.0, srcImage.rows / 2.0);
		Rotate = getRotationMatrix2D(center, dAngle, 1.0);
		//使旋转后的图像没有缺失
		Rect2f bbox = RotatedRect(Point2f(), srcImage.size(), dAngle).boundingRect2f();
		Rotate.at<double>(0, 2) += bbox.width / 2.0 - srcImage.cols / 2.0;
		Rotate.at<double>(1, 2) += bbox.height / 2.0 - srcImage.rows / 2.0;
		warpAffine(srcImage, middleImage1, Rotate, bbox.size());
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if(middleWindow2 >= middleWindow1)
	{
		Point2f center(middleImage2.cols / 2, middleImage2.rows / 2);
		Rotate = getRotationMatrix2D(center, dAngle, 1.0);
		Rect2f bbox = RotatedRect(Point2f(), middleImage2.size(), dAngle).boundingRect2f();
		Rotate.at<double>(0, 2) += bbox.width / 2.0 - middleImage2.cols / 2.0;
		Rotate.at<double>(1, 2) += bbox.height / 2.0 - middleImage2.rows / 2.0;
		warpAffine(middleImage2, middleImage1, Rotate, bbox.size());
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if(middleWindow1 > middleWindow2)
	{
		Point2f center(middleImage1.cols / 2, middleImage1.rows / 2);
		Rotate = getRotationMatrix2D(center, dAngle, 1.0);
		Rect2f bbox = RotatedRect(Point2f(), middleImage1.size(), dAngle).boundingRect2f();
		Rotate.at<double>(0, 2) += bbox.width / 2.0 - middleImage1.cols / 2.0;
		Rotate.at<double>(1, 2) += bbox.height / 2.0 - middleImage1.rows / 2.0;
		warpAffine(middleImage1, middleImage2, Rotate, bbox.size());
		imshow(MiddleWindowName2, middleImage2);
		resultMidWindow2.push_back(middleImage2);
		middleWindow2++;
	}
	fs_write << "Operation" << "{";
	fs_write << "function" << "warpAffine";
	fs_write << "effect" << "image  rotate";
	fs_write << "}";
	Rotate.convertTo(Rotate, CV_32F);  //旋转矩阵格式转换  方便只将变换矩阵保存到yaml文件
	float out[3][3];
	fs_write << "Param" << "{";
	fs_write << "image size" << "{";
	fs_write << "rows" << srcImage.rows;
	fs_write << "cols" << srcImage.cols << "}";
	fs_write << "angle" << dAngle;
	fs_write << "Matrix" << "[:";			//旋转变换矩阵
	for (int i = 0; i < Rotate.rows; i++)
	{
		for (int j = 0; j < Rotate.cols; j++)
		{
			out[i][j] = Rotate.at<float>(i, j);
			fs_write << out[i][j];
		}
	}
	fs_write << "]";
	fs_write << "Size" << "{";				//旋转变换矩阵大小
	fs_write << "rows" << Rotate.rows << "cols" << Rotate.cols << "}";
	fs_write << "}";
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
	char* flipWay;
	if (dFlipCode == 0)
		flipWay = "垂直翻转";
	else if (dFlipCode > 0)
		flipWay = "水平翻转";
	else
		flipWay = "水平加垂直翻转";
	if (middleWindow1 == 0 && middleWindow2 == 0)
	{
		flip(srcImage, middleImage1, dFlipCode);
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if(middleWindow2 >= middleWindow1)
	{
		flip(middleImage2, middleImage1, dFlipCode);
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if(middleWindow1 > middleWindow2)
	{
		flip(middleImage1, middleImage2, dFlipCode);
		imshow(MiddleWindowName2, middleImage2);
		resultMidWindow2.push_back(middleImage2);
		middleWindow2++;
	}
	fs_write << "Operation" << "{";
	fs_write << "function" << "flip" << "effect" << flipWay;
	fs_write << "}";
	fs_write << "Param" << "{" << "filpCode" << dFlipCode << "}";
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

	if(middleWindow1 == 0 && middleWindow2 == 0)
	{
		resize(srcImage, middleImage1, Size(round(srcImage.cols * dScale), round(srcImage.rows * dScale)), 0, 0, CV_INTER_LINEAR);
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if(middleWindow2 >= middleWindow1)
	{
		resize(middleImage2, middleImage1, Size(round(middleImage2.cols * dScale), round(middleImage2.rows * dScale)), 0, 0, CV_INTER_LINEAR);
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if(middleWindow1 > middleWindow2)
	{
		resize(middleImage1, middleImage2, Size(round(middleImage1.cols * dScale), round(middleImage1.rows * dScale)), 0, 0, CV_INTER_LINEAR);
		imshow(MiddleWindowName2, middleImage2);
		resultMidWindow2.push_back(middleImage2);
		middleWindow2++;
	}
	fs_write << "Operation" << "{";
	fs_write << "function" << "resize" << "effect" << "image resize";
	fs_write << "}";
	fs_write << "Param" << "{" << "scale" << dScale <<"}";
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
	Mat Sim;
	if(middleWindow1 == 0 && middleWindow2 == 0)
	{
		Point2f center = Point2f(srcImage.cols / 2, srcImage.rows / 2);
		Sim = getRotationMatrix2D(center, dAngle, dScale);
		warpAffine(srcImage, middleImage1, Sim, srcImage.size());
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if(middleWindow2 >= middleWindow1)
	{
		Point2f center = Point2f(middleImage2.cols / 2, middleImage2.rows / 2);
		Sim = getRotationMatrix2D(center, dAngle, dScale);
		warpAffine(middleImage2, middleImage1, Sim, middleImage2.size());
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if(middleWindow1 > middleWindow2)
	{
		Point2f center = Point2f(middleImage1.cols / 2, middleImage1.rows / 2);
		Sim = getRotationMatrix2D(center, dAngle, dScale);
		warpAffine(middleImage1, middleImage2, Sim, middleImage1.size());
		imshow(MiddleWindowName2, middleImage2);
		resultMidWindow2.push_back(middleImage2);
		middleWindow2++;
	}
	fs_write << "Operation" << "{";
	fs_write << "function" << "warpAffine";
	fs_write << "effect" << "image Similarity transform";
	fs_write << "}";
	Sim.convertTo(Sim, CV_32F);  //相似变换矩阵格式转换  方便只将变换矩阵保存到yaml文件
	float out[3][3];
	fs_write << "Param" << "{";
	fs_write << "Matrix" << "[:";			//相似变换矩阵
	for (int i = 0; i < Sim.rows; i++)
	{
		for (int j = 0; j < Sim.cols; j++)
		{
			out[i][j] = Sim.at<float>(i, j);
			fs_write << out[i][j];
		}
	}
	fs_write << "]";
	fs_write << "Size" << "{";				//相似变换矩阵大小
	fs_write << "rows" << Sim.rows << "cols" << Sim.cols << "}";
	fs_write << "}";
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

	//srcPoints[0] = Point2f(63, 252);
	//srcPoints[1] = Point2f(344, 167);
	//srcPoints[2] = Point2f(354, 310);
	//srcPoints[3] = Point2f(110, 431);
	if (srcPoints[0] == Point2f(0, 0))
	{
		MessageBox(_T("请用鼠标左键标记透视变换的源点"));
		return;
	}
	//透视变换目标点坐标
	dstPoints[0] = Point2f(0, 0);
	dstPoints[1] = Point2f(srcImage.cols, 0);
	dstPoints[2] = Point2f(srcImage.cols, srcImage.rows);
	dstPoints[3] = Point2f(0, srcImage.rows);
	
	Mat Perspective = getPerspectiveTransform(srcPoints, dstPoints);//由四个点对计算透视变换矩阵  
	if (middleWindow1 == 0 && middleWindow2 == 0)
	{
		warpPerspective(srcImage, middleImage1, Perspective, srcImage.size());
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if(middleWindow2 >= middleWindow1)
	{
		warpPerspective(srcImage, middleImage1, Perspective, middleImage2.size());
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if(middleWindow1 > middleWindow2)
	{
		warpPerspective(srcImage, middleImage2, Perspective, middleImage1.size());
		imshow(MiddleWindowName2, middleImage2);
		resultMidWindow2.push_back(middleImage2);
		middleWindow2++;
	}
	fs_write << "Operation" << "{";
	fs_write << "function" << "warpPerspective" ;
	fs_write << "effect" << "image perspective transform";
	fs_write << "}";
	Perspective.convertTo(Perspective, CV_32F);  //透视变换矩阵格式转换  方便只将变换矩阵保存到yaml文件
	float out[3][3];
	fs_write << "Param" << "{";
	fs_write << "image size" << "{";
	fs_write << "rows" << srcImage.rows;
	fs_write << "cols" << srcImage.cols << "}";
	fs_write << "Points" << "[";				//透视变换的源点
	for (int i = 0; i < countL; i++)
	{
		fs_write << srcPoints[i];
	}
	fs_write << "]";
	fs_write << "Matrix" << "[:";			//透视变换矩阵
	for (int i = 0; i < Perspective.rows; i++)
	{
		for (int j = 0; j < Perspective.cols; j++)
		{
			out[i][j] = Perspective.at<float>(i, j);
			fs_write << out[i][j];
		}
	}
	fs_write << "]";
	fs_write << "Size" << "{";				//透视变换矩阵大小
	fs_write << "rows" << Perspective.rows << "cols" << Perspective.cols << "}";
	fs_write  << "}";
	//fs_write << "param" << Perspective;
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
	if (middleWindow1 == 0 && middleWindow2 == 0)
	{
		warpAffine(srcImage, middleImage1, Affine, srcImage.size());
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if(middleWindow2 >= middleWindow1)
	{
		warpAffine(middleImage2, middleImage1, Affine, middleImage2.size());
		imshow(MiddleWindowName1, middleImage1);
		resultMidWindow1.push_back(middleImage1);
		middleWindow1++;
	}
	else if (middleWindow1 > middleWindow2)
	{
		warpAffine(middleImage1, middleImage2, Affine, middleImage1.size());
		imshow(MiddleWindowName2, middleImage2);
		resultMidWindow2.push_back(middleImage2);
		middleWindow2++;
	}
	fs_write << "Operation" << "{";
	fs_write << "function" << "warpAffine";
	fs_write << "effect" << "image Affine transform";
	fs_write << "}";
	Affine.convertTo(Affine, CV_32F);  //仿射变换矩阵格式转换  方便只将变换矩阵保存到yaml文件
	float out[3][3];
	fs_write << "Param" << "{";
	fs_write << "image size" << "{";
	fs_write << "rows" << srcImage.rows;
	fs_write << "cols" << srcImage.cols << "}";
	fs_write << "Points" << "[";				//仿射变换的源点
	for (int i = 0; i < countR; i++)
	{
		fs_write << srcPointsA[i];
	}
	fs_write << "]";
	fs_write << "Matrix" << "[:";			//仿射变换矩阵
	for (int i = 0; i < Affine.rows; i++)
	{
		for (int j = 0; j < Affine.cols; j++)
		{
			out[i][j] = Affine.at<float>(i, j);
			fs_write << out[i][j];
		}
	}
	fs_write << "]";
	fs_write << "Size" << "{";				//仿射变换矩阵大小
	fs_write << "rows" << Affine.rows << "cols" << Affine.cols << "}";
	fs_write << "}";
}