#pragma once

#include <stdlib.h>
#include <time.h>
#include <vector>

#include <SFML/System/Thread.hpp>

#include "Enemy.h"
#include "Platform.h"
#include "Player.h"
#include "Projectile.h"

using namespace std;

enum SceneTypes {
	Normal,
	Parkour,
	ParkourWithZombies,
	Plain,
	BossArena,
	END
};

typedef struct GameObjects {
	vector<Enemy*> Enemies;
	vector<Platform> Platforms;
	vector<Projectile*> Projectiles;
	int id=0;
} GameObjects;
		
class SceneGenerator
{
private :

	Thread* generatorThread;
	unordered_map<string, unordered_map<string, vector<Texture*>>> Textures;

	int baseLine;
	float screenWidth;
	float screenHeight;

	void generateScene();
public :
	GameObjects& gameObjects;
	bool isGeneratingScene;
	SceneGenerator(GameObjects& gameObjects, int baseLine, float screenWidth, float screenHeight);
	void setTextures(unordered_map<string, unordered_map<string, vector<Texture*>>>& textures);
	void generateNextScene();
};

