
#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "InputDialog.h"

#define IDC_CMD_EDIT 200
#define IDC_X_EDIT 201
#define IDC_Y_EDIT 202
#define IDC_Z_EDIT 203

#define CMD_EDIT_TITLE "Command"
#define X_EDIT_TITLE "X ="
#define Y_EDIT_TITLE "Y ="
#define Z_EDIT_TITLE "Z ="

// ------------------------
//    Class CInputDialog
// ------------------------

IMPLEMENT_DYNAMIC(CInputDialog, CDialog)

CInputDialog::CInputDialog(CString title)
	: CDialog(CInputDialog::IDD, NULL), mTitle(title)
{ }

CInputDialog::~CInputDialog()
{ }

BOOL CInputDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetWindowText(mTitle);

    return TRUE;
}

void CInputDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

// ----------------------
//    Class CCmdDialog
// ----------------------

CCmdDialog::CCmdDialog(CString title)
    : CInputDialog(title), mCmd("")
{ }

CCmdDialog::~CCmdDialog()
{ }

string CCmdDialog::GetCmd()
{
    return ((LPCTSTR)mCmd);
}

void CCmdDialog::DoDataExchange(CDataExchange* pDX)
{
    CInputDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_CMD_EDIT, mCmd);
}

// CCmdDialog message handlers
BEGIN_MESSAGE_MAP(CCmdDialog, CInputDialog)
    ON_WM_CREATE ()
    ON_WM_PAINT()
END_MESSAGE_MAP()

int CCmdDialog::OnCreate(LPCREATESTRUCT lpcs)
{
    mCmdEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
      CRect(10, 30, 450, 100), this, IDC_CMD_EDIT);

    return 0;
}

void CCmdDialog::OnPaint()
{   
    CPaintDC dc(this);
    dc.SetBkMode(TRANSPARENT);

    CRect cmd_rect(10, 10, 450, 30);
	dc.DrawText(ctext, -1, &cmd_rect, DT_SINGLELINE);

    mCmdEdit.SetFocus();
}

// ----------------------
//    Class CXyzDialog
// ----------------------

CXyzDialog::CXyzDialog(CString title)
    : CInputDialog(title), mX(0.0), mY(0.0), mZ(0.0)
{ }

CXyzDialog::~CXyzDialog()
{ }

vec3 CXyzDialog::GetXYZ()
{
    return vec3(mX, mY, mZ);
}

void CXyzDialog::DoDataExchange(CDataExchange* pDX)
{
    CInputDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_X_EDIT, mX);
    DDX_Text(pDX, IDC_Y_EDIT, mY);
    DDX_Text(pDX, IDC_Z_EDIT, mZ);
}

// CXyzDialog message handlers
BEGIN_MESSAGE_MAP(CXyzDialog, CInputDialog)
    ON_WM_CREATE ()
    ON_WM_PAINT()
END_MESSAGE_MAP()

int CXyzDialog::OnCreate(LPCREATESTRUCT lpcs)
{
    mXEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
      CRect(130, 70, 340, 90), this, IDC_X_EDIT);

    mYEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
      CRect(130, 140, 340, 160), this, IDC_Y_EDIT);

    mZEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
      CRect(130, 210, 340, 230), this, IDC_Z_EDIT);

    return 0;
}

void CXyzDialog::OnPaint()
{   
    CPaintDC dc(this);
    dc.SetBkMode(TRANSPARENT);

    CRect x_rect(100, 72, 450, 90);
    dc.DrawText(CString(X_EDIT_TITLE), -1, &x_rect, DT_SINGLELINE);

    CRect y_rect(100, 142, 450, 160);
    dc.DrawText(CString(Y_EDIT_TITLE), -1, &y_rect, DT_SINGLELINE);

    CRect z_rect(100, 212, 450, 230);
    dc.DrawText(CString(Z_EDIT_TITLE), -1, &z_rect, DT_SINGLELINE);

    mXEdit.SetFocus();
}

// -------------------------
//    Class CCmdXyzDialog
// -------------------------

CCmdXyzDialog::CCmdXyzDialog(CString title)
    :  CInputDialog(title), mCmd(""), mX(0.0), mY(0.0), mZ(0.0)
{ }

CCmdXyzDialog::~CCmdXyzDialog()
{ }

