#pragma once
#include "pch.h"
#include "DirectXHelper.h"
#include "GameObject.h"
#include "Controller.h"
#include "BasicTimer.h"

class GameObject;

class Layer
{
	public:
		Layer(int index);
		bool AddObjectToLayer(void * object);
		void RemoveObject(std::wstring objectName);
		GameObject * GetObjectByName(std::wstring objectName);
		void Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext>);
		void Destroy();
		void Update(unsigned long frameNumber);
		void AddRemoveObjects();
		int layerIndex;
		int objIndex;
		bool layerDeleted;

	private:
		typedef std::map<std::wstring, GameObject *> ObjectMap;
		ObjectMap m_objectByRef;
		std::list<std::wstring> m_deleteQueue;
		std::list<GameObject *> m_addQueue;
};