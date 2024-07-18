#include "SceneGenerator.h"

void SceneGenerator::generateScene()
{
	isGeneratingScene = true;

	srand(time(NULL));
	SceneTypes sceneType = static_cast<SceneTypes>(rand() % END);

	if (sceneType == Plain||true) {
		for (int i = 0; i < (10 + rand() % 5); i++) {
			gameObjects.Enemies.push_back(new Enemy(50,  fmod(rand(), (screenWidth * 0.8)) + screenWidth * 0.1, baseLine-20, Textures["zombie"]));
		}
	}
	else if (sceneType == Normal) {

	}

	gameObjects.id = gameObjects.id + 1;

	isGeneratingScene = false;
}

SceneGenerator::SceneGenerator(GameObjects& gameObjects,int baseLine, float screenWidth, float screenHeight):gameObjects(gameObjects)
{
	this->baseLine = baseLine;
	this -> screenWidth = screenWidth;
	this -> screenHeight = screenHeight;


	generatorThread = nullptr;
}

void SceneGenerator::setTextures(unordered_map<string, unordered_map<string, vector<Texture*>>>& textures)
{
	this->Textures = textures;
}

void SceneGenerator::generateNextScene()
{
	if (isGeneratingScene) return;
	delete generatorThread;
	generatorThread = new Thread(&SceneGenerator::generateScene, this);
	generatorThread->launch();
}


