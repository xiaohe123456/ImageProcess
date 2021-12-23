#pragma once


// ImageFilter 对话框
#include "ImageProcessDlg.h"

class ImageFilter : public CDialogEx
{
	DECLARE_DYNAMIC(ImageFilter)

public:
	ImageFilter(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ImageFilter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGImageFilter };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButtonboxfilter();
	afx_msg void OnBnClickedButtonmeanfilter();
	afx_msg void OnBnClickedButtonmedianfilter();
	afx_msg void OnBnClickedButtongaussianfilter();
	afx_msg void OnBnClickedButtonbilateralfilter();
	

	// 方框滤波核大小
	CComboBox BoxFilter;
	// 均值滤波核大小
	CComboBox MeanFilter;
	// 中值滤波核大小
	CComboBox MedianFilter;
	// 高斯核大小
	CComboBox GaussianFilterKernel;
	// 高斯分布X标准差
	CComboBox GaussianFilterSigamaX;
	// 高斯分布Y标准差
	CComboBox GaussianFilterSigamaY;
	// 双边滤波的直径	// 双边滤波的直径
	CComboBox BilateralFilterDiameter;
	// 双边滤波的颜色空间Sigma值
	CComboBox BilateralFilterColor;
	// 双边滤波的坐标空间Sigma
	CComboBox BilateralFilterLocateSigma;
	afx_msg void OnBnClickedButtonintroducebox();
};
