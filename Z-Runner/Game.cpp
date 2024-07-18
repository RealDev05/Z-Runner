#include "Game.h"
#include <iostream>
#include <filesystem>

void Game::initializeVariables()
{
	this->window = nullptr;

    //Loading texture
    path ResFolder = current_path() / "res\\textures\\sprites";

    for (const auto& character : directory_iterator(ResFolder)) {
        unordered_map<string, vector<Texture*>>* textureGroup = new unordered_map<string, vector<Texture*>>();
        for (const auto& animation : directory_iterator(character.path())) {
            vector<Texture*>* textures = new vector<Texture*>();
            for (const auto& file : directory_iterator(animation.path())) {
                Texture* texture = new Texture();
                texture->loadFromFile("res\\textures\\sprites\\"+relative(file,ResFolder).string());
                textures->push_back(texture);
            }
            (*textureGroup)[animation.path().filename().string()] = *textures;
        }
        Textures[character.path().filename().string()] = *textureGroup;
    }
}

void Game::initializeWindow()
{
	videoMode.width = SCREEN_WIDTH;//1920
	videoMode.height = SCREEN_HEIGHT;//1080

	window=new RenderWindow(videoMode, "Z-Runner", Style::Titlebar | Style::Close);

    window->setFramerateLimit(30);

    window->setKeyRepeatEnabled(false);

    deltaTime = clock.restart();
}

Game::Game() : mainCamera(FloatRect(0.f, SCREEN_HEIGHT-VIEW_HEIGHT, VIEW_WIDTH, VIEW_HEIGHT)),sceneGenerator(nextSceneObjects, SCREEN_HEIGHT * 0.9f,SCREEN_WIDTH,SCREEN_HEIGHT), grid(SCREEN_WIDTH, vector<PathFindingNode>(SCREEN_HEIGHT, PathFindingNode(0, 0, true))), pathFinder(SCREEN_WIDTH, SCREEN_HEIGHT)
{
	this->initializeVariables();
	this->initializeWindow();

    ground.initialize(SCREEN_WIDTH, SCREEN_HEIGHT * 0.1f,0,SCREEN_HEIGHT*0.9f, Color(99, 86, 49,0),true);
    player.initialize(SCREEN_WIDTH/56, SCREEN_WIDTH / 56, SCREEN_WIDTH / 37, ground.getPosition().y - SCREEN_WIDTH / 56, Textures["player"]);

    Platform safeZoneBarrier;
    safeZoneBarrier.initialize(200, 100, VIEW_WIDTH - 200, ground.getPosition().y - 100, Color::Red, false);

    window->setView(mainCamera);

    gameObjects.Platforms.push_back(safeZoneBarrier);
    gameObjects.Enemies.push_back(new Enemy( 50, VIEW_WIDTH - 300, ground.getPosition().y - 150, Textures["zombie"]));

    
    /*for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            grid[x][y] = PathFindingNode(x, y, true); // Initialize each node as walkable
        }
    }

    // Adjust walkable nodes based on intersections with platforms and obstacles
    for (auto& row : grid) {
        for (auto& node : row) {
            sf::FloatRect nodeRect(static_cast<float>(node.x), static_cast<float>(node.y), 1.0f, 1.0f); // Create SFML FloatRect for current node

            // Check intersections with platforms
            for (Platform& platform : gameObjects.Platforms) {
                if (platform.getObject().getGlobalBounds().intersects(nodeRect)) {
                    node.walkable = false;
                    break; // No need to check other platforms if already not walkable
                }
            }

            // Check intersections with obstacles (assuming Obstacle is similar to Platform)
            if (!node.walkable) continue; // Skip obstacle check if node is already not walkable
            for (Obstacle& obstacle : gameObjects.Obstacles) {
                if (obstacle.getObject().getGlobalBounds().intersects(nodeRect)) {
                    node.walkable = false;
                    break; // No need to check other obstacles if already not walkable
                }
            }
        }
    }

    pathFinder.setGrid(grid);*/


    sceneGenerator.setTextures(Textures);
    sceneGenerator.generateNextScene();
}

Game::~Game()
{
	delete window;
}

void Game::eventHandler()
{
    while (window->pollEvent(event)) {
        switch (event.type) {

        case Event::Closed:
            window->close();
            break;
        case Event::KeyPressed:
            switch (event.key.code) {
            case Keyboard::Escape:
                window->close();
                break;
            case Keyboard::A:
                pressedKey = event.key.code;
                break;
            case Keyboard::D:
                pressedKey = event.key.code;
                break;
            case Keyboard::Space:
                if (!player.isFalling) {
                    player.addVelocity(Vector2f(0,-JUMP_HEIGHT));
                }
            }
            break;
        case Event::KeyReleased:
            switch (event.key.code) {
            case Keyboard::A:
                pressedKey = -1;
                break;
            case Keyboard::D:
                pressedKey = -1;
                break;
            }
            break;

        }
    }

}

