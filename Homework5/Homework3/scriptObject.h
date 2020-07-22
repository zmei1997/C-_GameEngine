#pragma once

class scriptObject
{
private:
	int m_r = 0;
	int m_g = 0;
	int m_b = 0;
	float m_length = 0.f;
	float m_width = 0.f;
public:
	scriptObject();
	int GetR();
	int GetG();
	int GetB();
	float GetLenght();
	float GetWidth();
	void SetR(int val);
	void SetG(int val);
	void SetB(int val);
	void SetLength(float val);
	void SetWidth(float val);
};

