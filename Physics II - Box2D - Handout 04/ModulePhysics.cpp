#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "ModuleWindow.h"
#include "math.h"
#include "ModuleSceneIntro.h"
#include <string>
#include <random>
#include <time.h>

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
	
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	flipperforce = -4600;

	b2BodyDef groundDef;
	b2Body* groundBody = world->CreateBody(&groundDef);

	b2EdgeShape groundShape;

	App->scene_intro->flipperLe.add(flipperLeft = App->physics->CreateRectangle(230, 620, 70, 20, 0, false));
	flipperLeftPoint = App->physics->CreateCircle(230, 620, 2, 0.0f, false);
	flipperLeftPoint->body->SetType(b2_staticBody);
	

	// Flipper Joint (flipper rectangle x flipper circle to give it some movement)
	b2RevoluteJointDef flipperLeftJoint;

	flipperLeftJoint.bodyA = flipperLeft->body;
	flipperLeftJoint.bodyB = flipperLeftPoint->body;
	flipperLeftJoint.referenceAngle = 0 * DEGTORAD;
	flipperLeftJoint.enableLimit = true;
	flipperLeftJoint.lowerAngle = -30 * DEGTORAD;
	flipperLeftJoint.upperAngle = 30 * DEGTORAD;
	flipperLeftJoint.localAnchorA.Set(PIXEL_TO_METERS(-33), 0);
	flipperLeftJoint.localAnchorB.Set(0, 0);
	b2RevoluteJoint* joint_leftFlipper = (b2RevoluteJoint*)App->physics->world->CreateJoint(&flipperLeftJoint);

	// --- Right flipper ---
	App->scene_intro->flipperRi.add(flipperRight = App->physics->CreateRectangle(410, 620, 70, 20,0, false));
	flipperRightPoint = App->physics->CreateCircle(410, 620, 2, 0.0f, false);
	flipperRightPoint->body->SetType(b2_staticBody);

	// Flipper Joint
	b2RevoluteJointDef flipperRightJoint;

	flipperRightJoint.bodyA = flipperRight->body;
	flipperRightJoint.bodyB = flipperRightPoint->body;
	flipperRightJoint.referenceAngle = 0 * DEGTORAD;
	flipperRightJoint.enableLimit = true;
	flipperRightJoint.lowerAngle = -30 * DEGTORAD;
	flipperRightJoint.upperAngle = 30 * DEGTORAD;
	flipperRightJoint.localAnchorA.Set(PIXEL_TO_METERS(33), 0);
	flipperRightJoint.localAnchorB.Set(0, 0);
	b2RevoluteJoint* joint_rightFlipper = (b2RevoluteJoint*)App->physics->world->CreateJoint(&flipperRightJoint);


	App->scene_intro->flipperLe.add(flipperLeft2 = App->physics->CreateRectangle(530, 620, 70, 20,0,false));
	flipperLeftPoint2 = App->physics->CreateCircle(530, 620, 2, 0.0f, false);
	flipperLeftPoint2->body->SetType(b2_staticBody);


	// Flipper Joint (flipper rectangle x flipper circle to give it some movement)
	b2RevoluteJointDef flipperLeftJoint2;

	flipperLeftJoint2.bodyA = flipperLeft2->body;
	flipperLeftJoint2.bodyB = flipperLeftPoint2->body;
	flipperLeftJoint2.referenceAngle = 0 * DEGTORAD;
	flipperLeftJoint2.enableLimit = true;
	flipperLeftJoint2.lowerAngle = -30 * DEGTORAD;
	flipperLeftJoint2.upperAngle = 30 * DEGTORAD;
	flipperLeftJoint2.localAnchorA.Set(PIXEL_TO_METERS(-33), 0);
	flipperLeftJoint2.localAnchorB.Set(0, 0);
	b2RevoluteJoint* joint_leftFlipper2 = (b2RevoluteJoint*)App->physics->world->CreateJoint(&flipperLeftJoint2);

	// --- Right flipper ---
	App->scene_intro->flipperRi.add(flipperRight2 = App->physics->CreateRectangle(710, 620, 70, 20,0,false));
	flipperRightPoint2 = App->physics->CreateCircle(710, 620, 2, 0.0f, false);
	flipperRightPoint2->body->SetType(b2_staticBody);

	// Flipper Joint
	b2RevoluteJointDef flipperRightJoint2;

	flipperRightJoint2.bodyA = flipperRight2->body;
	flipperRightJoint2.bodyB = flipperRightPoint2->body;
	flipperRightJoint2.referenceAngle = 0 * DEGTORAD;
	flipperRightJoint2.enableLimit = true;
	flipperRightJoint2.lowerAngle = -30 * DEGTORAD;
	flipperRightJoint2.upperAngle = 30 * DEGTORAD;
	flipperRightJoint2.localAnchorA.Set(PIXEL_TO_METERS(33), 0);
	flipperRightJoint2.localAnchorB.Set(0, 0);
	b2RevoluteJoint* joint_rightFlipper2 = (b2RevoluteJoint*)App->physics->world->CreateJoint(&flipperRightJoint2);









	srand(time(NULL));



	// --- Spring Physics ---
	App->scene_intro->springs.add(springUp = App->physics->CreateRectangle(980, 700, 55, 30,0,false));
	springDown = App->physics->CreateRectangle(987, 755, 55, 10,0,false);
	springDown->body->SetType(b2_staticBody);
	

	// Create a Joint to Join the top and the bot from the spring
	b2DistanceJointDef SpringJointDef;

	SpringJointDef.bodyA = springUp->body;
	SpringJointDef.bodyB = springDown->body;

	SpringJointDef.localAnchorA.Set(0, 0);
	SpringJointDef.localAnchorB.Set(0, 0);

	SpringJointDef.length = 1.5f;

	SpringJointDef.collideConnected = true;

	SpringJointDef.frequencyHz = 7.0f;
	SpringJointDef.dampingRatio = 0.05f;

	b2PrismaticJoint* SpringJoint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&SpringJointDef);

	daBall = App->physics->CreateCircle(1000, 550, 25, 0.4f, true);

	App->scene_intro->circlexup.add(daBall);
	//bumper circulo
	

	App->scene_intro->circles.add(App->physics->CreateCircle(420, 250, 20, 1.5f, false));
	App->scene_intro->circles.add(App->physics->CreateCircle(170, 500, 20, 1.5f, false));
	App->scene_intro->circles.add(App->physics->CreateCircle(780, 450, 20, 1.5f, false));
	App->scene_intro->circles.add(App->physics->CreateCircle(400, 450, 20, 1.5f, false));
	App->scene_intro->circles.add(App->physics->CreateCircle(439, 610, 20, 1.5f, false));
	App->scene_intro->circles.add(App->physics->CreateCircle(493, 610, 20, 1.5f, false));


	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for (b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if (c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if (pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, float restitution, bool physics)
{
	b2BodyDef body;
	if (physics == true)
		body.type = b2_dynamicBody;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 3.0f;
	fixture.restitution = restitution;


	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, float restitution,bool physics)
{
	b2BodyDef body;
	if (physics == false)
	body.type = b2_dynamicBody;
	else 
	body.type = b2_staticBody;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 10.0f;
	fixture.restitution = restitution;
	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);
	
	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	/*if (!debug)
		return UPDATE_CONTINUE;*/



	 char buffer[50]; 

	 sprintf_s(buffer, "Score: %d", imunderyourskin); // Format the string

	 char* myCharPointer = _strdup(buffer); // Allocate memory and copy the string

	App->window->SetTitle(myCharPointer);
	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();

				App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->GetVertexCount();
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->GetVertex(i));
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->GetVertex(0));
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				if (debug) {
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for (int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if (i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
			
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
			}
			break;
			}

			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				int mouse_x, mouse_y;
				SDL_GetMouseState(&mouse_x, &mouse_y);

				b2Vec2 mouse_position(PIXEL_TO_METERS(mouse_x), PIXEL_TO_METERS(mouse_y));
				if (b->GetFixtureList()->TestPoint(mouse_position))
				{
					// A body was selected, you can create a mouse joint here.
					selected_body = b;
				}
				// Iterate through bodies and check if they contain the mouse position

			}

			// TODO 2: If a body was selected, create a mouse joint using the "mouse_joint" class property.
			if (selected_body)
			{
				b2MouseJointDef md;
				md.bodyA = ground;
				md.bodyB = selected_body;
				md.target = selected_body->GetPosition();
				md.maxForce = 1000.0f * selected_body->GetMass();
				mouse_joint = (b2MouseJoint*)world->CreateJoint(&md);
				selected_body = nullptr; // Set the selected body to null, as we have created the joint.
			}

			// TODO 3: If the player keeps pressing the mouse button, update the target position and draw a red line between both anchor points.
			if (mouse_joint)
			{
				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
				{
					int mouse_x, mouse_y;
					SDL_GetMouseState(&mouse_x, &mouse_y);
					b2Vec2 target(PIXEL_TO_METERS(mouse_x), PIXEL_TO_METERS(mouse_y));
					mouse_joint->SetTarget(target);

					// Draw a red line between both anchor points
					App->renderer->DrawLine(METERS_TO_PIXELS(mouse_joint->GetAnchorA().x), METERS_TO_PIXELS(mouse_joint->GetAnchorA().y),
						METERS_TO_PIXELS(mouse_joint->GetAnchorB().x), METERS_TO_PIXELS(mouse_joint->GetAnchorB().y),
						255, 0, 0);
				}
			}

			// TODO 4: If the player releases the mouse button, destroy the joint.
			if (mouse_joint)
			{
				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
				{
					world->DestroyJoint(mouse_joint);
					mouse_joint = nullptr;
				}
			}
			// TODO 1: If mouse button 1 is pressed ...
			// App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
			// test if the current body contains mouse position
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		flipperLeft->body->ApplyForceToCenter(b2Vec2(0, flipperforce), 1);
		flipperLeft2->body->ApplyForceToCenter(b2Vec2(0, flipperforce), 1);

	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		flipperRight->body->ApplyForceToCenter(b2Vec2(0, flipperforce), 1);
		flipperRight2->body->ApplyForceToCenter(b2Vec2(0, flipperforce), 1);
	}

	if (daBall->body->GetPosition().y > 15)
	{
		daBall->body->SetTransform(b2Vec2(512, 650),0);
		daBall = App->physics->CreateCircle(1000, 550, 25, 0.4f, true);
		App->scene_intro->circlexup.add(daBall);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && launching == false)
	{
		launching = true;
	}
	if (launching == true)
	{
		if (springForce < 9000) {
			springForce += 300;
		}
		springUp->body->ApplyForceToCenter(b2Vec2(0, springForce), 1);
	}
	if (springForce >= rand() % 9000 + 7000) {
		springForce = -300;
		launching = false;
		
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int& y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

PhysBody* ModulePhysics::CreateSolidChain(int x, int y, int* points, int size) {
	// Create BODY at position x,y
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE
	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];
	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	shape.CreateLoop(p, size / 2);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &shape;

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Clean-up temp array
	delete p;

	// Create our custom PhysBody class
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	// Return our PhysBody class
	return pbody;
}


int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx * fx) + (fy * fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if (physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);

	if (physA == daBall || physB == daBall && physA != springUp || physB != springUp)
		imunderyourskin += 100;
	
}

