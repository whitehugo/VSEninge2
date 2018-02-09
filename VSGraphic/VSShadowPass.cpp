#include "VSShadowPass.h"
#include "VSSpatial.h"
#include "VSBoneNode.h"
#include "VSShaderStringFactory.h"
#include "VSConstValue.h"
#include "VSGeometry.h"
#include "VSBoneNode.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSMaterial.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSCubeShadowPass,VSPass)
BEGIN_ADD_PROPERTY(VSCubeShadowPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCubeShadowPass)
ADD_PRIORITY(VSBlendState)
ADD_PRIORITY(VSDepthStencilState)
ADD_PRIORITY(VSRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(CubeShadowPassDraw, )
bool VSCubeShadowPass::InitialDefaultState()
{

	return 1;
}
bool VSCubeShadowPass::TerminalDefaultState()
{

	return 1;
}
VSCubeShadowPass::VSCubeShadowPass()
{

	
	m_pVShaderSet = NULL;
	m_pPShaderSet = NULL;
//	VSBlendDesc BlendDesc;
//	VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);
//	m_RenderState.SetBlendState(pBlendState);
// 	VSRasterizerDesc RasterizerDesc;
// 	RasterizerDesc.m_uiCullType = VSRasterizerDesc::CT_CW;
// 	VSRasterizerState * pRasterizerState= VSResourceManager::CreateRasterizerState(RasterizerDesc);
// 	m_RenderState.SetRasterizerState(pRasterizerState);

}
VSCubeShadowPass::~VSCubeShadowPass()
{

	

	m_pVShaderSet = NULL;
	m_pPShaderSet = NULL;
}


bool VSCubeShadowPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(CubeShadowPassDraw)
	if(!pRenderer || !m_pCamera || !m_pSpatial || !m_pMaterialInstance)
		return 0;

	VSMaterial * pMaterial = m_pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return 0;
	}


	MSPara.pCamera = m_pCamera;
	MSPara.pGeometry = (VSGeometry *)m_pSpatial;
	MSPara.pMaterialInstance = m_pMaterialInstance;
	MSPara.uiPassId = m_uiPassId;
	MSPara.pShadowLight = (VSLight*)m_pPointLight;


	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	if (!GetVShader(MSPara, VSResourceManager::GetInnerVertexShaderMap(), VSUsedName::ms_cCubShadowVertex))
	{
		return 0;
	}
	if (!GetPShader(MSPara,VSResourceManager::GetCubShadowShaderMap(),pMaterial->GetResourceName()))
	{
		return 0;
	}
	
	pRenderer->SetMaterialVShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurPShader[GetPassType()]);

	if(!pRenderer->DrawMesh((VSGeometry *)m_pSpatial,&m_RenderState,
		m_pMaterialInstance->m_pCurVShader[GetPassType()],
		m_pMaterialInstance->m_pCurPShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}
IMPLEMENT_RTTI(VSVolumeShadowPass,VSPass)
BEGIN_ADD_PROPERTY(VSVolumeShadowPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSVolumeShadowPass)
ADD_PRIORITY(VSBlendState)
ADD_PRIORITY(VSDepthStencilState)
ADD_PRIORITY(VSRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(VolumeShadowRenderPassDraw, )
bool VSVolumeShadowPass::InitialDefaultState()
{

	return 1;
}
bool VSVolumeShadowPass::TerminalDefaultState()
{

	return 1;
}
VSVolumeShadowPass::VSVolumeShadowPass()
{


	m_pVShaderSet = NULL;
	m_pPShaderSet = NULL;
	
	VSDepthStencilDesc DepthStencilDesc;
	DepthStencilDesc.m_bDepthWritable = false;
	DepthStencilDesc.m_uiDepthCompareMethod = VSDepthStencilDesc::CM_LESS;
	DepthStencilDesc.m_bStencilEnable = true;
	DepthStencilDesc.m_bTwoSideStencilMode = true;
	DepthStencilDesc.m_uiSPassZFailOP = VSDepthStencilDesc::OT_DECREMENT;
	DepthStencilDesc.m_uiCCW_SPassZFailOP = VSDepthStencilDesc::OT_INCREMENT;
	VSDepthStencilState * pDepthStencilState = VSResourceManager::CreateDepthStencilState(DepthStencilDesc);
	m_RenderState.SetDepthStencilState(pDepthStencilState);

	VSRasterizerDesc RasterizerDesc;
	RasterizerDesc.m_uiCullType = VSRasterizerDesc::CT_NONE;
	VSRasterizerState * pRasterizerState= VSResourceManager::CreateRasterizerState(RasterizerDesc);
	m_RenderState.SetRasterizerState(pRasterizerState);

	VSBlendDesc BlendDesc;
	BlendDesc.ucWriteMask[0] = VSBlendDesc::WM_NONE;
	VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);
	m_RenderState.SetBlendState(pBlendState);

}
VSVolumeShadowPass::~VSVolumeShadowPass()
{



	m_pVShaderSet = NULL;
	m_pPShaderSet = NULL;
}

bool VSVolumeShadowPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(VolumeShadowRenderPassDraw)
	if(!pRenderer || !m_pCamera || !m_pSpatial || !m_pMaterialInstance)
		return 0;

	VSMaterial * pMaterial = m_pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return 0;
	}

	MSPara.pCamera = m_pCamera;
	MSPara.pGeometry = (VSGeometry *)m_pSpatial;
	MSPara.pMaterialInstance = m_pMaterialInstance;
	MSPara.uiPassId = m_uiPassId;
	MSPara.pShadowLight = (VSLight*)m_pLocalLight;


	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	if (!GetVShader(MSPara, VSResourceManager::GetInnerVertexShaderMap(), VSUsedName::ms_cVolumeShadowVertex))
	{
		return 0;
	}
	if (!GetPShader(MSPara,VSResourceManager::GetVolumeShadowShaderMap(),pMaterial->GetResourceName()))
	{
		return 0;
	}

	pRenderer->SetMaterialVShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurPShader[GetPassType()]);

	if(!pRenderer->DrawMesh((VSGeometry *)m_pSpatial,&m_RenderState,
		m_pMaterialInstance->m_pCurVShader[GetPassType()],
		m_pMaterialInstance->m_pCurPShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}




IMPLEMENT_RTTI(VSShadowPass,VSPass)
BEGIN_ADD_PROPERTY(VSShadowPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSShadowPass)
ADD_PRIORITY(VSBlendState)
ADD_PRIORITY(VSDepthStencilState)
ADD_PRIORITY(VSRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(ShadowRenderPassDraw, )
bool VSShadowPass::InitialDefaultState()
{

	return 1;
}
bool VSShadowPass::TerminalDefaultState()
{

	return 1;
}
VSShadowPass::VSShadowPass()
{


	m_pVShaderSet = NULL;
	m_pPShaderSet = NULL;

}
VSShadowPass::~VSShadowPass()
{



	m_pVShaderSet = NULL;
	m_pPShaderSet = NULL;
}


bool VSShadowPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(ShadowRenderPassDraw)
	if(!pRenderer || !m_pCamera || !m_pSpatial || !m_pMaterialInstance)
		return 0;

	VSMaterial * pMaterial = m_pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return 0;
	}


	MSPara.pCamera = m_pCamera;
	MSPara.pGeometry = (VSGeometry *)m_pSpatial;
	MSPara.pMaterialInstance = m_pMaterialInstance;
	MSPara.uiPassId = m_uiPassId;
	MSPara.pShadowLight = (VSLight*)m_pLocalLight;


	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	if (!GetVShader(MSPara, VSResourceManager::GetInnerVertexShaderMap(), VSUsedName::ms_cShadowVertex))
	{
		return 0;
	}
	if (!GetPShader(MSPara,VSResourceManager::GetShadowShaderMap(),pMaterial->GetResourceName()))
	{
		return 0;
	}

	pRenderer->SetMaterialVShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurPShader[GetPassType()]);

	if(!pRenderer->DrawMesh((VSGeometry *)m_pSpatial,&m_RenderState,
		m_pMaterialInstance->m_pCurVShader[GetPassType()],
		m_pMaterialInstance->m_pCurPShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}



IMPLEMENT_RTTI(VSDualParaboloidShadowPass,VSPass)
BEGIN_ADD_PROPERTY(VSDualParaboloidShadowPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDualParaboloidShadowPass)
ADD_PRIORITY(VSBlendState)
ADD_PRIORITY(VSDepthStencilState)
ADD_PRIORITY(VSRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(DualParaboloidRenderPassDraw, )
bool VSDualParaboloidShadowPass::InitialDefaultState()
{

	return 1;
}
bool VSDualParaboloidShadowPass::TerminalDefaultState()
{

	return 1;
}
VSDualParaboloidShadowPass::VSDualParaboloidShadowPass()
{


	m_pVShaderSet = NULL;
	m_pPShaderSet = NULL;

}
VSDualParaboloidShadowPass::~VSDualParaboloidShadowPass()
{



	m_pVShaderSet = NULL;
	m_pPShaderSet = NULL;
}


bool VSDualParaboloidShadowPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(DualParaboloidRenderPassDraw)
	if(!pRenderer || !m_pCamera || !m_pSpatial || !m_pMaterialInstance)
		return 0;

	VSMaterial * pMaterial = m_pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return 0;
	}

	MSPara.pCamera = m_pCamera;
	MSPara.pGeometry = (VSGeometry *)m_pSpatial;
	MSPara.pMaterialInstance = m_pMaterialInstance;
	MSPara.uiPassId = m_uiPassId;
	MSPara.pShadowLight = (VSLight*)m_pPointLight;

	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	if (!GetVShader(MSPara, VSResourceManager::GetInnerVertexShaderMap(), VSUsedName::ms_cDualParaboloidShadowVertex))
	{
		return 0;
	}
	if (!GetPShader(MSPara,VSResourceManager::GetDualParaboloidShadowShaderMap(),pMaterial->GetResourceName()))
	{
		return 0;
	}

	pRenderer->SetMaterialVShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurPShader[GetPassType()]);

	if(!pRenderer->DrawMesh((VSGeometry *)m_pSpatial,&m_RenderState,
		m_pMaterialInstance->m_pCurVShader[GetPassType()],
		m_pMaterialInstance->m_pCurPShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}
