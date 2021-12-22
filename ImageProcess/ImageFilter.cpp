// ImageFilter.cpp: 实现文件
//

#include "pch.h"
#include "ImageProcess.h"
#include "ImageFilter.h"
#include "afxdialogex.h"
#include "CommonData.h"
#include "ImageProcessDlg.h"
// ImageFilter 对话框

IMPLEMENT_DYNAMIC(ImageFilter, CDialogEx)

ImageFilter::ImageFilter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGImageFilter, pParent)
{

}

ImageFilter::~ImageFilter()
{
}

void ImageFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBOBoxFilter, BoxFilter);
	DDX_Control(pDX, IDC_COMBOMeanFilter, MeanFilter);
	DDX_Control(pDX, IDC_COMBOMedianFilter, MedianFilter);
	DDX_Control(pDX, IDC_COMBOGaussianFilterKernel, GaussianFilterKernel);
	DDX_Control(pDX, IDC_COMBOGaussianFilterSigmaX, GaussianFilterSigamaX);
	DDX_Control(pDX, IDC_COMBOGaussianFilterSigmaY, GaussianFilterSigamaY);
	DDX_Control(pDX, IDC_COMBOBilateralFilterD, BilateralFilterDiameter);
	DDX_Control(pDX, IDC_COMBOBilateralFilterColor, BilateralFilterColor);
	DDX_Control(pDX, IDC_COMBOBilateralFilterLocate, BilateralFilterLocateSigma);
}

BEGIN_MESSAGE_MAP(ImageFilter, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTONBoxFilter, &ImageFilter::OnBnClickedButtonboxfilter)
	ON_BN_CLICKED(IDC_BUTTONMeanFilter, &ImageFilter::OnBnClickedButtonmeanfilter)
	ON_BN_CLICKED(IDC_BUTTONMedianFilter, &ImageFilter::OnBnClickedButtonmedianfilter)
	ON_BN_CLICKED(IDC_BUTTONGaussianFilter, &ImageFilter::OnBnClickedButtongaussianfilter)
	ON_BN_CLICKED(IDC_BUTTONBilateralFilter, &ImageFilter::OnBnClickedButtonbilateralfilter)
END_MESSAGE_MAP()

BOOL ImageFilter::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	int i = 1;
	//设置方框、均值和中值滤波器核控件的大小
	while (i < 20)
	{
		str.Format(_T("%d"), i);
		BoxFilter.AddString(str);
		MeanFilter.AddString(str);
		MedianFilter.AddString(str);
		GaussianFilterKernel.AddString(str);
		i += 2;
	}
	BoxFilter.SetCurSel(2);
	MeanFilter.SetCurSel(2);
	MedianFilter.SetCurSel(2);
	GaussianFilterKernel.SetCurSel(2); //设置初值
	//高斯滤波
	double xy = 0.8;
	while (xy < 2.5)
	{
		str.Format(_T("%f"), xy);
		GaussianFilterSigamaX.AddString(str);
		GaussianFilterSigamaY.AddString(str);
		xy += 0.1;
	}
	GaussianFilterSigamaX.AddString(_T("0"));
	GaussianFilterSigamaY.AddString(_T("0"));
	GaussianFilterSigamaX.SetCurSel(1);
	GaussianFilterSigamaY.SetCurSel(0);
	//双边滤波
	for (int d = 1; d < 10; d++)
	{
		str.Format(_T("%d"), d);
		BilateralFilterDiameter.AddString(str);
	}
	for (int sigma = 10; sigma < 150; sigma++)
	{
		str.Format(_T("%d"), sigma);
		BilateralFilterColor.AddString(str);
		BilateralFilterLocateSigma.AddString(str);
	}
	BilateralFilterDiameter.SetCurSel(5);
	BilateralFilterColor.SetCurSel(0);
	BilateralFilterLocateSigma.SetCurSel(0);
	return 0;
}

