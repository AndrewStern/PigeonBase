#include "pch.h"
#include "TextureManager.h"

TextureEntry * TextureManager::AddTexture(Main * p_Main, std::wstring _TextureName, bool _IsSprite, int _Rows, int _Columns)
{	
	std::wstring fileName = L"Assets/";
	fileName+=_TextureName;
	TextureEntry * tex = TextureManager::m_textureStore[_TextureName];

		Texture* _TempTexture = new Texture();
		
		// Set Texture numbers of rows
		_TempTexture->Rows = _Rows;
		// Set texture number of colums
		_TempTexture->Columns = _Columns;
		// Set texture name
		_TempTexture->m_TextureName = _TextureName;
		// Set texture sprite 
		_TempTexture->isSprite = _IsSprite;
		// Set texture bitmap
		if (tex == NULL)
		{
			_TempTexture->m_Bitmap = p_Main->CreateDirectXResources(fileName);
		}
		else
		{
			//_TempTexture->m_Bitmap = p_Main->CreateDirectXResources(fileName);

			_TempTexture->m_Bitmap=tex->texture->m_Bitmap;
		}
		// Set width and of the texture
		_TempTexture->SetSize( _TempTexture->m_Bitmap->GetSize().width, _TempTexture->m_Bitmap->GetSize().height);
		// Set the time animation play time
		_TempTexture->FrameSpeed = 10.0f;

		TextureEntry * te = new TextureEntry();
		te->refCount = 1;
		te->texture = _TempTexture;
		te->textureName = _TextureName;
		TextureManager::m_textureStore[_TextureName] = te;
		return te;
	/*}
	else
	{
		tex->refCount++;
		return tex;
	}*/
}
void TextureManager::DeleteTexture(std::wstring _TextureName)
{
	/*TextureEntry * te = TextureManager::m_textureStore[_TextureName];
	if (te != NULL)
	{
		te->refCount--;
		if (te->refCount==0)
		{
			m_textureStore.erase(_TextureName);
			te->texture->m_Bitmap.ReleaseAndGetAddressOf();
			delete te->texture;
			delete te;
		}
	}*/
}