void Game::update()
{
    vector<RectangleShape> objects;

    Vector2f mousePos = window->mapPixelToCoords(Mouse::getPosition(*window));
    deltaTime = clock.restart();

    eventHandler();

    if (pressedKey == Keyboard::A) {
        player.addVelocity(Vector2f(-MOVEMENT_SPEED, 0));
    }
    else if (pressedKey==Keyboard::D) {
        player.addVelocity(Vector2f(MOVEMENT_SPEED, 0));

   }

    //Player on ground check
    player.isFalling = player.isCollidingWith(ground.getObject(),true)!=0;
    for (Platform platform : gameObjects.Platforms) {
        int collidingSide = player.isCollidingWith(platform.getObject(),true);
        objects.push_back(platform.getObject());

        player.isFalling = player.isFalling && collidingSide!=0;

        if (collidingSide==0) continue;

        if (collidingSide == 1) {
            player.setVelocity(Vector2f(player.getVelocity().x < 0 ? 0 : player.getVelocity().x, player.getVelocity().y));
        }
        else if (collidingSide == 3) {
            player.setVelocity(Vector2f(player.getVelocity().x > 0 ? 0 : player.getVelocity().x, player.getVelocity().y));
        }
        else if (collidingSide == 2 && !platform.ispassThrough) {
            player.setVelocity(Vector2f(player.getVelocity().x, player.getVelocity().y < 0 ? 0 : player.getVelocity().y));
        }

    }

    if (!player.isFalling && player.getVelocity().y > 0) {
        player.setVelocity(Vector2f(player.getVelocity().x, 0));
    }

    //gravity
    if (player.isFalling) {
        player.addVelocity(Vector2f(0, GRAVITY)*deltaTime.asSeconds());
    }

    //friction
    if (player.getVelocity().x != 0) {
        float friction = -player.getVelocity().x*0.4;
        friction = abs(friction) < 0.1 ? -player.getVelocity().x: friction;
        player.addVelocity(Vector2f(friction, 0));
    }

    //Player shooting
    if (player.canAttack && !player.died) {
        if (gameObjects.Projectiles.size() > 10) {
            Projectile* t;
            t = gameObjects.Projectiles.back();
            gameObjects.Projectiles.pop_back();
            delete t;
        }

        gameObjects.Projectiles.insert(gameObjects.Projectiles.begin(), player.shootAt(mousePos));
    }

    objects.push_back(ground.getObject());

    //Updating projectiles
    for (Projectile* p : gameObjects.Projectiles) {
        if (p->destroyed) {
            continue;
        }
        
        p->update(deltaTime.asSeconds(),objects);
    }

    //Updating Enemies
    for (Enemy* e : gameObjects.Enemies) {
        if (e->died) {
            continue;
        }

        if (e->getObject().getGlobalBounds().intersects(player.getObject().getGlobalBounds())) {
            player.receiveDamage(e->attackPlayer());
        }

        e->canSeePlayer = FloatRect(mainCamera.getCenter() - mainCamera.getSize() / 2.f, mainCamera.getSize()).intersects(e->getObject().getGlobalBounds());

        e->update(deltaTime.asSeconds(),gameObjects.Platforms,gameObjects.Projectiles,player.getObject().getGlobalBounds(),ground.getObject().getGlobalBounds(),pathFinder);

    }
    //Player died
    if (player.died) {
        player.setVelocity(Vector2f(0, player.getVelocity().y));
        //player.setColor(Color::White);
    }

    player.updatePosition(player.getPosition() + player.getVelocity()*deltaTime.asSeconds());

    //camera movements
    if (player.getPosition().x >= mainCamera.getCenter().x && player.getVelocity().x > 0) {
        if (mainCamera.getCenter().x < SCREEN_WIDTH - VIEW_WIDTH / 2) {
            mainCamera.move(Vector2f(player.getVelocity().x * deltaTime.asSeconds(), 0));
        }
    }
    else if (player.getPosition().x <= mainCamera.getCenter().x && player.getVelocity().x < 0) {
        if (mainCamera.getCenter().x > VIEW_WIDTH / 2) {
            mainCamera.move(Vector2f(player.getVelocity().x * deltaTime.asSeconds(), 0));
        }
    }

    if (player.getPosition().x >= SCREEN_WIDTH * 0.9) {
        player.updatePosition(Vector2f(SCREEN_WIDTH / 37, ground.getPosition().y - SCREEN_WIDTH / 56));
        player.setVelocity(Vector2f(0, 0));
        mainCamera.reset(FloatRect(0.f, SCREEN_HEIGHT - VIEW_HEIGHT, VIEW_WIDTH, VIEW_HEIGHT));
        if (gameObjects.id != nextSceneObjects.id) {
            gameObjects = nextSceneObjects;
            sceneGenerator.generateNextScene();
        }
        /*if (!isInSceneTransition) {
            cout << "Starting scene Generation" << endl;
            isInSceneTransition = true;
        }else if (!sceneGenerator.isGeneratingScene) {
            cout << "Finished scene Generation" << endl;
            
            isInSceneTransition = false;
        } */
    }

}

void Game::render()
{
    window->clear();
 
    window->draw(ground.getObject());
    window->draw(player.getObject());

    for (Platform p : gameObjects.Platforms) {
        window->draw(p.getObject());
    }

    for (Projectile* p : gameObjects.Projectiles) {
        if (p->destroyed) continue;
        window->draw(p->getObject());
    }
    for (Enemy* e : gameObjects.Enemies) {
        if (e->died) continue;
        window->draw(e->getObject());
    }

    window->setView(mainCamera);

    window->display();
}

bool Game::isRunning()
{
	return this->window->isOpen();
}
