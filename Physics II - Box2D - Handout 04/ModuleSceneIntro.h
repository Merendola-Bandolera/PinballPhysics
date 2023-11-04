#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	p2List<PhysBody*> boxesV;
	p2List<PhysBody*> circlexup;
	p2List<PhysBody*> flipperLe;
	p2List<PhysBody*> flipperRi;
	p2List<PhysBody*> springs;

	PhysBody* sensor;
	bool sensed;


	SDL_Texture* background;
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* boxV;
	SDL_Texture* circleChup;
	SDL_Texture* spring;
	SDL_Texture* flipperL;
	SDL_Texture* flipperR;
	SDL_Texture* rick;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
