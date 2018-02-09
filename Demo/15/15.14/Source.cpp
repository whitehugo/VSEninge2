//演示一个Shirley光照模型
#include <VSApplication.h>
#include <VSEngineInput.h>
#include <VSCameraActor.h>
#include <VSWorld.h>
#include <VS1stCameraController.h>
#include "VSStaticActor.h"
#include "VSLightActor.h"
#include "VSSkyLight.h"
#include "VSViewFamily.h"
#include "VSTimer.h"
#include "VSSceneManager.h"
#include "VSSkelectonActor.h"
#include "VSPostEffectFunction.h"
using namespace VSEngine2;
class VSDemoWindowsApplication : public VSWindowApplication
{

	DLCARE_APPLICATION(VSDemoWindowsApplication);
public:
	VSDemoWindowsApplication();
	~VSDemoWindowsApplication();
	virtual bool PreInitial();
	virtual bool OnInitial();
	virtual bool OnTerminal();
	virtual bool PostUpdate();
	virtual bool PreUpdate();
	virtual bool OnDraw();
	virtual void OnMove(int xPos, int yPos);
	virtual void OnReSize(int iWidth, int iHeight);
	virtual void OnKeyDown(unsigned int uiKey);
	virtual void OnKeyUp(unsigned int uiKey);
	virtual void OnLButtonDown(int xPos, int yPos);
	virtual void OnLButtonUp(int xPos, int yPos);
	virtual void OnRButtonDown(int xPos, int yPos);
	virtual void OnRButtonUp(int xPos, int yPos);
	virtual void OnMButtonDown(int xPos, int yPos);
	virtual void OnMButtonUp(int xPos, int yPos);
	virtual void OnMouseMove(int xPos, int yPos);
	virtual void OnMouseWheel(int xPos, int yPos, int zDet);
protected:
	VSCameraActor* m_pCameraActor;
	VS1stCameraController* m_p1stCameraController;
	VSSceneMap * m_pTestMap;
	bool m_bLMouseDowning;
	int		m_iDetMouseX;
	int		m_iDetMouseY;
	int		m_iLastMouseX;
	int		m_iLastMouseY;
	VSMaterialInstance * pMaterialInstance0;
	VSMaterialInstance * pMaterialInstance1;
	float NU0;
	float NU1;

	float NV0;
	float NV1;

