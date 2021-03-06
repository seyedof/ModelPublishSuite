
// UploadToolDlg.h : header file
//

#pragma once


// CUploadToolDlg dialog
class CUploadToolDlg : public CDialogEx
{
// Construction
public:
	CUploadToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UPLOADTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk ();
	afx_msg void OnBnClickedBtnModelUpload ();
	afx_msg void OnBnClickedBtnAdUpload ();
	virtual LRESULT WindowProc ( UINT message, WPARAM wParam, LPARAM lParam );

	void GetDlgItemAsChar ( UINT id, char* pszValue ) ;

	afx_msg void OnBnClickedBtnModelFile ();
	afx_msg void OnBnClickedBtnCreateSubs ();
	afx_msg void OnBnClickedBtnAdFile ();
};
