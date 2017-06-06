
// MFCDatabaseAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDatabaseApp.h"
#include "MFCDatabaseAppDlg.h"
#include "afxdialogex.h"
#include <afxdb.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define NUM_ATTRIB 19
#define SERVER_NAME "192.168.10.3"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SOCKET ConnectSocket = INVALID_SOCKET;
    
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCDatabaseAppDlg dialog

CMFCDatabaseAppDlg::CMFCDatabaseAppDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCDatabaseAppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CMFCDatabaseAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_List);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}

BEGIN_MESSAGE_MAP(CMFCDatabaseAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SUBMIT, &CMFCDatabaseAppDlg::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &CMFCDatabaseAppDlg::OnNMClickList2)
END_MESSAGE_MAP()

/* Multiple overriden Log functions for debugging purposes
void Log(std::string s1,std::string str)
{
	ofstream logFile;
	logFile.open("D:\\logfile.txt",ios::app);
	logFile<<s1<<" : "<<str<<"\n";
	logFile.close();
}


void Log(std::string str,int ecode)
{
	ofstream logFile;
	logFile.open("D:\\logfile.txt",ios::app);
	logFile<<str<<" : "<<ecode<<"\n";
	logFile.close();

}


void Log(std::string str)
{
	ofstream logFile;
	logFile.open("D:\\logfile.txt",ios::app);
	logFile<<str<<"\n";
	logFile.close();
}
*/

//Function to convert CString to std::string, socket functions further use 'const char*', use std::string::c_str()
std::string CStrToStr(CString s)
{
	CT2CA SName(s);
	std::string sName(SName);
	return sName;
}

// CMFCDatabaseAppDlg message handlers

BOOL CMFCDatabaseAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//Adds the hint message in the edit box
	CWnd *pwd = GetDlgItem(IDC_KEY );
	HWND hwnd = pwd->GetSafeHwnd();
	::SendMessage(hwnd,EM_SETCUEBANNER,FALSE,(LPARAM)L"Enter Key Here");
	
	//Settings columns and behaviour
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0,L"CDS Number : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(1,L"Reg. ID : ",LVCFMT_LEFT,-1,0);
	m_List.InsertColumn(2,L"Product Name : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(3,L"Reg. Key : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(4,L"Telephone : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(5,L"E-Mail : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(6,L"Reg. Date : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(7,L"Licenses : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(8,L"Final Key : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(9,L"Comp. Name : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(10,L"CP : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(11,L"Add. Ln1 : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(12,L"Add. Ln2 : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(13,L"City : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(14,L"State : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(15,L"Country : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(16,L"Pin : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(17,L"Fax : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(18,L"Flag1 : ",LVCFMT_LEFT,-1,1);
	m_List.InsertColumn(19,L"Flag2 : ",LVCFMT_LEFT,-1,1);
	m_List.SetColumnWidth(19,38);
	
	//SOCKET Connection Code
	WSADATA wsaData;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    	
	int iResult;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) 
	{
        //Log("WSAStartup failed with error",iResult);
		string msg="WSAStartup failed with error : "+to_string(iResult);
		MessageBoxA (NULL,msg.c_str(),"ERROR",MB_ICONERROR);
		return FALSE;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
	iResult = getaddrinfo(SERVER_NAME, DEFAULT_PORT, &hints, &result);
	if ( iResult != 0 ) 
	{
        //Log("getaddrinfo failed with error",iResult);
		string msg="getaddrinfo failed with error : "+to_string(iResult);
		MessageBoxA (NULL,msg.c_str(),"ERROR",MB_ICONERROR);
        WSACleanup();
        return FALSE;
    }
    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) 
	{
		//Log("In for loop");
        // Create a SOCKET for connecting to server
		ptr=result;
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) 
		{
			//Log("socket failed with error",WSAGetLastError());
            string msg="socket failed with error : "+to_string(WSAGetLastError());
			MessageBoxA(NULL,msg.c_str(),"ERROR",MB_ICONERROR);
			WSACleanup();
            return FALSE;
        }
		//Log("Before attempting to connect");
        // Connect to server.
		try
		{
			iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) 
			{
				//Log("Connection Failed with error",iResult);
				string msg="Connection Failed with error : "+to_string(WSAGetLastError());
				MessageBoxA (NULL,msg.c_str(),"ERROR",MB_ICONERROR);
				closesocket(ConnectSocket);
				ConnectSocket = INVALID_SOCKET;
				return FALSE;
			}
		}

		catch(...)
		{
			//Log("Exception");
		}
        
    //Log("Inside end of for loop");
	}

	//Log("Outside for loop");
    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) 
	{
		string msg="Unable to connect to server : "+to_string(WSAGetLastError());
		MessageBoxA (NULL,msg.c_str(),"ERROR",MB_ICONERROR);
				
		//Log("Unable to connect to server!",WSAGetLastError());
		WSACleanup();
        return FALSE;
    }
		
	//Log("Socket Connected");
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCDatabaseAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCDatabaseAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}	
}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCDatabaseAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCDatabaseAppDlg::OnBnClickedOk()
{
	//Log("\nIn OnBnClick");
	CString Key;
	GetDlgItemText(IDC_KEY, Key);

	//Log("Key",CStrToStr(Key));
	
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

	//Auto corrects case to upper
	Key.MakeUpper();

	//Key Validation, Requires a 10 digit key with 1st 3 alphabets, last 7 numeric
	if ( Key.GetLength()!=10 ||
		(Key.Left(3).SpanIncluding(L"QWERTYUIOPASDFGHJKLZXCVBNM")!=Key.Left(3))||
		(Key.Right(7).SpanIncluding(L"0123456789")!=Key.Right(7)) )
	{
		//Log("Invalid Key");
		MessageBoxA(NULL,"Check your Key Again","Invalid Key",MB_ICONWARNING);
		return;
	}

	//Log("Before key send");
	iResult = send( ConnectSocket,CStrToStr(Key).c_str(),Key.GetLength()+1, 0 );
	if (iResult == SOCKET_ERROR) 
	{
		//Log("Key send failed with error: ", WSAGetLastError());
		string msg="send failed with error : "+to_string(WSAGetLastError());
		MessageBoxA(NULL,msg.c_str(),"ERROR",MB_ICONERROR);
		closesocket(ConnectSocket);
		WSACleanup();
		return;
	}

	//Log("Key sent");
	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0)
	{
		if (strcmp(recvbuf, "NO RECORDS FOUND") == 0)
		{
			MessageBoxA(NULL, recvbuf, "INVALID KEY", MB_ICONERROR);
			//Log("ERROR OCCURED - NO RECORDS");
			return;
		}
	}
	else
	{
		//Log("RECV ERROR");
	}
	
	bool flag = true;
	
	while (flag)
	{
		char readySTB[]="READY?";
		m_List.InsertItem(0,Key,0);
		//Log("Ready to recieve data");
		//Recieve all entries;
		for (int i=1;i<=NUM_ATTRIB;i++)
		{
			iResult = send( ConnectSocket, readySTB, sizeof(readySTB) , 0 );
			if (iResult == SOCKET_ERROR) 
			{
				//Log("ready send failed with error: ", WSAGetLastError());
				closesocket(ConnectSocket);
				WSACleanup();
				return;
			}
	
	
			iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
			if (iResult>0)
			{
				CString temp(recvbuf);
				m_List.SetItemText(0,i,temp);
			}
		}
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult>0)
		{
			CString temp(recvbuf);
			if (temp.Compare(L"FINISHED")==0)
				flag=false;
		}
	
	}
	
	for (int i = 0;i < m_List.GetHeaderCtrl()->GetItemCount();i++)
		m_List.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
}


