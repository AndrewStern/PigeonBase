#pragma once

#include <wrl.h>

namespace GameJam
{
	public ref class BasicTimer sealed
	{
	private:
		LARGE_INTEGER	m_frequency;
		LARGE_INTEGER	m_currentTime;
		LARGE_INTEGER	m_startTime;
		LARGE_INTEGER	m_lastTime;
		float			m_total;
		float			m_delta;
		LARGE_INTEGER	m_stopTime;
		bool			m_IsPaused;

	public:
		BasicTimer()
		{
			if (!QueryPerformanceFrequency(&m_frequency))
			{
				throw ref new Platform::FailureException();
			}
			Reset();
		}
	
		void Reset()
		{
			Update();
			m_startTime = m_currentTime;
			m_total = 0.0f;
			m_delta = 1.0f / 60.0f;
		}
	
		void Update()
		{
			if(m_IsPaused)
			{
				return;
			}
			if (!QueryPerformanceCounter(&m_currentTime))
			{
				throw ref new Platform::FailureException();
			}
		
			m_total = static_cast<float>(
				static_cast<double>(m_currentTime.QuadPart-m_startTime.QuadPart) /
				static_cast<double>(m_frequency.QuadPart)
				);
		
			if (m_lastTime.QuadPart==m_startTime.QuadPart)
			{
				// If the timer was just reset, report a time delta equivalent to 60Hz frame time.
				m_delta = 1.0f / 60.0f;
			}
			else
			{
				m_delta = static_cast<float>(
					static_cast<double>(m_currentTime.QuadPart-m_lastTime.QuadPart) /
					static_cast<double>(m_frequency.QuadPart)
					);
			}
		
			m_lastTime = m_currentTime;
		}
	
		property float Total
		{
			float get() { return m_total; }
		}
	
		property float Delta
		{
			float get() { return m_delta; }
		}

		void Pause()
		{
			m_IsPaused = !m_IsPaused;
			if(m_IsPaused)
			{
				m_stopTime = m_currentTime;
			}
			else
			{
				m_startTime = m_stopTime;
			}
		}

	};
}
