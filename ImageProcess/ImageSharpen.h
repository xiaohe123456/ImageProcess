#pragma once


// ImageSharpen 对话框

class ImageSharpen : public CDialogEx
{
	DECLARE_DYNAMIC(ImageSharpen)

public:
	ImageSharpen(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ImageSharpen();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGImageSharpen };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	// sobel算子核的大小
	CComboBox SobelKernel;
	// sobel算子缩放因子
	CComboBox SobelScale;
	// sobel算子中delta值
	CComboBox SobelDelta;
	afx_msg void OnBnClickedButtonsobel();
	// X或Y方向  0表示x方向  1表示y方向	
	CComboBox XorY;
	// 拉普拉斯核大小
	CComboBox LaplacianKernel;
	afx_msg void OnBnClickedButtonlaplacian();
};
