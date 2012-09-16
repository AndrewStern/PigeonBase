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
	TextureEntry * te = new TextureEntry();

	// Set texture bitmap
	if (tex == NULL)
	{	
		TextureEntry * te2 = new TextureEntry();
		Texture* _TempTexture2 = new Texture();
		_TempTexture2->m_Bitmap = p_Main->CreateDirectXResources(fileName);
		_TempTexture->m_Bitmap=_TempTexture2->m_Bitmap;
		te2->refCount=1;
		// Set width and of the texture
		_TempTexture->SetSize( _TempTexture->m_Bitmap->GetSize().width, _TempTexture->m_Bitmap->GetSize().height);
		// Set the time animation play time
		_TempTexture->FrameSpeed = 10.0f;
		te->texture = _TempTexture;
		te->textureName = _TextureName;
		te2->texture = _TempTexture2;
		te2->textureName = _TextureName;
		TextureManager::m_textureStore[_TextureName] = te2;
	}
	else
	{
		_TempTexture->m_Bitmap=tex->texture->m_Bitmap;
		tex->refCount=tex->refCount+1;
			// Set width and of the texture
		_TempTexture->SetSize( _TempTexture->m_Bitmap->GetSize().width, _TempTexture->m_Bitmap->GetSize().height);
		// Set the time animation play time
		_TempTexture->FrameSpeed = 10.0f;
		te->texture = _TempTexture;
		te->textureName = _TextureName;
	}

	return te;

}
void TextureManager::DeleteTexture(std::wstring _TextureName)
{
	//TODO Tom have a look at this, ive moved te out of the if(te->refCount==0) as its not all a referance now, but deleting that deletes te->texture that is one
	//I cant think of a way round this atm
	TextureEntry * te = TextureManager::m_textureStore[_TextureName];

	
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
	}
}