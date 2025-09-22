
// MPITestDlg.h: 头文件
//

#pragma once


// CMPITestDlg 对话框
class CMPITestDlg : public CDialogEx
{
// 构造
public:
	CMPITestDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MPITEST_DIALOG };
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
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtMsg;
	CListBox m_lstPersons;
	CRichEditCtrl m_redtMsgs;
public:
	void appendMsg(const CString& msg, bool bWrap = true, COLORREF clr =RGB(0,0,0), bool bBold = false);
	// 置空线程句柄
	void clearThreadHandle();
	bool isExiting() const { return m_bExiting; }
private:
	int init();
	int uninit();
	// 发送消息
	int sendNodeMsg(int nDstRank, CString& msg, bool bBroadcast = false);
	

private:
	HANDLE m_handThread = NULL;
	BOOL m_bExiting = FALSE;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CEdit m_edtCurrentNode;
};
