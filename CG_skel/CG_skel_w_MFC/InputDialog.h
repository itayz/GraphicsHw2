
#pragma once

#include <string>
using std::string;

#include "vec.h"
#include "Material.h"

// ------------------------
//    Class CInputDialog
// ------------------------

class CInputDialog : public CDialog
{
    DECLARE_DYNAMIC(CInputDialog)

public:
    CInputDialog(CString title = "Input Dialog");
    virtual ~CInputDialog();

    virtual BOOL OnInitDialog();

    enum { IDD = IDD_INPUTDIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
	
private:
    CString mTitle;
};

// ----------------------
//    Class CCmdDialog
// ----------------------

class CCmdDialog : public CInputDialog
{
public:
	
	CCmdDialog(CString title = "Input Dialog");
    virtual ~CCmdDialog();
	CString ctext;
    string GetCmd();

protected:
    CString mCmd;
    CEdit mCmdEdit;
	
    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg int OnCreate (LPCREATESTRUCT lpcs);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

// ----------------------
//    Class CXyzDialog
// ----------------------

class CXyzDialog : public CInputDialog
{
public:
    CXyzDialog(CString title = "Input Dialog");
    virtual ~CXyzDialog();

    vec3 GetXYZ();

protected:
    float mX;
    float mY;
    float mZ;
    CEdit mXEdit;
    CEdit mYEdit;
    CEdit mZEdit;

    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg int OnCreate (LPCREATESTRUCT lpcs);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

// -------------------------
//    Class CCmdXyzDialog
// -------------------------

class CCmdXyzDialog : public CInputDialog
{
public:
    CCmdXyzDialog(CString title = "Input Dialog");
    virtual ~CCmdXyzDialog();

    string GetCmd();
    vec3 GetXYZ();

protected:
    CString mCmd;
    float mX;
    float mY;
    float mZ;
    CEdit mCmdEdit;
    CEdit mXEdit;
    CEdit mYEdit;
    CEdit mZEdit;

    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg int OnCreate (LPCREATESTRUCT lpcs);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};


class CMaterialDialog : public CInputDialog
{
public:
	CMaterialDialog(CString title = "Material Input Dialog");
	virtual ~CMaterialDialog();

	Material GetMaterial();
	void SetMaterial(const Material& material);

protected:
	CEdit mAmbientR;
	CEdit mAmbientG;
	CEdit mAmbientB;
	CEdit mDiffuseR;
	CEdit mDiffuseG;
	CEdit mDiffuseB;
	CEdit mSpecularR;
	CEdit mSpecularG;
	CEdit mSpecularB;
	CEdit mEmissionR;
	CEdit mEmissionG;
	CEdit mEmissionB;
	CEdit mShininess;
	Material mMaterial;

	virtual void DoDataExchange(CDataExchange* pDX);

	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};