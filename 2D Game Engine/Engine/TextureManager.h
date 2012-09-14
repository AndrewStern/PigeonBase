#pragma once
#include "Main.h"
#include "Texture.h"
#include <string>
#include <unordered_map>

class Main;

struct TextureEntry
{
	Texture * texture;
	int refCount;
	std::wstring textureName;
};

class TextureManager
{
	public:
		TextureEntry * AddTexture(Main * pMain, std::wstring _TextureName, bool _IsSprite, int _Rows, int _Columns);
		std::unordered_map <std::wstring, TextureEntry *> m_textureStore;
		void DeleteTexture(std::wstring _TextureName);
};