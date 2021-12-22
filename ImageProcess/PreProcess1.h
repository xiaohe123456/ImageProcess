#pragma once


// PreProcess1 对话框

class PreProcess1 : public CDialogEx
{
	DECLARE_DYNAMIC(PreProcess1)

public:
	PreProcess1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PreProcess1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGPreProcess1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonimagefilter1();
	afx_msg void OnBnClickedButtonimageenhance1();
//	afx_msg void OnButtonmeanfilter();
	afx_msg void OnBnClickedButtonadjustfilter();
	afx_msg void OnBnClickedButtonenhanceadjust1();
};
