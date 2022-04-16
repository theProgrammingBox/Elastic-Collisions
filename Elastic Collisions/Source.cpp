#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace olc;

struct Ball
{
	vf2d velocity;
	vf2d position;
	float radius;
	float mass;

	Ball(float VelocityX, float VelocityY, float X, float Y, float Radius)
	{
		velocity = vf2d(VelocityX, VelocityY);
		position = vf2d(X, Y);
		radius = Radius;
		mass = 3.14159265358979323846 * Radius * Radius;
	}

	void Move(float time)
	{
		position += velocity * time;
	}
};

class Example : public olc::PixelGameEngine
{
private:
	vector<Ball> ballArr;
	float time;

	void RenderBall(Ball ball)
	{
		FillCircle(ball.position, ball.radius);
	}

	void CheckCollision(Ball& ball)
	{
		if (ball.position.x < ball.radius || ball.position.x + ball.radius > ScreenWidth())
		{
			ball.velocity.x = 0 - ball.velocity.x;
		}
		if (ball.position.y < ball.radius || ball.position.y + ball.radius > ScreenHeight())
		{
			ball.velocity.y = 0 - ball.velocity.y;
		}
	}

	void CheckBallCollision(Ball& ball1, Ball& ball2)
	{
		vf2d dPosition = ball1.position - ball2.position;
		float distanceSquared = dPosition.mag2();
		float totalRadius = ball2.radius + ball1.radius;
		if (distanceSquared < totalRadius * totalRadius)
		{
			vf2d dDelocity = ball1.velocity - ball2.velocity;
			float dot = dDelocity.dot(dPosition);

			float inverseTotalMass = 1 / (ball1.mass + ball2.mass);
			float massComponent1 = 2 * ball2.mass * inverseTotalMass;
			float massComponent2 = 2 * ball1.mass * inverseTotalMass;

			ball1.velocity = ball1.velocity - massComponent1 * dot / distanceSquared * dPosition;
			ball2.velocity = ball2.velocity - massComponent2 * dot / distanceSquared * (vf2d(0, 0) - dPosition);
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
		time = 0.1;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		FillRect(vi2d(0, 0), vi2d(ScreenWidth(), ScreenHeight()), BLACK);

		if (GetKey(olc::Key::UP).bHeld)  time += 0.01;
		if (GetKey(olc::Key::DOWN).bHeld)  time -= 0.01;

		for (int i = 0; i < ballArr.size(); i++)
		{
			ballArr[i].Move(time);
			CheckCollision(ballArr[i]);
		}
		CheckBallCollision(ballArr[0], ballArr[1]);

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