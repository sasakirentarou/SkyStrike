#include "main.h"
#include "debug.h"
#include "input.h"
#include "scene.h"
#include "manager.h"
#include "jet.h"
#include "enemyJet.h"
#include "collisionBox.h"
#include "camera.h"

bool Debug::m_DebugMode{};
float Debug::m_Frame = 60.0f;

void Debug::Draw()
{
	Scene* scene = Manager::GetScene();
	Jet* jet = scene->GetGameObject<Jet>();
	Camera* camera = scene->GetGameObject<Camera>();
	std::vector <EnemyJet*>		enemys = scene->GetGameObjects<EnemyJet>();
	std::vector <CollisionBox*> collisions = scene->GetGameObjects<CollisionBox>();

	if (m_DebugMode)//デバックモード
	{
		{
			ImGui::Begin("System");
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 100.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::SliderFloat("Frame", &m_Frame, 1.0f, 240.0f);
			if (ImGui::Button("Reset"))
				m_Frame = 60;
			ImGui::End();
			SetFrame(m_Frame);
		}

		switch (Scene::m_SceneState)
		{
		case SCENE_TITLE:
			break;

		case SCENE_TUTORIAL:

			//Player
			jet->Debug();

			//Camera
			camera->Debug();

			//CollisionBox
			for (CollisionBox* collision : collisions)
			{
				collision->Debug();
			}
		

			break;

		case SCENE_GAME:

			//Player
			jet->Debug();

			//Camera
			camera->Debug();

			//EnemyJet
			for (EnemyJet* enemy : enemys)
			{
				enemy->Debug();
			}
		
			//CollisionBox
			for (CollisionBox* collision : collisions)
			{
				collision->Debug();
			}


			break;

		case SCENE_RESULT:
			break;

		default:
			break;
		}
	}
	else
	{
		for (CollisionBox* collision : collisions)
		{
			collision->SetDrawFlg(false);
		}
	}

	if (Input::GetKeyTrigger(VK_F1))
	{
		m_DebugMode = !m_DebugMode;
	}
}
