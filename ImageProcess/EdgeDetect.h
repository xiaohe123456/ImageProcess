#pragma once


// EdgeDetect 对话框

class EdgeDetect : public CDialogEx
{
	DECLARE_DYNAMIC(EdgeDetect)

public:
	EdgeDetect(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~EdgeDetect();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGEdgeDetect };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtoncanny();
};
