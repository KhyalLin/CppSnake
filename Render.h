#pragma once
#include <Windows.h>
#include <stdexcept>
#include "Basic.h"

class Render
{
private:
	wchar_t* screen;
	HANDLE hConsole;
	DWORD dwBytesWritten;

public:
	Render();
	~Render();

	void wait();
	void draw();
	void draw(const Snake&);
	void draw(const Food&);
	void show();
};

Render::Render() : dwBytesWritten(0)
{
	screen = new wchar_t[nScreenWidth * nScreenHeight];
	if (!screen) throw std::runtime_error("Render");
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ |
		GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
}

Render::~Render()
{
	delete[] screen;
}

void Render::wait()
{
	wsprintf(&screen[nScreenWidth * (nScreenHeight / 2) + 45],
		L"Press \"Space\" to Start Again!");
	show();

	while ((GetAsyncKeyState(0x20) & 0x8000) == 0)
		continue;
}

void Render::draw()
{
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
		screen[i] = L' ';

	for (int i = 0; i < nScreenWidth; i++)
	{
		screen[i] = L'=';
		screen[nScreenWidth * 2 + i + 1] = L'=';
	}
	wsprintf(&screen[nScreenWidth + 50],
		L"S N A K E!!                 Score: ");
}

void Render::draw(const Snake& s)
{
	for (Snake::SnakeSegment s : s.snake)
		screen[s.y * nScreenWidth + s.x] = L'O';
	screen[s.snake.front().y * nScreenWidth + s.snake.front().x] = L'@';
	wsprintf(&screen[nScreenWidth + 85], 
		L"%d ", s.score());
}

void Render::draw(const Food& f)
{
	wchar_t apple;
	switch (f.mode()) {
	case Mode::normal: apple = L'#'; break;
	case Mode::bonus: apple = L'$'; break;
	case Mode::accerate: apple = L'%'; break;
	}
	screen[f.y() * nScreenWidth + f.x()] = apple;
}

void Render::show()
{
	WriteConsoleOutputCharacterW(hConsole, screen,
		nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
}