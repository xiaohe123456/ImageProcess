#pragma once
//#include "ImageProcessDlg.h"

// ImageGeoTransform 对话框

class ImageGeoTransform : public CDialogEx
{
	DECLARE_DYNAMIC(ImageGeoTransform)

public:
	ImageGeoTransform(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ImageGeoTransform();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGGeoTransform };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	

	DECLARE_MESSAGE_MAP()
public:


	afx_msg void OnBnClickedButtonimagerotate();
	afx_msg void OnBnClickedButtonimagemove();
	afx_msg void OnBnClickedButtonimagescale();
	afx_msg void OnBnClickedButtonimagesimilarity();
	afx_msg void OnBnClickedButtonimageperpective();
	// 图像平移X方向分量
	CComboBox MoveX;
	// 图像平移Y方向分量
	CComboBox MoveY;
	// 图像旋转角度
	CComboBox RotateAngle;
	// 图像缩放比例
	CComboBox ImageScale;
	// 图像相似变换角度
	CComboBox SimilarityAngle;
	// 图像相似变换缩放比例
	CComboBox SimilarityScale;
	// 图像翻转方式
	CComboBox FlipCode;
	afx_msg void OnBnClickedButtonimageflip();
	afx_msg void OnBnClickedButtonimageaffine();
};


