#pragma once
#include"pch.h"

class Timer :public Singleton<Timer>
{
	friend class Singleton<Timer>;

private:
	double m_dGameTime;
	double m_dSecondPerFrame;
	double m_dFramePerSecond;
	std::chrono::high_resolution_clock::time_point m_BeforeTime;
	int m_iFps;

public:
	int GetFPS();
	double GetSecPerFrame() const { return m_dSecondPerFrame; };
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

};

