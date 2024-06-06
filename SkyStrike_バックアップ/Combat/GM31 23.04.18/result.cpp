#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "title.h"
#include "result.h"
#include "input.h"
#include "inputx.h"
#include "fade.h"
#include "textureManager.h"
#include "audio.h"

void Result::Init()
{
	Scene::m_SceneState = SCENE_RESULT;

	// polygon
	m_Texture = AddGameObject<TextureManager>(2);
	m_Texture->SetSceneTexture(TEXTURE_RESULT);

	m_Fade = AddGameObject<Fade>(2);

	m_ConfirmSE = AddGameObject<GameObject>(0)->AddComponet<Audio>();
	m_ConfirmSE->Load("asset\\audio\\confirm.wav");
	
	m_BGM = AddGameObject<GameObject>(0)->AddComponet<Audio>();
	m_BGM->Load("asset\\audio\\result_bgm.wav");
	m_BGM->Play(true);
}

void Result::Update()
{
	Scene::Update();

	//ƒ{ƒŠƒ…[ƒ€
	m_ConfirmSE->Volume(Scene::m_SEVolume * (0.3f * 2));
	m_BGM->Volume(Scene::m_BGMVolume * (0.08 * 2));

	if (Input::GetKeyTrigger(VK_RETURN) || InputX::IsButtonTriggered(0,XINPUT_GAMEPAD_A))
	{
		m_ConfirmSE->Play(true);
		m_Fade->FadeOut();
	}
	if (m_Fade->GetFadeFinish())
	{
		Manager::SetScene<Title>();
	}
}
