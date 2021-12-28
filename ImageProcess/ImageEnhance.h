#pragma once


// ImageEnhance 对话框
#include "ImageProcessDlg.h"

class ImageEnhance : public CDialogEx
{
	DECLARE_DYNAMIC(ImageEnhance)

public:
	ImageEnhance(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ImageEnhance();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGImageEnhance };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButtonlineenhance();
	afx_msg void OnBnClickedButtongammaenhance();
	afx_msg void OnBnClickedButtonequhist();
	afx_msg void OnBnClickedButtonlogenhance();
	afx_msg void OnBnClickedButtonsegline();
	afx_msg void OnBnClickedButtonclahe();
	// 线性变换图像增强斜率K	
	CComboBox LineK;
	// 线性变换图像增强截距B
	CComboBox LineB;
	// 伽马增强系数
	CComboBox GammaCof;
	// 伽马变换指数
	CComboBox GammaIndex;
	// 自适应直方图均衡化对比度阈值
	CComboBox ContrastThresh;
	// 自适应直方图均衡化网格大小
	CComboBox EqualizeGrid;
	// 对数变换系数
	CComboBox LogC;
	// 分段线性变换斜率K1
	CComboBox SegLineK1;
	// 分段线性变换截距B1
	CComboBox SegLineB1;
	// 分段线性变换斜率K2
	CComboBox SegLineK2;
	// 分段线性变换B2
	CComboBox SegLineB2;
	// 分段线性变换分段值1
	CComboBox SegLineGray1;
	// 分段线性变换分段值2
	CComboBox SegLineGray2;
	afx_msg void OnBnClickedButtonintroduceline();
	afx_msg void OnBnClickedButtonintroducegamma();
	afx_msg void OnBnClickedButtonintroduceahe();
	afx_msg void OnBnClickedButtonintroduceequhist();
	afx_msg void OnBnClickedButtonintroducelog();
};
