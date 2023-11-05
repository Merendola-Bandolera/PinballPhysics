#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int& y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, float restitution, bool physics);
	PhysBody* CreateRectangle(int x, int y, int width, int height, float restitution, bool physics);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size);
	PhysBody* ModulePhysics::CreateSolidChain(int x, int y, int* points, int size);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);
	int imunderyourskin = 0;
	int lives = 3;
	int bonusScore;
	b2World* world;
private:
	bool launching;
	bool debug;
	float gravity = 7.0f;
	float restitution = 2.5f;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
	b2Body* selected_body = nullptr;

	PhysBody* flipperLeft;
	PhysBody* flipperRight;
	PhysBody* flipperLeftPoint;
	PhysBody* flipperRightPoint;

	PhysBody* flipperLeft2;
	PhysBody* flipperRight2;
	PhysBody* flipperLeftPoint2;
	PhysBody* flipperRightPoint2;

	PhysBody* SpecialBumper;

	PhysBody* daBall;
	int flipperforce;
	b2RevoluteJoint* joint_rightFlipper;
	b2RevoluteJoint* joint_leftFlipper;

	b2RevoluteJoint* joint_rightFlipper2;
	b2RevoluteJoint* joint_leftFlipper2;

	PhysBody* springUp;
	PhysBody* springDown;
	int springForce;
};