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

public:
	Example()
	{
		sAppName = "Visualizing";
	}

	bool OnUserCreate() override
	{
		ballArr.clear();
		ballArr.push_back(Ball(2, 0, 500, 500, 10));

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		FillRect(vi2d(0, 0), vi2d(1000, 1000), BLACK);

		for (int i = 0; i < ballArr.size(); i++)
		{
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
	if (demo.Construct(1000, 1000, 1, 1))
		demo.Start();

	return 0;
}