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
	float mass;

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
		mass = 3.14159265358979323846 * Radius * Radius;
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

	void CheckBallCollision(Ball& ball1, Ball& ball2)
	{
		float dx = ball2.x - ball1.x;
		float dy = ball2.y - ball1.y;
		float distance = dx * dx + dy * dy;
		float totalRadius = ball2.radius + ball1.radius;
		if (distance < totalRadius * totalRadius)
		{
			float velocityX = ball1.velocityX - ball2.velocityX;
			float velocityY = ball1.velocityY - ball2.velocityY;
			float inverseTotalMass = 1 / (ball1.mass + ball2.mass);
			float movingMassComponent = ball1.mass - ball2.mass;
			float stationaryMassComponent = 2 * ball1.mass;

			ball1.velocityX = (movingMassComponent * inverseTotalMass * velocityX) + ball2.velocityX;
			ball2.velocityX = (stationaryMassComponent * inverseTotalMass * velocityX) + ball2.velocityX;

			ball1.velocityY = (movingMassComponent * inverseTotalMass * velocityY) + ball2.velocityY;
			ball2.velocityY = (stationaryMassComponent * inverseTotalMass * velocityY) + ball2.velocityY;
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
		ballArr.push_back(Ball(2, 5, 100, 100, 30));
		ballArr.push_back(Ball(-3, -2, 200, 100, 40));

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		FillRect(vi2d(0, 0), vi2d(ScreenWidth(), ScreenHeight()), BLACK);

		CheckBallCollision(ballArr[0], ballArr[1]);

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