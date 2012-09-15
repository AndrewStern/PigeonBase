#pragma once

#include <wrl/client.h>
#include <wrl.h>
#include <d3d11_1.h>
#include <d2d1_1.h>
#include <d2d1effects.h>
#include <dwrite_1.h>
#include <wincodec.h>
#include <vector>
#include <memory>
#include <ppltasks.h>

#include <mmreg.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
#include <list>

#include "App.xaml.h"
#include <Box2D\Box2D.h>

#define XM_STRICT_VECTOR4 1
#include <DirectXMath.h>

#define XAUDIO2_HELPER_FUNCTIONS 1
#include <xaudio2.h>
#include <xaudio2fx.h>

#include "Engine\BasicMath.h"

#include "Game\PlayerTurret.h"
#include "Game\Pigeon.h"


//class Vector2
//{
//public:
//	float x;
//	float y;
//
//	Vector2()
//	{
//		x = 0.0f;
//		y = 0.0f;
//	}
//
//	Vector2(float x, float y)
//	{
//		this->x = x;
//		this->y = y;
//	}
//
//	Vector2 operator+(const Vector2 &v) const 
//	{
//		return Vector2(x + v.x, y + v.y);
//	}
//
//	Vector2 operator*(const Vector2 &v) const 
//	{
//		return Vector2(x * v.x, y * v.y);
//	}
//
//	Vector2 operator/(const Vector2 &v) const 
//	{
//		return Vector2(x / v.x, y / v.y);
//	}
//
//	Vector2 & operator+=(const Vector2 &rhs)
//	{
//		this->x += rhs.x;
//		this->y += rhs.y;
//		return *this;
//	}
//	
//	Vector2 & operator-=(const Vector2 &rhs)
//	{
//		this->x -= rhs.x;
//		this->y -= rhs.y;
//		return *this;
//	}
//	
//	Vector2 & operator*=(const Vector2 &rhs)
//	{
//		this->x *= rhs.x;
//		this->y *= rhs.y;
//		return *this;
//	}
//
//	Vector2 & operator/=(const Vector2 &rhs)
//	{
//		this->x /= rhs.x;
//		this->y /= rhs.y;
//		return *this;
//	}
//};

//subclass b2QueryCallback
class MyQueryCallback : public b2QueryCallback 
{
public:
    std::vector<b2Body*> foundBodies;
      
    bool ReportFixture(b2Fixture* fixture) 
	{
        foundBodies.push_back( fixture->GetBody() ); 
        return true;//keep going to find all fixtures in the query area
    }
};

