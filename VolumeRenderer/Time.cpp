#include "Time.h"

Time::Time() :m_PlayTime(0)
{
	m_DeltaTime = 0.0f;
}
Time::~Time()
{
}

//Tick works out the current Delta Time and Play Time for the application 
void Time::Tick()
{
	DWORD currentTime = GetTickCount();
	m_DeltaTime = min((float)(currentTime - m_PlayTime) / 1000.0f, 0.1f);
	m_PlayTime = currentTime;
}

//Returns the elapsed time between each frame
float Time::DeltaTime()const
{
	return m_DeltaTime;
}
//returns the current total application play time
float Time::PlayTime()const
{
	return (float)m_PlayTime;
}


