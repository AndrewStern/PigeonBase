#include "pch.h"
#include "Texture.h"
#include "BasicTimer.h"

/*

Ex -> Texture Sprite work

		   Colums
		  0|  1|   2|   3|
	  0| F0| F1|  F2|  F3|
Rows  1| F4| F5|  F6|  F7|
	  2| F8| F9| F10| F11|

FX <- Frames 
Total Frames = 12

*/

Texture::Texture(void)
{
	Timer = ref new GameJam::BasicTimer();
	CurrentFrame = 0;
	currentColum = 0;
	currentRow = 0;
	NextFrame = true;
	m_Pause = false;
}

void Texture::SetSize(float _Width, float _Height)
{
	if(isSprite)
	{
		width = _Width/Columns;
		height = _Height/Rows;
	}
	else
	{
		width = _Width;
		height = _Height;
	}

	NumberOfFrames = Rows*Columns;
}

void Texture::PlaySprite()
{
	if(!m_Pause)
	{
		float _TimeForSingleRow = (1/FrameSpeed)/Rows;
		float _TimeForSingleColum = (1/FrameSpeed)/Columns;

		if(isSprite && (FrameSpeed>0) )
		{
		
			#pragma region Single Frame

				FrameLeft = width  * currentColum;	
				FrameRight = width + (width  * currentColum);

				FrameTop = height * currentRow;
				FrameBotom = height + ( height  * currentRow);


				Timer->Update();
				if(NextFrame)
				{
					frameStartTime = Timer->Total;
					NextFrame = false;
				}

				if((Timer->Total - frameStartTime) >= _TimeForSingleRow)
				{
					currentColum++;
					NextFrame = true;
				
					CurrentFrame++;
				}

				if(currentColum >= Columns )
				{
					currentColum = 0;
					currentRow++;
				}

				if(currentRow >= Rows)
				{
					currentRow = 0;
					CurrentFrame = 0;
				}
			#pragma endregion
				
		}
		else
		{
			FrameLeft = 0;
			FrameRight = width;

			FrameTop = 0;
			FrameBotom = height;
		}
	}

}

void Texture::PauseSprite()
{
	m_Pause = true;
}

void Texture::ResumeSprite()
{
	m_Pause = false;
}

// Set the frame according to given frame number
void Texture::SetTextureFrame(int _FrameNumber)
{
	int _FrameRow, _FrameColum; 
	
	if(_FrameNumber > NumberOfFrames)
	{
		_FrameNumber = NumberOfFrames-1;
	}
	CurrentFrame = _FrameNumber;
	if(Rows>=Columns)
	{
		_FrameRow  = _FrameNumber/Rows;
		_FrameColum  = _FrameNumber%Rows;
	}
	else
	{
		_FrameRow  = _FrameNumber/Columns;
		_FrameColum  = _FrameNumber%Columns;
	}

	currentColum = _FrameColum;
	currentRow = _FrameRow;

	FrameLeft = width  * currentColum;	
	FrameRight = width + (width  * currentColum);

	FrameTop = height * currentRow;
	FrameBotom = height + ( height  * currentRow);
}

bool Texture::IsPlaying()
{
	return m_Pause;
}

void Texture::SetFrameSpeed(float _Speed)
{
	FrameSpeed = _Speed;
}	