string CCmdXyzDialog::GetCmd()
{
    return ((LPCTSTR)mCmd);
}

vec3 CCmdXyzDialog::GetXYZ()
{
    return vec3(mX, mY, mZ);
}

void CCmdXyzDialog::DoDataExchange(CDataExchange* pDX)
{
    CInputDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_CMD_EDIT, mCmd);
    DDX_Text(pDX, IDC_X_EDIT, mX);
    DDX_Text(pDX, IDC_Y_EDIT, mY);
    DDX_Text(pDX, IDC_Z_EDIT, mZ);
}

// CCmdXyzDialog message handlers
BEGIN_MESSAGE_MAP(CCmdXyzDialog, CInputDialog)
    ON_WM_CREATE ()
    ON_WM_PAINT()
END_MESSAGE_MAP()

int CCmdXyzDialog::OnCreate(LPCREATESTRUCT lpcs)
{    
    mCmdEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
      CRect(10, 30, 450, 100), this, IDC_CMD_EDIT);

    mXEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
      CRect(40, 135, 250, 155), this, IDC_X_EDIT);

    mYEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
      CRect(40, 190, 250, 210), this, IDC_Y_EDIT);

    mZEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
      CRect(40, 245, 250, 265), this, IDC_Z_EDIT);

    return 0;
}

void CCmdXyzDialog::OnPaint()
{   
    CPaintDC dc(this);
    dc.SetBkMode(TRANSPARENT);

    CRect cmd_rect(10, 10, 450, 30);
    dc.DrawText(CString(CMD_EDIT_TITLE), -1, &cmd_rect, DT_SINGLELINE);

    CRect x_rect(10, 137, 450, 155);
    dc.DrawText(CString(X_EDIT_TITLE), -1, &x_rect, DT_SINGLELINE);

    CRect y_rect(10, 192, 450, 210);
    dc.DrawText(CString(Y_EDIT_TITLE), -1, &y_rect, DT_SINGLELINE);

    CRect z_rect(10, 247, 450, 265);
    dc.DrawText(CString(Z_EDIT_TITLE), -1, &z_rect, DT_SINGLELINE);

    mCmdEdit.SetFocus();
}


CMaterialDialog::CMaterialDialog(CString title) : CInputDialog(title) {}

CMaterialDialog::~CMaterialDialog() {}

Material CMaterialDialog::GetMaterial()
{
	return mMaterial;
}

void CMaterialDialog::SetMaterial(const Material& material)
{
	mMaterial = material;
}

void CMaterialDialog::DoDataExchange(CDataExchange* pDX)
{
	CInputDialog::DoDataExchange(pDX);
	DDX_Text(pDX, 1, mMaterial.ambient.x);
	DDX_Text(pDX, 2, mMaterial.ambient.y);
	DDX_Text(pDX, 3, mMaterial.ambient.z);
	DDX_Text(pDX, 4, mMaterial.diffuse.x);
	DDX_Text(pDX, 5, mMaterial.diffuse.y);
	DDX_Text(pDX, 6, mMaterial.diffuse.z);
	DDX_Text(pDX, 7, mMaterial.specular.x);
	DDX_Text(pDX, 8, mMaterial.specular.y);
	DDX_Text(pDX, 9, mMaterial.specular.z);
	DDX_Text(pDX, 10, mMaterial.emission.x);
	DDX_Text(pDX, 11, mMaterial.emission.y);
	DDX_Text(pDX, 12, mMaterial.emission.z);
	DDX_Text(pDX, 13, mMaterial.shininess);
}

