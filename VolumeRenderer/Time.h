#ifndef Time_h__
#define Time_h__
#include <windows.h>
class Time
{
public:
	Time();
	~Time();
	float DeltaTime()const;
	float PlayTime()const;

	void Tick();
private:
	float m_DeltaTime;
	DWORD m_PlayTime;
};
#endif // Time_h__