	float Rs0;
	float Rs1;

};
IMPLEMENT_APPLICATION(VSDemoWindowsApplication);
VSDemoWindowsApplication::VSDemoWindowsApplication()
{

	NU0 = 70.0f;
	NU1 = 70.0f;

	NV0 = 44.0f;
	NV1 = 44.0f;

	Rs0 = 1.0f;
	Rs1 = 1.0f;
}
VSDemoWindowsApplication::~VSDemoWindowsApplication()
{

}
bool VSDemoWindowsApplication::PreInitial()
{
	VSWindowApplication::PreInitial();
	m_uiInputAPIType = VSEngineInput::IAT_WINDOWS;
	//m_uiInputAPIType = VSEngineInput::IAT_DX;
	VSResourceManager::ms_bUpdateThread = false;
	VSResourceManager::ms_bRenderThread = false;
	m_bLMouseDowning = false;

	m_iDetMouseX = 0;
	m_iDetMouseY = 0;
	m_iLastMouseX = 0;
	m_iLastMouseY = 0;

	VSEngineFlag::EnableCLODMesh = false;

	return true;
}
bool VSDemoWindowsApplication::OnInitial()
{
	VSWindowApplication::OnInitial();

	m_p1stCameraController = VSObject::GetInstance<VS1stCameraController>();
	m_pCameraActor = (VSCameraActor *)VSWorld::ms_pWorld->CreateActor<VSCameraActor>();
	m_pCameraActor->GetTypeNode()->AddController(m_p1stCameraController);
	VSVector3 CameraPos(0.0f, 100.0f, -300.0f);
	VSVector3 CameraDir(0.0f, 0.0f, 1.0f);
	m_pCameraActor->GetTypeNode()->CreateFromLookDir(CameraPos, CameraDir);
	m_pCameraActor->GetTypeNode()->SetPerspectiveFov(AngleToRadian(90.0f), (m_uiScreenWidth * 1.0f) / (m_uiScreenHeight), 1.0f, 8000.0f);

	m_pTestMap = VSWorld::ms_pWorld->CreateScene(_T("Test"));

	VSSkelectonActor * pSKActor1 = (VSSkelectonActor *)VSWorld::ms_pWorld->CreateActor(_T("NewMonsterShirley.SKMODEL"), VSVector3(0, 0, 0), VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pMaterialInstance0 = pSKActor1->GetTypeNode()->GetUseMaterialInstance(0);
	pMaterialInstance1 = pSKActor1->GetTypeNode()->GetUseMaterialInstance(1);

	VSStaticActor* pStaticActor = (VSStaticActor*)VSWorld::ms_pWorld->CreateActor(_T("NewOceanPlane.STMODEL"), VSVector3(0, 0, 0), VSMatrix3X3::ms_Identity, VSVector3(100.0f, 100.f, 100.0f), m_pTestMap);
	pStaticActor->GetTypeNode()->CastShadow(false);

	VSSkyLightActor * pSkyLightActor = (VSSkyLightActor *)VSWorld::ms_pWorld->CreateActor<VSSkyLightActor>(VSVector3::ms_Zero, VSMatrix3X3::ms_Identity, VSVector3::ms_One, m_pTestMap);
	pSkyLightActor->GetTypeNode()->m_DownColor = VSColorRGBA(0.0f, 0.0f, 0.5f, 1.0f);
	pSkyLightActor->GetTypeNode()->m_UpColor = VSColorRGBA(0.2f, 0.2f, 0.2f, 1.0f);

	VSMatrix3X3 Dir;
	Dir.CreateEluer(0, AngleToRadian(45), AngleToRadian(45));
	VSDirectionLightActor * pDirectionLightActor = (VSDirectionLightActor *)VSWorld::ms_pWorld->CreateActor<VSDirectionLightActor>(VSVector3::ms_Zero, Dir, VSVector3::ms_One, m_pTestMap);

	m_pTestMap->GetScene()->Build();

	VSArray<VSString> SceneMap;
	SceneMap.AddElement(_T("Main"));
	SceneMap.AddElement(_T("Test"));

	VSWorld::ms_pWorld->AttachWindowViewFamilyToCamera(m_pCameraActor, VSWindowViewFamily::VT_WINDOW_NORMAL,
		_T("WindowUse"), SceneMap, VSForwordEffectSceneRenderMethod::ms_Type.GetName().GetBuffer(), -1);


	return true;
}
bool VSDemoWindowsApplication::OnTerminal()
{
	VSWindowApplication::OnTerminal();

	return true;
}
bool VSDemoWindowsApplication::PostUpdate()
{
	VSWindowApplication::PostUpdate();
	if (m_bLMouseDowning)
	{
		m_p1stCameraController->m_RotXDelta = ((m_iDetMouseY)* 1.0f) * 0.001f/*VSTimer::ms_pTimer->GetDetTime() * 0.5f*/;
		m_p1stCameraController->m_RotYDelta = ((m_iDetMouseX)* 1.0f) * 0.001f/*VSTimer::ms_pTimer->GetDetTime() * 0.5f*/;
		m_iDetMouseX = 0;
		m_iDetMouseY = 0;
	}
	else
	{
		m_p1stCameraController->m_RotXDelta = 0.0f;
		m_p1stCameraController->m_RotYDelta = 0.0f;
	}

	pMaterialInstance0->SetPShaderValue(_T("NU"), &NU0, sizeof(NU0));
	pMaterialInstance1->SetPShaderValue(_T("NU"), &NU1, sizeof(NU1));

	pMaterialInstance0->SetPShaderValue(_T("NV"), &NV0, sizeof(NV0));
	pMaterialInstance1->SetPShaderValue(_T("NV"), &NV1, sizeof(NV1));

	pMaterialInstance0->SetPShaderValue(_T("Rs"), &Rs0, sizeof(Rs0));
	pMaterialInstance1->SetPShaderValue(_T("Rs"), &Rs1, sizeof(Rs1));

	return true;
}
bool VSDemoWindowsApplication::PreUpdate()
{
	VSWindowApplication::PreUpdate();
	return true;
}
bool VSDemoWindowsApplication::OnDraw()
{
	VSWindowApplication::OnDraw();
	VSRenderer::ms_pRenderer->DrawText(NULL, 700, 400, VSColorRGBA(1.0f, 1.0f, 1.0f), _T("Use Key '+' And '-' to Change NU0"));
	VSRenderer::ms_pRenderer->DrawText(NULL, 700, 420, VSColorRGBA(1.0f, 1.0f, 1.0f), _T("NU0:%f"), NU0);
	VSRenderer::ms_pRenderer->DrawText(NULL, 700, 440, VSColorRGBA(1.0f, 1.0f, 1.0f), _T("Use Key '[' And ']' to Change NU1"));
	VSRenderer::ms_pRenderer->DrawText(NULL, 700, 460, VSColorRGBA(1.0f, 1.0f, 1.0f), _T("NU1:%f"), NU1);

	VSRenderer::ms_pRenderer->DrawText(NULL, 700, 480, VSColorRGBA(1.0f, 1.0f, 1.0f), _T("Use Key '1' And '2' to Change NV0"));
	VSRenderer::ms_pRenderer->DrawText(NULL, 700, 500, VSColorRGBA(1.0f, 1.0f, 1.0f), _T("NV0:%f"), NV0);
	VSRenderer::ms_pRenderer->DrawText(NULL, 700, 520, VSColorRGBA(1.0f, 1.0f, 1.0f), _T("Use Key 'F1' And 'F2' to Change NV1"));
	VSRenderer::ms_pRenderer->DrawText(NULL, 700, 540, VSColorRGBA(1.0f, 1.0f, 1.0f), _T("NV1:%f"), NV1);

	VSRenderer::ms_pRenderer->DrawText(NULL, 700, 560, VSColorRGBA(1.0f, 1.0f, 1.0f), _T("Use Key '3' And '4' to Change Rs0"));
	VSRenderer::ms_pRenderer->DrawText(NULL, 700, 580, VSColorRGBA(1.0f, 1.0f, 1.0f), _T("Rs0:%f"), Rs0);
	VSRenderer::ms_pRenderer->DrawText(NULL, 700, 600, VSColorRGBA(1.0f, 1.0f, 1.0f), _T("Use Key 'F3' And 'F4' to Change Rs1"));
	VSRenderer::ms_pRenderer->DrawText(NULL, 700, 620, VSColorRGBA(1.0f, 1.0f, 1.0f), _T("Rs1:%f"), Rs1);

	return true;
}
void VSDemoWindowsApplication::OnMove(int xPos, int yPos)
{

}
void VSDemoWindowsApplication::OnReSize(int iWidth, int iHeight)
{

}
void VSDemoWindowsApplication::OnKeyDown(unsigned int uiKey)
{
	VSApplication::OnKeyDown(uiKey);
	if (uiKey == VSEngineInput::BK_UP)
	{
		m_p1stCameraController->m_MoveZDelta = 1.0f;
	}
	else if (uiKey == VSEngineInput::BK_DOWN)
	{
		m_p1stCameraController->m_MoveZDelta = -1.0f;
	}
	else if (uiKey == VSEngineInput::BK_LEFT)
	{
		m_p1stCameraController->m_MoveXDelta = -1.0f;
	}
	else if (uiKey == VSEngineInput::BK_RIGHT)
	{
		m_p1stCameraController->m_MoveXDelta = 1.0f;
	}
	else if (uiKey == VSEngineInput::BK_MINUS)
	{
		NU0 -= 0.1f;
	}
	else if (uiKey == VSEngineInput::BK_EQUALS)
	{
		NU0 += 0.1f;
	}
	else if (uiKey == VSEngineInput::BK_LBRACKET)
	{
		NU1 -= 0.1f;
	}
	else if (uiKey == VSEngineInput::BK_RBRACKET)
	{
		NU1 += 0.1f;
	}
	else if (uiKey == VSEngineInput::BK_1)
	{
		NV0 -= 0.1f;
	}
	else if (uiKey == VSEngineInput::BK_2)
	{
		NV0 += 0.1f;
	}
	else if (uiKey == VSEngineInput::BK_F1)
	{
		NV1 -= 0.1f;
	}
	else if (uiKey == VSEngineInput::BK_F2)
	{
		NV1 += 0.1f;
	}
	else if (uiKey == VSEngineInput::BK_3)
	{
		Rs0 -= 0.001f;
	}
	else if (uiKey == VSEngineInput::BK_4)
	{
		Rs0 += 0.001f;
	}
	else if (uiKey == VSEngineInput::BK_F3)
	{
		Rs1 -= 0.001f;
	}
	else if (uiKey == VSEngineInput::BK_F4)
	{
		Rs1 += 0.001f;
	}

}
void VSDemoWindowsApplication::OnKeyUp(unsigned int uiKey)
{
	VSApplication::OnKeyUp(uiKey);
	if (uiKey == VSEngineInput::BK_UP)
	{
		m_p1stCameraController->m_MoveZDelta = 0.0f;
	}
	else if (uiKey == VSEngineInput::BK_DOWN)
	{
		m_p1stCameraController->m_MoveZDelta = 0.0f;
	}
	else if (uiKey == VSEngineInput::BK_LEFT)
	{
		m_p1stCameraController->m_MoveXDelta = 0.0f;
	}
	else if (uiKey == VSEngineInput::BK_RIGHT)
	{
		m_p1stCameraController->m_MoveXDelta = 0.0f;
	}
}
void VSDemoWindowsApplication::OnLButtonDown(int xPos, int yPos)
{
	VSWindowApplication::OnLButtonDown(xPos, yPos);
	m_bLMouseDowning = true;
	m_iLastMouseX = xPos;
	m_iLastMouseY = yPos;
	m_iDetMouseX = 0;
	m_iDetMouseY = 0;
}
void VSDemoWindowsApplication::OnLButtonUp(int xPos, int yPos)
{
	VSWindowApplication::OnLButtonUp(xPos, yPos);
	m_bLMouseDowning = false;
}
void VSDemoWindowsApplication::OnRButtonDown(int xPos, int yPos)
{
	VSWindowApplication::OnRButtonDown(xPos, yPos);
}
void VSDemoWindowsApplication::OnRButtonUp(int xPos, int yPos)
{
	VSWindowApplication::OnRButtonUp(xPos, yPos);
}
void VSDemoWindowsApplication::OnMButtonDown(int xPos, int yPos)
{
	VSWindowApplication::OnMButtonDown(xPos, yPos);
}
void VSDemoWindowsApplication::OnMButtonUp(int xPos, int yPos)
{
	VSWindowApplication::OnMButtonUp(xPos, yPos);
}
void VSDemoWindowsApplication::OnMouseMove(int xPos, int yPos)
{
	VSWindowApplication::OnMouseMove(xPos, yPos);
	if (m_bLMouseDowning)
	{
		m_iDetMouseX = xPos - m_iLastMouseX;
		m_iDetMouseY = yPos - m_iLastMouseY;
		m_iLastMouseX = xPos;
		m_iLastMouseY = yPos;
	}

}
void VSDemoWindowsApplication::OnMouseWheel(int xPos, int yPos, int zDet)
{
	VSWindowApplication::OnMouseWheel(xPos, yPos, zDet);
}
