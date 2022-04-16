#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace olc;

struct Ball
{
	float velocityX;
	float velocityY;
	float x;
	float y;
	float radius;

	Ball(float VelocityX,
		float VelocityY,
		float X,
		float Y,
		float Radius)
	{
		velocityX = VelocityX;
		velocityY = VelocityY;
		x = X;
		y = Y;
		radius = Radius;
	}

	void Move()
	{
		x += velocityX;
		y += velocityY;
	}
};

class Example : public olc::PixelGameEngine
{
private:
	vector<Ball> ballArr;

	void RenderBall(Ball ball)
	{
		FillCircle(vi2d(ball.x, ball.y), ball.radius);
	}

	void CheckCollision(Ball& ball)
	{
		if (ball.x < ball.radius || ball.x + ball.radius > ScreenWidth())
		{
			ball.velocityX = 0 - ball.velocityX;
		}
		if (ball.y < ball.radius || ball.y + ball.radius > ScreenHeight())
		{
			ball.velocityY = 0 - ball.velocityY;
		}
	}

public:
	Example()
	{
		sAppName = "Collision";
	}

	bool OnUserCreate() override
	{
		ballArr.clear();
		ballArr.push_back(Ball(2, 0, 100, 100, 10));
		ballArr.push_back(Ball(-2, 0, 200, 100, 10));

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		FillRect(vi2d(0, 0), vi2d(ScreenWidth(), ScreenHeight()), BLACK);

		for (int i = 0; i < ballArr.size(); i++)
		{
			CheckCollision(ballArr[i]);
			ballArr[i].Move();
		}

		for (int i = 0; i < ballArr.size(); i++)
		{
			RenderBall(ballArr[i]);
		}

		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(500, 500, 2, 2))
		demo.Start();

	return 0;
}