void CMFCDatabaseAppDlg::OnCancel()
{
	//TODO : Send message to server to exit thread
	char exitMSG[]="SHUTDOWN";
	int iResult;
	iResult = send( ConnectSocket, exitMSG, sizeof(exitMSG) , 0 );
	if (iResult == SOCKET_ERROR) 
	{
		//Log("send failed with error: ", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
	}
	//Log("Shutdown msg sent");
	
	//Shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) 
	{
	    //Log("shutdown failed with error: ", WSAGetLastError());
        closesocket(ConnectSocket);
    }
	//Log("SHUTDOWN COMPLETE");
    // cleanup
    closesocket(ConnectSocket);
    CDialogEx::OnCancel();
}




void CMFCDatabaseAppDlg::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
		*pResult = 0;
	// TODO: Add your control notification handler code here
	*pResult = 0;


	//Log("SELECT FUNCTION");
	int row = m_List.GetSelectionMark();
	if (row<0)
		return;

	m_ListBox.ResetContent();
	m_ListBox.AddString(L"CDS Number : "+m_List.GetItemText(row,0));
	m_ListBox.AddString(L"Reg. ID : "+m_List.GetItemText(row,1));
	m_ListBox.AddString(L"Product Name : "+m_List.GetItemText(row,2));
	m_ListBox.AddString(L"Reg. Key : "+m_List.GetItemText(row,3));
	m_ListBox.AddString(L"Telephone : "+m_List.GetItemText(row,4));
	m_ListBox.AddString(L"E-Mail : "+m_List.GetItemText(row,5));
	m_ListBox.AddString(L"Reg. Date : "+m_List.GetItemText(row,6));
	m_ListBox.AddString(L"Licenses : "+m_List.GetItemText(row,7));
	m_ListBox.AddString(L"Final Key : "+m_List.GetItemText(row,8));
	m_ListBox.AddString(L"Comp. Name : "+m_List.GetItemText(row,9));
	m_ListBox.AddString(L"CP : "+m_List.GetItemText(row,10));
	m_ListBox.AddString(L"Add. Ln1 : "+m_List.GetItemText(row,11));
	m_ListBox.AddString(L"Add. Ln2 : "+m_List.GetItemText(row,12));
	m_ListBox.AddString(L"City : "+m_List.GetItemText(row,13));
	m_ListBox.AddString(L"State : "+m_List.GetItemText(row,14));
	m_ListBox.AddString(L"Country : "+m_List.GetItemText(row,15));
	m_ListBox.AddString(L"Pin : "+m_List.GetItemText(row,16));
	m_ListBox.AddString(L"Fax : "+m_List.GetItemText(row,17));
	m_ListBox.AddString(L"Flag1 : "+m_List.GetItemText(row,18));
	m_ListBox.AddString(L"Flag2 : "+m_List.GetItemText(row,19));
}