// ImageFilter 消息处理程序
//方框滤波
/*
boxFilter	(	InputArray 	src,		输入图像
				OutputArray 	dst,	输出图像
				int 	ddepth,			输出图像深度（-1或src.depth())
				Size 	ksize,			滤波器核大小
				Point 	anchor = Point(-1,-1),	锚点，默认Point(-1,-1) 表示核中心为锚点
				bool 	normalize = true,	    是否归一化，默认为true，true表示归一化
				int 	borderType = BORDER_DEFAULT  边界类型
			)

*/
void ImageFilter::OnBnClickedButtonboxfilter()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	int index = BoxFilter.GetCurSel();   //获取控件索引
	CString kSize;
	MeanFilter.GetLBText(index, kSize);  //获取控件索引对应的值
	int ksize = _ttoi(kSize);
	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if (middleWindow1 == 0 && middleWindow2 == 0)
	{
		boxFilter(srcImage, middleImage1, -1, Size(ksize, ksize));
		imshow(MiddleWindowName1, middleImage1);
		imageFilter = middleImage1;
		middleWindow1++;
		fout << "boxFilter(srcImage, middleImage1, -1, Size(" << ksize << ", " << ksize <<"));\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if (middleWindow2 >= middleWindow1)
	{
		if (flagFilter == 1)
			middleImage2 = srcImage;
		boxFilter(middleImage2, middleImage1, -1, Size(ksize, ksize));
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		imageFilter = middleImage1;
		//middleImage1.rows = 0;
		fout << "boxFilter(middleImage2, middleImage1, -1, Size(" << ksize << ", " << ksize << "));\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if (middleWindow1 > middleWindow2)
	{
		if (flagFilter == 1)
			middleImage1 = srcImage;
		boxFilter(middleImage1, middleImage2, -1, Size(ksize, ksize));
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		imageFilter = middleImage2;
		//middleImage1.rows = 0;
		fout << "boxFilter(middleImage1, middleImage2, -1, Size(" << ksize << ", " << ksize << "));\n";
	}
	flagFilter = 1;
}

//均值滤波
/*
blur(InputArray 	src,  输入图像
	 OutputArray 	dst,  输出图像
	 Size 	ksize,		  滤波核大小
	 Point 	anchor = Point(-1, -1),  锚点，默认值Point（-1，-1），该值若为负值，表示取核的中心为锚点
	 int 	borderType = BORDER_DEFAULT  边界类型
	 )
*/
void ImageFilter::OnBnClickedButtonmeanfilter()
{
	// TODO: 在此添加控件通知处理程序代码

	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}

	int index = MeanFilter.GetCurSel();  //获取控件索引
	CString kSize;
	MeanFilter.GetLBText(index, kSize);  //获取控件索引对应的值
	int ksize = _ttoi(kSize);
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
	else if (middleWindow2 >= middleWindow1)
	{
		if (flagFilter == 1)
			middleImage2 = srcImage; 
		blur(middleImage2, middleImage1, Size(ksize, ksize));
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		imageFilter = middleImage1;
		//middleImage2.rows = 0;
		fout << "blur(middleImage2, middleImage1, Size(" << ksize << ", " << ksize << "));\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if (middleWindow1 > middleWindow2)
	{
		if (flagFilter == 1)
			middleImage1 = srcImage;
		blur(middleImage1, middleImage2, Size(11, 11));
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		imageFilter = middleImage2;
		//middleImage1.rows = 0;
		fout << "blur(middleImage1, middleImage2, Size(" << ksize << ", " << ksize << "));\n";
	}
	flagFilter = 1;	
}

/*
medianBlur	(	InputArray 	src,      输入图像，为1、3、4通道的图像，当ksize为3或5时，图像深度只能为CV_8U、CV_16U
												  CV_32F中的一个，对于较大孔径尺寸的图片，图像深度只能为CV_8U
				OutputArray 	dst,  输出图像
				int 	ksize         滤波模板尺寸大小，必须是大于1的奇数
			)
中值滤波使用BORDER_REPLICATE处理边界像素
*/
void ImageFilter::OnBnClickedButtonmedianfilter()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	int index = MedianFilter.GetCurSel();  //获取控件索引
	CString kSize;
	MeanFilter.GetLBText(index, kSize);   //获取控件索引对应的值
	int ksize = _ttoi(kSize);
	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if (middleWindow1 == 0 && middleWindow2 == 0)
	{
		medianBlur(srcImage, middleImage1, ksize);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		imageFilter = middleImage1;
		fout << "medianBlur(srcImage, middleImage1, " << ksize << ");\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if (middleWindow2 >= middleWindow1)
	{
		if (flagFilter == 1)
			middleImage2 = srcImage;
		medianBlur(middleImage2, middleImage1, ksize);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		imageFilter = middleImage1;
		//middleImage2.rows = 0;
		fout << "medianBlur(middleImage2, middleImage1, " << ksize << ");\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if (middleWindow1 > middleWindow2)
	{
		if (flagFilter == 1)
			middleImage1 = srcImage;
		medianBlur(middleImage1, middleImage2, ksize);
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		imageFilter = middleImage2;
		//middleImage1.rows = 0;
		fout << "medianBlur(middleImage1, middleImage2, " << ksize << ");\n";
	}
	flagFilter = 1;
}

//高斯滤波
/*
GaussianBlur	(	InputArray 	src,      输入图像
					OutputArray 	dst,  输出图像
					Size 	ksize,		  高斯核大小，高斯核的宽度和高度可以不同但必须是正数且为奇数，也可以为0，将由sigma计算
					double 	sigmaX,		  X方向的高斯核标准偏差
					double 	sigmaY = 0,   Y方向的高斯核标准偏差，若sigmaY为0，则它与sigmaX相等，如果均为0，则由高斯核计算得到
					int 	borderType = BORDER_DEFAULT  边界填充类型
				)
*/
void ImageFilter::OnBnClickedButtongaussianfilter()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}
	int index;
	index = GaussianFilterKernel.GetCurSel();  //获取控件索引
	CString kSize;
	GaussianFilterKernel.GetLBText(index, kSize);   //获取控件索引对应的值
	int ksize = _ttoi(kSize);
	index = GaussianFilterSigamaX.GetCurSel();
	CString SigmaX;
	GaussianFilterSigamaX.GetLBText(index, SigmaX);
	int sigmaX = _ttoi(SigmaX);
	index = GaussianFilterSigamaY.GetCurSel();
	CString SigmaY;
	GaussianFilterSigamaY.GetLBText(index, SigmaY);
	int sigmaY = _ttoi(SigmaY);


	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if(middleWindow1 == 0 && middleWindow2 == 0)
	{
		GaussianBlur(srcImage, middleImage1, Size(ksize, ksize), sigmaX, sigmaY);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		imageFilter = middleImage1;
		fout << "GaussianBlur(srcImage, middleImage1, Size(" << ksize << ", " << ksize << "), " << sigmaX << ", " << sigmaY << ");\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if (middleWindow2 >= middleWindow1)
	{
		if (flagFilter == 1)
			middleImage2 = srcImage;
		GaussianBlur(middleImage2, middleImage1, Size(ksize, ksize), sigmaX, sigmaY);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		imageFilter = middleImage1;
		//middleImage2.rows = 0;
		fout << "GaussianBlur(middleImage2, middleImage1, Size(" << ksize << ", " << ksize << "), " << sigmaX << ", " << sigmaY << ");\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if (middleWindow1 > middleWindow2)
	{
		if (flagFilter == 1)
			middleImage1 = srcImage;
		GaussianBlur(middleImage1, middleImage2, Size(ksize, ksize), sigmaX, sigmaY);
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		imageFilter = middleImage2;
		//middleImage1.rows = 0;
		fout << "GaussianBlur(middleImage1, middleImage2, Size(" << ksize << ", " << ksize << "), " << sigmaX << ", " << sigmaY << ");\n";
	}
	flagFilter = 1;
}

//双边滤波
/*
bilateralFilter	(	InputArray 	src,        输入源图像
					OutputArray 	dst,    结果图像
					int 	d,              滤波过程中使用的每个像素邻域的直径，如果为负数，则由sigmaSpace计算
					double 	sigmaColor,     颜色空间中滤波器sigma的值，该值越大则会将像素邻域内越远的颜色混合在一起，从而产生更大的半相等颜色区域
					double 	sigmaSpace,     坐标空间中滤波器sigma的值，该值越大，越远的像素只要颜色相近它们就会相互影响，当d>0时，d指定邻域大小且与该参数无关，否则d正比于sigmaSpace
					int 	borderType = BORDER_DEFAULT  边界类型，指定如何确定图像范围外的像素取值（处理边缘像素时）
				)
参数sigma:可以令两个sigma参数的值相等，如果它们很小（小于10），滤波器没有什么效果；若它们很大（大于150），滤波器
效果会很强，使图像显得非常卡通化
参数d:d很大（d>5）会很慢，建议对实时应用设为5，对于需要过滤严重噪声的离线应用，可以将d设为9。
*/
void ImageFilter::OnBnClickedButtonbilateralfilter()
{
	// TODO: 在此添加控件通知处理程序代码

	if (srcImage.empty())
	{
		MessageBox(_T("加载图片失败"));
		return;
	}

	int index;
	index = BilateralFilterDiameter.GetCurSel();  //获取控件索引
	CString cd;
	BilateralFilterDiameter.GetLBText(index, cd);   //获取控件索引对应的值
	int d = _ttoi(cd);
	index = BilateralFilterColor.GetCurSel();
	CString SigmaColor;
	BilateralFilterColor.GetLBText(index, SigmaColor);
	int sigmaColor = _ttoi(SigmaColor);
	index = BilateralFilterLocateSigma.GetCurSel();
	CString SigmaSpace;
	BilateralFilterLocateSigma.GetLBText(index, SigmaSpace);
	int sigmaSpace = _ttoi(SigmaSpace);

	//if (middleImage1.rows == 0 && middleImage2.rows == 0)
	if (middleWindow1 == 0 && middleWindow2 == 0)
	{
		bilateralFilter(srcImage, middleImage1, d, sigmaColor, sigmaSpace);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		imageFilter = middleImage1;
		fout << "双边滤波\n";
		fout << "bilateralFilter(srcImage, middleImage1, " << d << ", " << sigmaColor << ", " << sigmaSpace << ");\n";
	}
	//else if (middleImage1.rows == 0 && middleImage2.rows != 0)
	else if (middleWindow2 >= middleWindow1)
	{
		if (flagFilter == 1)
			middleImage2 = srcImage;
		bilateralFilter(middleImage2, middleImage1, d, sigmaColor, sigmaSpace);
		imshow(MiddleWindowName1, middleImage1);
		middleWindow1++;
		imageFilter = middleImage1;
		//middleImage2.rows = 0;
		fout << "双边滤波\n";
		fout << "bilateralFilter(middleImage2, middleImage1, " << d << ", " << sigmaColor << ", " << sigmaSpace << ");\n";
	}
	//else if (middleImage2.rows == 0 && middleImage1.rows != 0)
	else if (middleWindow1 > middleWindow2)
	{
		if (flagFilter == 1)
			middleImage1 = srcImage;
		bilateralFilter(middleImage1, middleImage2, d, sigmaColor, sigmaSpace);
		imshow(MiddleWindowName2, middleImage2);
		middleWindow2++;
		imageFilter = middleImage2;
		//middleImage1.rows = 0;
		fout << "双边滤波\n";
		fout << "bilateralFilter(middleImage1, middleImage2, " << d << ", " << sigmaColor << ", " << sigmaSpace << ");\n";
	}
	flagFilter = 1;
}
