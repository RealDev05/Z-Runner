#pragma once

#define SCREEN_WIDTH 1920.f
#define SCREEN_HEIGHT 1080.f

#define GRAVITY 50

#define VIEW_WIDTH 960.f
#define VIEW_HEIGHT 540.f

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <vector>
#include <unordered_map>
#include <filesystem>

#include "Enemy.h"
#include "Platform.h"
#include "Player.h"
#include "Projectile.h"

#include "SceneGenerator.h"

#include "PathFinder.h"
#include <filesystem>

using namespace sf;
using namespace std;
using namespace filesystem;

//typedef struct GameObjects {
//	vector<Enemy> Enemies;
//	vector<Obstacle> Obstacles; 
//	vector<Platform> Platforms;
//	vector<Projectile*> Projectiles;
//} GameObjects;

class Game
{

private :
	RenderWindow* window;
	VideoMode videoMode;
	Event event;
	int pressedKey = -1;
	Clock clock;
	Time deltaTime;

	Player player;
	Platform ground;
	GameObjects gameObjects;
	unordered_map<string,unordered_map<string,vector<Texture*>>> Textures;

	SceneGenerator sceneGenerator;

	bool isInSceneTransition=false;

	View mainCamera;

	void loadTexture();

	void initializeVariables();
	void initializeWindow();

	vector<vector<PathFindingNode>> grid;
	PathFinder pathFinder;
public :
	GameObjects nextSceneObjects;
	Game();
	virtual ~Game();

	void eventHandler();
	void update();
	void render();

	bool isRunning();

	friend class SceneGenerator;
};

