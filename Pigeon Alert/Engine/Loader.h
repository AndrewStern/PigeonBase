#pragma once
#include "pch.h"
#include "DirectXHelper.h"
#include "GameObject.h"
#include "Controller.h"
#include "BasicTimer.h"

class GameObject;

class Loader
{
	public:
		void LoadScene(std::wstring fileName);
		void SaveScene(std::wstring fileName);

	private:
		virtual void * GetObject(std::wstring objectName) { return NULL; }
};