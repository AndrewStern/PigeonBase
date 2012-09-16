#pragma once
#include "BasicTimer.h"
#include <iostream>
#include <list>

using namespace std;

class Texture
{
private:
	int currentColum;
	
	int currentRow;

	float frameStartTime;
	float nextFrameTime;

	bool NextFrame;
	bool m_Pause;
	GameJam::BasicTimer^ Timer;
public:
	int m_TextureID;
	int Rows;
	int Columns;
	int CurrentFrame;
	int NumberOfFrames;

	float width;
	float height;
	float FrameRight;
	float FrameLeft;
	float FrameTop;
	float FrameBotom;
	float FrameSpeed;

	bool isSprite;

	std::wstring m_TextureName;
	Microsoft::WRL::ComPtr<ID2D1Bitmap> m_Bitmap;

	void SetSize(float _Width, float _Height);
	void PlaySprite();
	void PauseSprite();
	void ResumeSprite();
	bool IsPlaying();
	void SetTextureFrame(int _FrameNumber);
	Texture(void);
};

