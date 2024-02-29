#pragma once
#include <random>
#include <thread>
#include <Windows.h>
#include "Basic.h"
#include "Deque.h"

class Food
{
private:
	Mode mod;
	int xFood, yFood;

public:
	friend class Render;

	Food(int x = 30, int y = 15) 
		: xFood(x), yFood(y), mod(Mode::normal) {
	}
	void set(int x, int y) {
		xFood = x, yFood = y;
	}
	void set(Mode _m) {
		mod = _m;
	}
	int x() const {
		return xFood;
	}
	int y() const {
		return yFood;
	}
	Mode mode() const {
		return mod;
	}
};

class Snake
{
private:
	struct SnakeSegment { int x, y; };
	Deque<SnakeSegment> snake;
	Mode mod = Mode::normal;

	int nDirect = 3;
	int nScore = 0;
	bool bDead = false;

public:
	friend class Render;

	bool isdead() const {
		return bDead;
	}
	bool inbody(int x, int y) const {
		for (SnakeSegment s : snake)
			if (x == s.x and y == s.y)
				return true;
		return false;
	}
	int score() const {
		return nScore;
	}

	Snake() {
		for (int i = 0; i < 10; i++)
			snake.push_back({ 60 + i, 15 });
	}

	void wait() const;
	void getDirect();
	void goForward();
	void collide();
	void collide(Food& food);
};

void update(const Snake&, Food&);	// create a new food

void Snake::wait() const
{
	int nTime = 200;
	if (nDirect % 2 == 0)
		nTime *= 1.3;
	if (mod == Mode::accerate)
		nTime /= 1.5;
	std::this_thread::sleep_for( std::chrono::milliseconds(nTime));
}

void Snake::getDirect()
{
	bool bLeft = (GetAsyncKeyState(VK_LEFT) & 0x8000);
	bool bRight = (GetAsyncKeyState(VK_RIGHT) & 0x8000);
	static bool bPrevLeft = false;
	static bool bPrevRight = false;

	if (bLeft and !bPrevLeft)
		nDirect = (nDirect + 3) % 4;
	if (bRight and !bPrevRight)
		nDirect = (nDirect + 1) % 4;
	bPrevLeft = bLeft, bPrevRight = bRight;
}

void Snake::goForward()
{
	int x = snake.front().x,
		y = snake.front().y;

	switch (nDirect) {
	case 0: y--; break;
	case 1: x++; break;
	case 2: y++; break;
	case 3: x--; break;
	}
	snake.push_front({ x, y });
	snake.pop_back();
}

void Snake::collide()
{
	int x = snake.front().x,
		y = snake.front().y;

	if (x < 0 or x >= nScreenWidth)
	{
		bDead = true;
		return;
	}
	if (y < 3 or y >= nScreenHeight)
	{
		bDead = true;
		return;
	}

	for (auto it = snake.begin(); it != snake.end(); it++)
		if (it != snake.begin() and it->x == x and it->y == y)
		{
			bDead = true;
			return;
		}
}

void Snake::collide(Food& food)
{
	if (snake.front().x != food.x() or snake.front().y != food.y())
		return;

	int nLengthInc = 3, nScoreInc = 10;
	if ((mod = food.mode()) == Mode::bonus)
		nLengthInc *= 2, nScoreInc *= 5;
	for (int i = 0; i < nLengthInc; i++)
		snake.push_back({ snake.back().x, snake.back().y });
	nScore += nScoreInc;

	update(*this, food);
}

void update(const Snake& snake, Food& food)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> 
		xdtr(1, nScreenWidth - 2), 
		ydtr(3, nScreenHeight - 2),
		mdtr(1, 10);
	// standard Mersenne Twister algorithm 
	// just using num as dtr(gen) (:

	int x = xdtr(gen), y = ydtr(gen);
	while (snake.inbody(x, y))
		x = xdtr(gen), y = ydtr(gen);
	food.set(x, y);
	
	int m = mdtr(gen);
	Mode _m;
	if (m <= 7) 
		_m = Mode::normal;
	else if (m <= 9) 
		_m = Mode::bonus;
	else 
		_m = Mode::accerate;
	food.set(_m);
}