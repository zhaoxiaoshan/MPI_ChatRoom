
// MPITestDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MPITest.h"
#include "MPITestDlg.h"
#include "afxdialogex.h"
#include <mpi.h>
#define MCW MPI_COMM_WORLD
#define WAR_DLG(msg) ::MessageBox(NULL, msg, _T("系统警告"), MB_OK | MB_ICONWARNING)
static int s_rank = -1;			// 当前稚

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum MSG_TAG
{
	COMMON_MSG = 0,			// 普通消息
	BROADCAST_MSG,			// 广播消息
	EXIT_MSG			// 退出消息
};
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMPITestDlg 对话框



CMPITestDlg::CMPITestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MPITEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMPITestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtMsg);
	DDX_Control(pDX, IDC_LIST1, m_lstPersons);
	DDX_Control(pDX, IDC_RICHEDIT21, m_redtMsgs);
	DDX_Control(pDX, IDC_EDIT2, m_edtCurrentNode);
}

BEGIN_MESSAGE_MAP(CMPITestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CMPITestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMPITestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMPITestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMPITestDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CMPITestDlg 消息处理程序

BOOL CMPITestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	this->init();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMPITestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMPITestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMPITestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


DWORD WINAPI listenProc(LPVOID lpParam)
{
	CMPITestDlg* pDlg = static_cast<CMPITestDlg*>(lpParam);
	MPI_Status status;
	while (!pDlg->isExiting())
	{
		int count = 0;
		int flag = 0;
		MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MCW, &flag, &status);  // Blocking function this only returns when there is a message to receive
		if (0 == flag)
		{
			Sleep(500);
			continue;
		}
		MPI_Get_count(&status, MPI_WCHAR, &count);  //  To get count from the status variable
		if (1 > count)
		{
			Sleep(500);
			continue;
		}
		wchar_t* pMsg = new wchar_t[count + 1];
		memset(pMsg, 0, (count + 1) * sizeof(wchar_t));
		MPI_Recv(pMsg, count, MPI_WCHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MCW, &status);
		if (status.MPI_TAG == EXIT_MSG)
		{
			pDlg->appendMsg(_T("[系统] 退出消息！"), true, RGB(255, 0, 0), true);
		}
		else {
			int nFrom = status.MPI_SOURCE;
			CString thisMsg;
			if (status.MPI_TAG == BROADCAST_MSG)
			{
				thisMsg = _T("[广播] ");
				thisMsg.Append(pMsg);
				pDlg->appendMsg(thisMsg, true, RGB(255, 0, 0), true);
			}
			else {
				if (nFrom == s_rank)
				{
					thisMsg = _T("[我发出的消息] ");
					thisMsg.Append(pMsg);
					pDlg->appendMsg(thisMsg, true, RGB(0, 128, 0), true); // 绿色加粗
				}
				else {
					thisMsg.Format(_T("[来自 %d 的消息] %s"), nFrom, pMsg);
					pDlg->appendMsg(thisMsg, true, RGB(0, 0, 255), false); // 蓝色
				}
			}
		}
		if (status.MPI_TAG == EXIT_MSG)
		{
			// 退出线程
			break;
		}
	}
	pDlg->clearThreadHandle();
	return 0;
}

int CMPITestDlg::init()
{
	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	if (argv == NULL) {
		::MessageBox(NULL, L"解析命令行失败", L"错误", MB_OK);
		return 1;
	}
	char** ppArgs = new char*[argc];
	for (int i = 0; i < argc; i++) {
		int len = WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, NULL, 0, NULL, NULL);
		ppArgs[i] = new char[len];
		WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, ppArgs[i], len, NULL, NULL);
	}

	MPI_Init(&argc, &ppArgs);
	for (int i = 0; i < argc; i++) {
		delete[] ppArgs[i];
	}
	delete[]ppArgs;
	int size;
	MPI_Comm_rank(MCW, &s_rank);
	CString str;
	str.Format(_T("%d"), s_rank);
	m_edtCurrentNode.SetWindowTextW(str);
	MPI_Comm_size(MCW, &size);
	m_lstPersons.ResetContent();
	for (int i = 0; i < size; i++) {
		CString str;
		str.Format(L"节点 %d", i);
		m_lstPersons.AddString(str);
	}
	// 开启线程
	m_handThread = CreateThread(NULL, 0, listenProc, this, 0, NULL);
	if (NULL == m_handThread)
	{
		AfxMessageBox(_T("创建线程失败!"), MB_ICONERROR | IDOK);
		return 1;
	}
	return 0;
}