// CCmdDialog message handlers
BEGIN_MESSAGE_MAP(CMaterialDialog, CInputDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

int CMaterialDialog::OnCreate(LPCREATESTRUCT lpcs)
{
	int ambientTop = 18;
	mAmbientR.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(115, ambientTop, 185, ambientTop + 20), this, 1);
	mAmbientG.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(220, ambientTop, 290, ambientTop + 20), this, 2);
	mAmbientB.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(325, ambientTop, 395, ambientTop + 20), this, 3);

	int diffuseTop = 58;
	mDiffuseR.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(115, diffuseTop, 185, diffuseTop + 20), this, 4);
	mDiffuseG.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(220, diffuseTop, 290, diffuseTop + 20), this, 5);
	mDiffuseB.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(325, diffuseTop, 395, diffuseTop + 20), this, 6);

	int specularTop = 98;
	mSpecularR.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(115, specularTop, 185, specularTop + 20), this, 7);
	mSpecularG.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(220, specularTop, 290, specularTop + 20), this, 8);
	mSpecularB.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(325, specularTop, 395, specularTop + 20), this, 9);

	int emissionTop = 138;
	mEmissionR.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(115, emissionTop, 185, emissionTop + 20), this, 10);
	mEmissionG.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(220, emissionTop, 290, emissionTop + 20), this, 11);
	mEmissionB.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(325, emissionTop, 395, emissionTop + 20), this, 12);

	int shininessTop = 178;
	mShininess.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(90, shininessTop, 140, shininessTop + 20), this, 13);

	return 0;
}

void CMaterialDialog::OnPaint()
{
	CPaintDC dc(this);
	dc.SetBkMode(TRANSPARENT);

	int ambientTop = 20;
	CRect ambient_rect(10, ambientTop, 80, ambientTop + 18);
	dc.DrawText(CString("Ambient:"), -1, &ambient_rect, DT_SINGLELINE);
	CRect ambient_r_rect(90, ambientTop, 110, ambientTop + 18);
	dc.DrawText(CString("R ="), -1, &ambient_r_rect, DT_SINGLELINE);
	CRect ambient_g_rect(195, ambientTop, 215, ambientTop + 18);
	dc.DrawText(CString("G ="), -1, &ambient_g_rect, DT_SINGLELINE);
	CRect ambient_b_rect(300, ambientTop, 320, ambientTop + 18);
	dc.DrawText(CString("B ="), -1, &ambient_b_rect, DT_SINGLELINE);

	int diffuseTop = 60;
	CRect diffuse_rect(10, diffuseTop, 80, diffuseTop + 18);
	dc.DrawText(CString("Diffuse:"), -1, &diffuse_rect, DT_SINGLELINE);
	CRect diffuse_r_rect(90, diffuseTop, 110, diffuseTop + 18);
	dc.DrawText(CString("R ="), -1, &diffuse_r_rect, DT_SINGLELINE);
	CRect diffuse_g_rect(195, diffuseTop, 215, diffuseTop + 18);
	dc.DrawText(CString("G ="), -1, &diffuse_g_rect, DT_SINGLELINE);
	CRect diffuse_b_rect(300, diffuseTop, 320, diffuseTop + 18);
	dc.DrawText(CString("B ="), -1, &diffuse_b_rect, DT_SINGLELINE);

	int specularTop = 100;
	CRect specular_rect(10, specularTop, 80, specularTop + 18);
	dc.DrawText(CString("Specular:"), -1, &specular_rect, DT_SINGLELINE);
	CRect specular_r_rect(90, specularTop, 110, specularTop + 18);
	dc.DrawText(CString("R ="), -1, &specular_r_rect, DT_SINGLELINE);
	CRect specular_g_rect(195, specularTop, 215, specularTop + 18);
	dc.DrawText(CString("G ="), -1, &specular_g_rect, DT_SINGLELINE);
	CRect specular_b_rect(300, specularTop, 320, specularTop + 18);
	dc.DrawText(CString("B ="), -1, &specular_b_rect, DT_SINGLELINE);

	int emissionTop = 140;
	CRect emission_rect(10, emissionTop, 80, emissionTop + 18);
	dc.DrawText(CString("Emission:"), -1, &emission_rect, DT_SINGLELINE);
	CRect emission_r_rect(90, emissionTop, 110, emissionTop + 18);
	dc.DrawText(CString("R ="), -1, &emission_r_rect, DT_SINGLELINE);
	CRect emission_g_rect(195, emissionTop, 215, emissionTop + 18);
	dc.DrawText(CString("G ="), -1, &emission_g_rect, DT_SINGLELINE);
	CRect emission_b_rect(300, emissionTop, 320, emissionTop + 18);
	dc.DrawText(CString("B ="), -1, &emission_b_rect, DT_SINGLELINE);

	int shininessTop = 180;
	CRect shininess_rect(10, shininessTop, 80, shininessTop + 18);
	dc.DrawText(CString("Shininess:"), -1, &shininess_rect, DT_SINGLELINE);

	mAmbientR.SetFocus();
}