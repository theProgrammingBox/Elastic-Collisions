#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace olc;

const float GRAVITY = 0.1;

struct Ball
{
	vf2d velocity;
	vf2d position;
	vf2d futureVelocity;
	float radius;
	float mass;

	Ball(float VelocityX, float VelocityY, float X, float Y, float Radius)
	{
		velocity = vf2d(VelocityX, VelocityY);
		futureVelocity = vf2d(VelocityX, VelocityY);
		position = vf2d(X, Y);
		radius = Radius;
		mass = 3.14159265358979323846 * Radius * Radius;
	}

	void Move(float time)
	{
		position += velocity * time;
	}

	void ApplyGravity(float time)
	{
		velocity += vf2d(0, GRAVITY) * time;
	}

	void CheckBallCollision(Ball& ball)
	{
		vf2d dPosition = position - ball.position;
		float distanceSquared = dPosition.mag2();
		float totalRadius = ball.radius + radius;
		if (distanceSquared < totalRadius * totalRadius)
		{
			vf2d dDelocity = velocity - ball.velocity;
			float dot = dDelocity.dot(dPosition);

			float inverseTotalMass = 1 / (mass + ball.mass);
			float massComponent1 = 2 * ball.mass * inverseTotalMass;
			float massComponent2 = 2 * mass * inverseTotalMass;

			futureVelocity = velocity - massComponent1 * dot / distanceSquared * dPosition;
			ball.futureVelocity = ball.velocity - massComponent2 * dot / distanceSquared * (vf2d(0, 0) - dPosition);
		}
	}

	void CheckBoarder(vf2d boarder)
	{
		if (position.x < radius || position.x + radius > boarder.x)
		{
			futureVelocity.x = 0 - velocity.x;
		}
		if (position.y < radius || position.y + radius > boarder.y)
		{
			futureVelocity.y = 0 - velocity.y;
		}
	}

	void UpdateVelocity()
	{
		velocity = futureVelocity;
	}
};

class Example : public olc::PixelGameEngine
{
private:
	vector<Ball> ballArr;
	float time;
	vf2d boarder;

	void RenderBall(Ball ball, Pixel color)
	{
		FillCircle(ball.position, ball.radius, color);
	}

public:
	Example()
	{
		sAppName = "Collision";
	}

	bool OnUserCreate() override
	{
		ballArr.clear();
		ballArr.push_back(Ball(0.3, 0, 100, 100, 50));
		ballArr.push_back(Ball(-0.2, -0.2, 200, 100, 40));
		ballArr.push_back(Ball(0.3, -0.2, 200, 200, 30));
		ballArr.push_back(Ball(0.3, 0.2, 100, 200, 20));
		time = 1;
		boarder = vf2d(ScreenWidth(), ScreenHeight());

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		for (int i = 0; i < ballArr.size(); i++)
		{
			RenderBall(ballArr[i], BLACK);
		}

		if (GetKey(olc::Key::UP).bHeld)  time += 0.01;
		if (GetKey(olc::Key::DOWN).bHeld)  time -= 0.01;

		for (int i = 0; i < ballArr.size(); i++)
		{
			//ballArr[i].ApplyGravity(time);

			ballArr[i].Move(time);
			ballArr[i].CheckBoarder(boarder);
			ballArr[i].Move(-time);
			ballArr[i].UpdateVelocity();
		}

		for (int i = 0; i < ballArr.size() - 1; i++)
		{
			for (int j = i + 1; j < ballArr.size(); j++)
			{
				ballArr[i].Move(time);
				ballArr[j].Move(time);

				ballArr[i].CheckBallCollision(ballArr[j]);

				ballArr[i].Move(-time);
				ballArr[j].Move(-time);

				ballArr[i].UpdateVelocity();
				ballArr[j].UpdateVelocity();
			}
		}

		for (int i = 0; i < ballArr.size(); i++)
		{
			//ballArr[i].Move(time);
		}

		for (int i = 0; i < ballArr.size(); i++)
		{
			RenderBall(ballArr[i], WHITE);
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