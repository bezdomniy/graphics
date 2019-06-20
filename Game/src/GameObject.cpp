#include "GameObject.h"
#include "Game.h"

GameObject::GameObject()
{
	currentSprite = nullptr;
	colliderBox = nullptr;
	gameObjectsPtr = nullptr;
	flipType = SDL_FLIP_NONE;
	//centre = nullptr;
}

//GameObject::GameObject(SDL_Rect* initialRect, int frames)
GameObject::GameObject(std::string const& name, SpriteSheet & spriteSheet, std::string const& spriteName, int x, int y, std::unordered_map<std::string, GameObject>* gameObjects)
{
	objectName = name;
	gameObjectsPtr = gameObjects;
	position_x = x;
	position_y = y;
	flipType = SDL_FLIP_NONE;
	currentSprite = spriteSheet[spriteName].first;
	colliderBox = new SDL_Rect({ position_x - 1, position_y - 1, currentSprite->w + 2, currentSprite->h + 2 });

	int frames = spriteSheet[spriteName].second;
	sprites.push_back(currentSprite);
	int width = currentSprite->w;
	for (int i = 1; i < frames; i++) {
		SDL_Rect* nextRect = new SDL_Rect({ currentSprite->x + i * currentSprite->w, currentSprite->y, currentSprite->w, currentSprite->h });
		sprites.push_back(nextRect);
	}

	numberOfFrames = frames;
}

GameObject::~GameObject()
{
	//destroy();
}

void GameObject::destroy()
{
	delete colliderBox;
	//colliderBox = nullptr;
}

std::pair<int, int> GameObject::getSize()
{
	return std::pair<int, int>{currentSprite->w, currentSprite->h};
}

void GameObject::nextFrame()
{
	if (currentSpriteIndex < numberOfFrames*(numberOfFrames * animationSlowdown) - 1)
		currentSpriteIndex++;		
	else
		currentSpriteIndex = 0;

	currentSprite = sprites.at(currentSpriteIndex/(numberOfFrames * animationSlowdown));

	updateVelocity();
	move();
}

void GameObject::setAnimationSlowdown(int slowdown)
{
	animationSlowdown = slowdown;
}

void GameObject::setPlayerControlled(bool c)
{
	playerControlled = c;
}

void GameObject::updateVelocity()
{
	//const short int BUMPBACK = 1;
	if (playerControlled) {
		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_UP:
				velocity_y = -1;
				break;
			case SDLK_DOWN:
				velocity_y = 1;
				break;
			case SDLK_LEFT:
				velocity_x = -1;
				flipType = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_RIGHT:
				velocity_x = 1;
				flipType = SDL_FLIP_NONE;
				break;
			case SDLK_z:
				rotationDegrees -= 60;
				break;
			case SDLK_x:
				rotationDegrees += 60;
				break;
			default:
				break;
			}
			//playerControlled = true;
		}

		if (Game::event.type == SDL_KEYUP) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_UP:
				velocity_y = 0;
				break;
			case SDLK_DOWN:
				velocity_y = 0;
				break;
			case SDLK_LEFT:
				velocity_x = 0;
				break;
			case SDLK_RIGHT:
				velocity_x = 0; 
				break;
			default:
				break;
			//case SDLK_z:
			//	rotationDegrees -= 60;
			//case SDLK_x:
			//	rotationDegrees += 60;
			}
			//playerControlled = true;
		}
	}
}

void GameObject::move()
{
	//printf("%d, %d\n", velocity_x, velocity_y);
	position_x += velocity_x;
	position_y += velocity_y;

	colliderBox->x = position_x;
	colliderBox->y = position_y;

	const short int BUMPBACK = 1;

	if (playerControlled && checkCollisions(gameObjectsPtr)) {
		//playerControlled = false;
		position_x -= velocity_x;
		position_y -= velocity_y;

		colliderBox->x = position_x;
		colliderBox->y = position_y;
	}
}

bool GameObject::checkCollisions(std::unordered_map<std::string, GameObject>* objectMap)
{
	for (auto& object : *objectMap) {
		if (objectName != object.second.objectName) {
			if (SDL_HasIntersection(colliderBox, object.second.colliderBox)) {
				return true;
			}
		}
	}
}


