#pragma once

#include "gameObject.h"
#include <list>
#include <typeinfo>
#include <vector>

//�V�[�����
enum SceneState
{
	SCENE_TITLE,
	SCENE_TUTORIAL,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_NONE,
};

class Scene
{
protected:
	// STL�̃��X�g�\��
	std::list<GameObject*> m_GameObject[3];

	//static SceneState m_SceneState;
public:
	//�{�����[���ݒ�
	static float m_BGMVolume;
	static float m_SEVolume;

	static SceneState m_SceneState;

	//static SceneState GetSceneState() { return m_SceneState; }
	//static void SetSceneState(SceneState state) { m_SceneState = state; }

	virtual void Init() {}

	virtual void Uninit()
	{
		for (int i = 0; i < 3; i++) {
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Uninit();
				delete gameObject;
			}

			// ���X�g�\���̍폜
			m_GameObject[i].clear();
		}
	}

	virtual void Update()
	{
		for (int i = 0; i < 3; i++) {
			// �͈�for���[�v
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Update();
			}

			// �����_��
			m_GameObject[i].remove_if([](GameObject* object)
			{
				return object->Destroy();
			});
		}
	}

	virtual void Draw()
	{
		for (int i = 0; i < 3; i++) {
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Draw();
			}
		}
	}

	virtual void ShadowDraw()
	{
		for (int i = 0; i < 3; i++) {
			for (GameObject* gameObject : m_GameObject[i])
			{
				if(gameObject->GetDepthEnable())
				{
					gameObject->Draw();
				}
			}
		}
	}

	virtual void ReflectDraw()
	{
		for (int i = 0; i < 3; i++) {
			for (GameObject* gameObject : m_GameObject[i])
			{
				if (gameObject->GetReflectEnable())
				{
					gameObject->Draw();
				}
			}
		}
	}

	// ���X�g�\���֒ǉ�
	// �e���v���[�g�֐�
	template <typename T>
	T* AddGameObject(int Layer)
	{
	    T* gameObject = new T();
		m_GameObject[Layer].push_back(gameObject);
		gameObject->Init();
		
		return gameObject;
	}

	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < 3; i++) {
			for (GameObject* object : m_GameObject[i])
			{
				// �^�𒲂ׂ�(RTTI ���I�^���)
				if (typeid(*object) == typeid(T))
				{
					return (T*)object;
				}
			}
		}
		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetGameObjects()
	{
		// STL�̔z��
		std::vector<T*> objects;

		for (int i = 0; i < 3; i++) {
			for (GameObject* object : m_GameObject[i])
			{
				// �^�𒲂ׂ�(RTTI ���I�^���)
				if (typeid(*object) == typeid(T))
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}
};

//SceneState Scene::m_SceneState = SCENE_NONE;