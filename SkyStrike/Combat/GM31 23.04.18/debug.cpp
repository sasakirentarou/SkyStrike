#include "main.h"
#include "debug.h"
#include "input.h"
#include "scene.h"
#include "manager.h"
#include "jet.h"
#include "enemyJet.h"
#include "collisionBox.h"

bool Debug::m_DebugMode{};
float Debug::m_Frame = 60.0f;

void Debug::Update()
{
	Scene* scene = Manager::GetScene();
	Jet* jet = scene->GetGameObject<Jet>();
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

		{//Player
			ImGui::Begin("Jet");
			ImGui::InputFloat3("Position", jet->GetPosition());
			ImGui::InputFloat4("Quaternion", jet->GetQuaternion());
			ImGui::InputFloat3("Velocity", jet->GetVelocity());
			ImGui::End();
		}

		{//CollisionBox
			ImGui::Begin("CollsionBox");
			for (CollisionBox* collision : collisions)
			{
				collision->SetDrawFlg(true);
				ImGui::InputFloat3("Scale", collision->GetScale());
				//ImGui::Checkbox("Draw", &);
			}
			ImGui::End();
		}


			break;

		case SCENE_GAME:

		{//Player
			ImGui::Begin("Jet");
			ImGui::InputFloat3("Position", jet->GetPosition());
			ImGui::InputFloat4("Quaternion", jet->GetQuaternion());
			ImGui::InputFloat3("Velocity", jet->GetVelocity());
			ImGui::End();
		}

		{//EnemyJet
			ImGui::Begin("EnemyJet");
			for (EnemyJet* enemy : enemys)
			{
				int id = enemy->GetEnemyID();
				int health = enemy->GetHealth();
				ImGui::InputInt("ID", &id);
				ImGui::InputFloat3("Position", enemy->GetPosition());
				ImGui::InputFloat4("Quaternion", enemy->GetQuaternion());
				ImGui::InputFloat3("Velocity", enemy->GetVelocity());
				ImGui::InputInt("HP", &health);
			}
			ImGui::End();
		}

		{//CollisionBox
			ImGui::Begin("CollsionBox");
			for (CollisionBox* collision : collisions)
			{
				collision->SetDrawFlg(true);
				ImGui::InputFloat3("Scale", collision->GetScale());
				//ImGui::Checkbox("Draw", &);
			}
			ImGui::End();
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
