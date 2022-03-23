#pragma once

#include <list>
#include <vector>
#include <typeinfo>
#include "gameObject.h"

class BehaviorTree;

#define OBJECT_TYPE	(3)

class Scene
{
protected:
	std::list<GameObject*>	m_GameObject[OBJECT_TYPE];//STLのリスト構造

public:
	Scene(){}
	virtual ~Scene(){}

	virtual void Init() = 0;// 純粋仮想関数

	template<typename T> //テンプレート関数
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObject[Layer].push_back(gameObject);

		return gameObject;
	}

	template<typename T>
	T* AddGameObject(int Layer,BehaviorTree *ai_tree)
	{
		T* gameObject = new T(ai_tree);
		gameObject->Init();
		m_GameObject[Layer].push_back(gameObject);

		return gameObject;
	}

	template<typename T>
	T* AddNumber(int Layer, int num)
	{
		T* gameObject = new T(num);
		gameObject->Init();
		m_GameObject[Layer].push_back(gameObject);

		return gameObject;
	}

	//ポインタを取得できる
	template<typename T>
	T* GetGameObject(int Layer)
	{
		for (GameObject* object : m_GameObject[Layer])
		{
			if (typeid(*object) == typeid(T))//型を調べる(RTTI動的型情報)
			{
				return(T*)object;
			}
		}
		return NULL;
	}

	template <typename T>
	std::vector<T*> GetGameObjects(int Layer)
	{
		std::vector<T*> objects;//STLの配列
		for (GameObject* object : m_GameObject[Layer])
		{
			if (typeid(*object) == typeid(T))
			{
				objects.push_back((T*)object);
			}
		}
		return objects;
	}

	virtual void Uninit()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Uninit();
				delete object;
			}
			m_GameObject[i].clear();//リストのクリア
		}
	}

	virtual void Update()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])//範囲forループ
			{
				object->Update();
			}
			//ラムダ式
			m_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });
		}
	}

	virtual void Draw()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Draw();
			}
		}
	}
};