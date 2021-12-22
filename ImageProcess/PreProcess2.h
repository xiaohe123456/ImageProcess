#pragma once


// PreProcess2 对话框

class PreProcess2 : public CDialogEx
{
	DECLARE_DYNAMIC(PreProcess2)

public:
	PreProcess2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PreProcess2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGPreProcess2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButtongeotransform2();
	afx_msg void OnBnClickedButtonimageenhance2();
	afx_msg void OnBnClickedButtonadjustgeotransform();
	afx_msg void OnBnClickedButtonadjustimageenhance();
};
