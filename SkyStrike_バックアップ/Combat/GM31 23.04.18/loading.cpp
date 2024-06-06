#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "loading.h"
#include "loadingLogo.h"
#include "loadingScreen.h"
#include "input.h"
#include "fade.h"

#include <thread>

bool Loading::m_GameLoad = false;
bool Loading::m_TutorialLoad = false;

void Loading::Init()
{
	AddGameObject<LoadingScreen>(2);
	AddGameObject<LoadingLogo>(2);

	if(m_GameLoad)
	{
		std::thread th(&Game::Load);
		th.detach();
	}
	else if (m_TutorialLoad)
	{
		std::thread th(&Tutorial::Load);
		th.detach();
	}
}

void Loading::Update()
{
	Scene::Update();

	if (Game::GetLoadFinish())
	{
		Manager::SetScene<Game>();
	}
	else if (Tutorial::GetLoadFinish())
	{
		Manager::SetScene<Tutorial>();
	}
}
