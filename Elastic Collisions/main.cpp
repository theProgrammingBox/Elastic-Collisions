#define OLC_PGE_APPLICATION
#include <iostream>
#include "olcPixelGameEngine.h"
#include "Randoms.h"

using namespace std;
using namespace olc;

const float AIR_FRICTION = 0.99999999;
const float CONTACT_DAMPENING = 0.9999;
const int NUM_BALLS = 1000;
const int BATCHES = 20;
const vf2d GRAVITY = { 0.0f, 200.0f };

struct Ball
{
	vf2d position;
	vf2d velocity;
	float radius;
	float mass;
	Pixel color;

	Ball(
		float X = 0, float Y = 0,
		float VelocityX = 0, float VelocityY = 0,
		float Radius = 0,
		Pixel Color = Pixel(0, 0, 0, 0))
	{
		position = vf2d(X, Y);
		velocity = vf2d(VelocityX, VelocityY);
		radius = Radius;
		mass = 3.14159265358979323846 * Radius * Radius;
		color = Color;
	}

	void Update(float fElapsedTime)
	{
		velocity *= AIR_FRICTION;
		velocity += GRAVITY * fElapsedTime;
		position += velocity * fElapsedTime;
	}

	void BounceOffWalls(int screenWidth, int screenHeight)
	{
		if (position.x < radius)
		{
			position.x = radius;
			velocity.x *= -CONTACT_DAMPENING;
		}

		if (position.x > screenWidth - radius)
		{
			position.x = screenWidth - radius;
			velocity.x *= -CONTACT_DAMPENING;
		}

		if (position.y < radius)
		{
			position.y = radius;
			velocity.y *= -CONTACT_DAMPENING;
		}

		if (position.y > screenHeight - radius)
		{
			position.y = screenHeight - radius;
			velocity.y *= -CONTACT_DAMPENING;
		}
	}

	void BallCollision(Ball& ball)
	{
		vf2d dPosition = position - ball.position;
		float distanceSquared = dPosition.mag2();
		float totalRadius = ball.radius + radius;
		if (distanceSquared < totalRadius * totalRadius)
		{
			float totalMass = mass + ball.mass;
			vf2d temp = CONTACT_DAMPENING * 2 * (velocity - ball.velocity).dot(dPosition) * dPosition / distanceSquared;
			float ratio1 = ball.mass / totalMass;
			float ratio2 = mass / totalMass;

			velocity -= ratio1 * temp;
			ball.velocity += ratio2 * temp;

			float distance = sqrt(distanceSquared);
			temp = dPosition * (distance - totalRadius) / distance;
			position -= ratio1 * temp;
			ball.position += ratio2 * temp;
		}
	}
};

class Example : public olc::PixelGameEngine
{
private:
	vector<Ball> ballArr;
	Ball* selectedBall = NULL;
	vf2d prevMousePos;
	vf2d mousePos;

	Pixel mapToRainbow(double d) { // 0 - 1
		d *= 6;
		double r = (d > 4) ? max(0.0, min(1.0, 6 - d)) : max(0.0, min(1.0, d - 2));
		double g = (d > 2) ? max(0.0, min(1.0, 4 - d)) : max(0.0, min(1.0, d));
		double b = (d > 3) ? max(0.0, min(1.0, d - 4)) : max(0.0, min(1.0, 2 - d));

		return Pixel(r * 0xff, g * 0xff, b * 0xff);
	}

	void MouseOptions(float fElapsedTime)
	{
		if (GetMouse(0).bPressed || GetMouse(1).bPressed && ballArr.size() != 0)
		{
			selectedBall = NULL;
			float minDistanceSquared = INFINITE;

			for (Ball& ball : ballArr)
			{
				float distanceSquared = (mousePos - ball.position).mag2();
				if (distanceSquared < minDistanceSquared && distanceSquared < ball.radius * ball.radius)
				{
					minDistanceSquared = distanceSquared;
					selectedBall = &ball;
				}
			}
		}

		if (GetMouse(0).bHeld)
		{
			if (selectedBall != NULL)
			{
				selectedBall->velocity = 100 * (mousePos - selectedBall->position);
			}
		}

		if (GetMouse(0).bReleased)
		{
			selectedBall = NULL;
		}

		if (GetMouse(1).bReleased)
		{
			if (selectedBall != NULL)
			{
				selectedBall->velocity = 10 * (mousePos - selectedBall->position);
				selectedBall = NULL;
			}
		}
	}

public:
	Example()
	{
		sAppName = "Circle Physics";
	}

	bool OnUserCreate() override
	{
		Random random;
		ballArr.clear();
		for (int i = 0; i < NUM_BALLS; i++)
		{
			vf2d vec = vf2d(random.DoubleRandom(), random.DoubleRandom());
			vec.norm();
			vec *= 100;
			ballArr.push_back(Ball(
				random.UDoubleRandom() * ScreenWidth(),
				random.UDoubleRandom() * ScreenHeight(),
				vec.x, vec.y,
				random.UDoubleRandom() * 8 + 4,
				mapToRainbow(random.UDoubleRandom())));
		}
		prevMousePos = vf2d(GetMouseX(), GetMouseY());
		mousePos = vf2d(GetMouseX(), GetMouseY());

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		mousePos = vf2d(GetMouseX(), GetMouseY());

		for (int i = 0; i < ballArr.size(); i++)
		{
			FillCircle(ballArr[i].position, ballArr[i].radius, BLACK);
		}

		if (selectedBall != NULL)
		{
			DrawLine(selectedBall->position, prevMousePos, BLACK);
		}

		MouseOptions(fElapsedTime);

		for (int iteration = 0; iteration < BATCHES; iteration++)
		{
			for (Ball& ball : ballArr)
			{
				ball.Update(fElapsedTime / BATCHES);

				ball.BounceOffWalls(ScreenWidth(), ScreenHeight());
			}

			for (int i = 0; i < ballArr.size() - 1; i++)
			{
				for (int j = i + 1; j < ballArr.size(); j++)
				{
					ballArr[i].BallCollision(ballArr[j]);
				}
			}
		}

		for (int i = 0; i < ballArr.size(); i++)
		{
			FillCircle(ballArr[i].position, ballArr[i].radius, ballArr[i].color * 0.8);
			FillCircle(ballArr[i].position, ballArr[i].radius * 0.8, ballArr[i].color);
		}

		if (selectedBall != NULL)
		{
			DrawLine(selectedBall->position, mousePos, WHITE);
		}

		prevMousePos = vf2d(GetMouseX(), GetMouseY());

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