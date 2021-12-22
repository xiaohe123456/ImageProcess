
// ImageProcessDlg.h: 头文件
//

#pragma once

// CImageProcessDlg 对话框
class CImageProcessDlg : public CDialogEx
{
// 构造
public:
	CImageProcessDlg(CWnd* pParent = nullptr);	// 标准构造函数



// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGEPROCESS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CString  mPath; //图像路径
	CString mPath1, mPath2;
	
	double Zoom = 1;		//图像缩放基准
	double Radio = 0.1;	//图像缩放比例
	
	std::vector<CString> mFileList;						//文件名列表
	int numImageNow;									//当前图像序号
	int numImageMax;									//文件夹中图像数目

	afx_msg void OnBnClickedButtonopenimage();
	afx_msg void OnBnClickedButtonsaveimage();
	afx_msg void OnBnClickedButtonimagegray();
	afx_msg void OnBnClickedButtonbinary();
	afx_msg void OnBnClickedButtonimageenhance();
	afx_msg void OnBnClickedButtonimagefilter();
	afx_msg void OnBnClickedButtonimagegeotransform();
	afx_msg void OnNMCustomdrawSlidersmall(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderbig(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// 二值化中的小阈值
	CSliderCtrl MinThreshold;
	// 二值化中的大阈值
	CSliderCtrl MaxThreshold;
	
	afx_msg void OnBnClickedButtonedgedetect();
	afx_msg void OnBnClickedButtonpreprocess1();
	afx_msg void OnBnClickedButtonpreprocess2();
	afx_msg void OnBnClickedButtonimagesharpen();
	afx_msg void OnBnClickedButtonnextimage();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	afx_msg void OnBnClickedButtonpreimage();
	afx_msg void OnBnClickedButtonrevoke();
};
