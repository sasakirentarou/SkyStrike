#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "loading.h"
#include "loadingLogo.h"
#include "loadingScreen.h"
#include "titleLogo.h"
#include "input.h"
#include "fade.h"

#include <thread>

void Loading::Init()
{
	AddGameObject<LoadingScreen>(2);
	AddGameObject<LoadingLogo>(2);

	std::thread th(&Game::Load);
	th.detach();
}

void Loading::Update()
{
	Scene::Update();

	if (Game::GetLoadFinish)
	{
		Manager::SetScene<Game>();
	}
}