int CMPITestDlg::uninit()
{
	if (NULL != m_handThread)
	{
		WaitForSingleObject(m_handThread, INFINITE);
		m_handThread = NULL;
	}
	MPI_Finalize();
	MPI_Abort(MCW, 0);
	return 0;
}


void CMPITestDlg::appendMsg(const CString& msg, bool bWrap /*=true*/, COLORREF clr /*=RGB(0,0,0)*/, bool bBold /*=false*/)
{
	// 移动到文本末尾
	int nLen = m_redtMsgs.GetWindowTextLength();
	m_redtMsgs.SetSel(nLen, nLen);

	// 设置格式
	CHARFORMAT2 cf;
	memset(&cf, 0, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR | CFM_BOLD;
	cf.crTextColor = clr;
	cf.dwEffects = bBold ? CFE_BOLD : 0;

	m_redtMsgs.SetSelectionCharFormat(cf);

	// 插入换行
	CString text = msg;
	if (bWrap) text += _T("\r\n");
	m_redtMsgs.ReplaceSel(text);
}

void CMPITestDlg::clearThreadHandle()
{
	m_handThread = NULL;
	this->uninit();
}




void CMPITestDlg::OnClose()
{
	// 中止当前线程
	m_bExiting = TRUE;
	this->uninit();
	MPI_Abort(MCW, 0);
}

int CMPITestDlg::sendNodeMsg(int nDstRank, CString& msg, bool bBroadcast)
{
	MPI_Request	request;
	MPI_Isend((LPCTSTR)msg, msg.GetLength() + 1, MPI_WCHAR, nDstRank, bBroadcast ? BROADCAST_MSG : COMMON_MSG, MCW, &request);
	MPI_Wait(&request, MPI_STATUS_IGNORE);
	return 0;
}

void CMPITestDlg::OnBnClickedButton1()
{
	int nDstRank = m_lstPersons.GetCurSel();
	if (0 > nDstRank)
	{
		WAR_DLG(_T("请先选择接受消息的目标节点!"));
		return;
	}
	if (nDstRank == s_rank)
	{
		WAR_DLG(_T("不能给自己发送消息!"));
		return;
	}
	if (0 == this->m_edtMsg.GetWindowTextLengthW())
	{
		WAR_DLG(_T("消息不能为空!"));
		return;
	}
	CString msg;
	this->m_edtMsg.GetWindowTextW(msg);
	sendNodeMsg(nDstRank, msg);
	appendMsg(_T("[我发出的消息] ") + msg, true, RGB(0, 128, 0), true);
	m_edtMsg.Clear();
}

void CMPITestDlg::OnBnClickedButton2()
{
	if (0 == this->m_edtMsg.GetWindowTextLengthW())
	{
		WAR_DLG(_T("消息不能为空!"));
		return;
	}
	CString msg;
	this->m_edtMsg.GetWindowTextW(msg);
	for (int i = 0; i < m_lstPersons.GetCount(); i++)
	{
		if (i == s_rank) continue;
		sendNodeMsg(i, msg, true);
	}
	appendMsg(_T("[我广播的消息] ") + msg, true, RGB(0, 128, 0), true);
	m_edtMsg.Clear();
}

void CMPITestDlg::OnBnClickedButton3()
{
	for (int i = 0; i < m_lstPersons.GetCount(); i++)
	{
		if (i == s_rank) continue; // 跳过自己
		MPI_Send(L"E", 1, MPI_WCHAR, i, EXIT_MSG, MCW);
	}
	// 中止当前线程
	m_bExiting = TRUE;
	this->uninit();

}

void CMPITestDlg::OnBnClickedButton4()
{
	int size;
	MPI_Comm_rank(MCW, &s_rank);
	MPI_Comm_size(MCW, &size);
	m_lstPersons.ResetContent();
	for (int i = 0; i < size; i++) {
		CString str;
		str.Format(L"节点 %d", i);
		m_lstPersons.AddString(str);
	}
	CString str;
	str.Format(_T("%d"), s_rank);
	m_edtCurrentNode.SetWindowTextW(str);
}
