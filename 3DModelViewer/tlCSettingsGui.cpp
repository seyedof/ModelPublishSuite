#include "stdafx.h"
#include "tlCSettingsGui.h"
#include <imgui/imgui.h>
#include "tlCSettings.h"
#include "Core/tlMacros.h"
#include <Core/tlC3DViewContext.h>

CSettingsGui::CSettingsGui()
{
}


CSettingsGui::~CSettingsGui()
{
}

bool CSettingsGui::Initialize()
{
	SetWndRect(0, 0, 250, 275);
	ResetToDefaults();

	return true;
}

void CSettingsGui::CleanUp()
{
}

void CSettingsGui::Update(C3DViewContext* pView)
{
	if (!m_bVisible)
		return;

	ImGuiWindowFlags flags = 0;
	//flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;

	CSettings::SETTINGS* pSettings = CSettings::GetInstance()->Settings();
	ENGINE_SETTINGS* pEngSettings = CEngineSettings::GetInstance()->GetSettings();

	if (m_bFirstTime) {
		int sw = pView->GetViewport().Width;
		ImGui::SetNextWindowPos(ImVec2(sw - m_fWndWidth, 0));
		m_bFirstTime = false;
	}

	if ( ImGui::Begin("Settings", &m_bVisible, ImVec2(m_fWndWidth, m_fWndHeight), m_fAlpha, flags) ) {

		ImGui::Checkbox("Show FPS", &pSettings->bShowFPS);
		ImGui::Checkbox("Show Sky", &pEngSettings->Sky.bShowSky);
		ImGui::Checkbox("Show Terrain", &pEngSettings->Terrain.bShowTerrain);
		ImGui::Checkbox("Show Foliage", &pEngSettings->Foliage.bShowFoliage);
		ImGui::Checkbox("Show Objects", &pEngSettings->Objects.bShowObjects);
		ImGui::Checkbox("Show Physics", &pEngSettings->Physics.bShowPhysicsModels);
		ImGui::Checkbox("Show Scene Nodes", &pEngSettings->Scene.bShowSceneNodes);
		ImGui::Checkbox("Show Object Bounding", &pEngSettings->Objects.bShowBoundings);
		ImGui::Checkbox("Show Object Path", &pEngSettings->Objects.bShowPath);
		ImGui::Checkbox("Wire-frame Terrain", &pEngSettings->Terrain.bWireframe);
		ImGui::PushItemWidth(120.0f);
		ImGui::InputFloat("Transparency", &m_fAlpha, 0.01f, 0.1f, 2);
		ImGui::PopItemWidth();
		VALIDATE_RANGE(m_fAlpha, 0.3f, 1.0f);
		ImGui::End();
	}

	if ( ! m_bVisible )
		OnShow ( false ) ;
}

void CSettingsGui::OnShow(bool bShow)
{
}

void CSettingsGui::ResetToDefaults()
{
	m_bVisible = true;
	m_fAlpha = 0.7f;
	m_bShowFps = true;
	m_bFirstTime = true;
}
