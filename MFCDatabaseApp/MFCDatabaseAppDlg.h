
// MFCDatabaseAppDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMFCDatabaseAppDlg dialog
class CMFCDatabaseAppDlg : public CDialogEx
{
// Construction
public:
	CMFCDatabaseAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFCDATABASEAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	 

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	CListCtrl m_List;
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	CListBox m_ListBox;
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchangeListbox();
};
