#include "Entity.h"
#include "Game.h"

#define DEBUG true

Entity::Entity()
{
	currentSprite = nullptr;
	spriteVector = nullptr;
	colliderBox = nullptr;
	entityPtr = nullptr;
	spriteSheetPtr = nullptr;
	flipType = SDL_FLIP_NONE;
	//centre = nullptr;
}

Entity::~Entity()
{
	//delete collider;
}

Entity::Entity(std::string const& name, SpriteSheet* spriteSheet, std::string const& spriteName, int x, int y, SDL_Renderer* renderer, std::unordered_map<std::string, Entity>* entities) : GameObject(name, x, y, renderer)
{
	entityPtr = entities;

	velocity << 0.f, 0.f;
	acceleration << 0.1f, 0.f;

	idleSprite = spriteName;

	spriteSheetPtr = spriteSheet;
	spriteVector = spriteSheetPtr->operator[](idleSprite);

	numberOfFrames = spriteVector->size();

	//std::cout << numberOfFrames << "\n";

	currentSprite = spriteVector->at(currentSpriteIndex);

	worldSpacePosition = new SDL_Rect({ x,y, currentSprite->w, currentSprite->h });

	collidable = true;

	if (collidable) {
		colliderBox = new SDL_Rect({ x,y, worldSpacePosition->w, worldSpacePosition->h });
	}

}

Entity::Entity(std::string const& name, SpriteSheet* spriteSheet, int const spriteIndex, int x, int y, SDL_Renderer* renderer, std::unordered_map<std::string, Entity>* entities) : GameObject(name, x, y, renderer)
{
	entityPtr = entities;

	velocity << 0.f, 0.f;
	acceleration << 0.1f, 0.f;

	spriteSheetPtr = spriteSheet;
	spriteVector = new std::vector<Sprite*>();

	//std::cout << spriteSheetPtr->sprites.size() << "\n";
	spriteVector->push_back(spriteSheetPtr->operator[](spriteIndex));

	numberOfFrames = spriteVector->size();

	//std::cout << numberOfFrames << "\n";

	currentSprite = spriteVector->at(currentSpriteIndex);

	worldSpacePosition = new SDL_Rect({ x,y, currentSprite->w, currentSprite->h });

	collidable = true;

	if (collidable) {
		colliderBox = new SDL_Rect({ x,y, worldSpacePosition->w, worldSpacePosition->h });
	}
}

std::pair<int, int> Entity::getSize()
{
	return std::pair<int, int>{worldSpacePosition->w, worldSpacePosition->h};
}


void Entity::nextFrame()
{
	//std::printf("%f\n", velocity.y());
	if (velocity.x() != 0 /*|| velocity_y != 0*/) {
		//std::printf("%f\n", velocity.x());
		spriteVector = spriteSheetPtr->operator[](runSprite);
		numberOfFrames = spriteVector->size();
	}
	else if (!onGround && velocity.y() >= 1.f /*|| velocity_y != 0*/) {
		/*std::printf("%f\n", velocity.y());*/
		spriteVector = spriteSheetPtr->operator[](jumpSprite);
		numberOfFrames = spriteVector->size();
	}
	else {
		spriteVector = spriteSheetPtr->operator[](idleSprite);
		numberOfFrames = spriteVector->size();
	}
	if (currentSpriteIndex < numberOfFrames * (numberOfFrames * animationSlowdown) - 1)
		//if (currentSpriteIndex < numberOfFrames - 1)
		currentSpriteIndex++;
	else
		currentSpriteIndex = 0;

	currentSprite = spriteVector->at(currentSpriteIndex / (numberOfFrames * animationSlowdown));
	worldSpacePosition->w = currentSprite->w;
	worldSpacePosition->h = currentSprite->h;

	colliderBox->w = worldSpacePosition->w;
	colliderBox->h = worldSpacePosition->h;
	//currentSprite = spriteVector->at(currentSpriteIndex);

}

void Entity::setAnimationSlowdown(int slowdown)
{
	animationSlowdown = slowdown;
}

void Entity::setPlayerControlled(bool c)
{
	playerControlled = c;
}

void Entity::updateVelocity()
{
	if (playerControlled) {
		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_UP:
				if (onGround) {
					onGround = false;
					velocity.y() -= 3.f;
				}
				//else {
				//	velocity.y() = 0.f;
				//}
					
				break;
			case SDLK_DOWN:
				//if (velocity_y < 1) {
				//	velocity_y += accelerate;
				//}
				//velocity.y() = 1.0f;
				break;
			case SDLK_LEFT:
				//colliderBox->y = worldSpacePosition->y - 1;
				if (onGround && velocity.x() > -1.0f) {
					velocity.x() -= acceleration.x();
				}
				flipType = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_RIGHT:
				//colliderBox->y = worldSpacePosition->y - 1;
				if (onGround && velocity.x() < 1.0f) {
					velocity.x() += acceleration.x();
				}
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
				//velocity.y() = 0.0f;
				break;
			case SDLK_DOWN:
				//velocity.y() = 0.0f;
				break;
			case SDLK_LEFT:
				//colliderBox->y = worldSpacePosition->y + 1;
				if (Game::almostEquals(velocity.x(), 0.0f)) {
					velocity.x() = 0.0f;
				}
				if (onGround && velocity.x() > 0.0f) {
					velocity.x() -= acceleration.x();
				}
				if (onGround && velocity.x() < 0.0f) {
					velocity.x() += acceleration.x();
				}
				break;
			case SDLK_RIGHT:
				//colliderBox->y = worldSpacePosition->y + 1;
				if (Game::almostEquals(velocity.x(), 0.0f)) {
					velocity.x() = 0.0f;
				}
				if (onGround && velocity.x() > 0.0f) {
					velocity.x() -= acceleration.x();
				}
				if (onGround && velocity.x() < 0.0f) {
					velocity.x() += acceleration.x();
				}
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

	if (!fixedInPlace) {
		acceleration.y() = Game::accelerationFromGravity(acceleration.y(), velocity.y(), onGround);
	}
	if (onGround) {
		velocity.y() = 0.f;
	}
}

void Entity::move(float timeStep)
{
	//printf("%d, %d\n", velocity_x, velocity_y);
	worldSpacePosition->x += int (velocity.x() * timeStep);

	if (!onGround) {
		velocity.y() += acceleration.y();
	}

	std::cout << velocity.y() * timeStep << "\n";
	worldSpacePosition->y += int (velocity.y() * timeStep);

	colliderBox->x = worldSpacePosition->x;
	colliderBox->y = worldSpacePosition->y;

	//onGround = false;

	//if (onGround) {
	//	colliderBox->y = worldSpacePosition->y + 1;
	//}

	//onGround = false;

	std::vector<int> collisions = checkCollisions(entityPtr);

	if (playerControlled && !collisions.empty()) {
	//if (playerControlled && (collisions[0] || collisions[1] || collisions[2] || collisions[3])) {
		std::cout << collisions[0] << ", " << collisions[1] << ", " << collisions[2] << ", " << collisions[3] << "\n";

		if (/*!onGround && */collisions[3]) {
			//worldSpacePosition->y -= (velocity.y() * timeStep) + collisions[3];
			worldSpacePosition->y = collisions[3] - colliderBox->h;
			velocity.y() = 0.f;
			acceleration.y() = 0.f;
			onGround = true;
		}
		else if (collisions[2]) {
			//worldSpacePosition->y += (velocity.y() * timeStep) - collisions[2];
			worldSpacePosition->y = collisions[2];
			velocity.y() = 0.f;
			acceleration.y() = 0.f;
		}

		else if (collisions[0]) {
			//worldSpacePosition->x += (velocity.x() * timeStep) - collisions[0];
			worldSpacePosition->x = collisions[0];
			velocity.x() = 0.f;
			//acceleration.x() = 0.f;
		}
		else if (collisions[1]) {
			//worldSpacePosition->x -= (velocity.x() * timeStep) - collisions[1];
			worldSpacePosition->x = collisions[1] - colliderBox->w;
			velocity.x() = 0.f;
			//acceleration.x() = 0.f;
		}

		//if (onGround)
		//	onGround = !(collisions[0] == 0 && collisions[1] == 0);
		
		colliderBox->x = worldSpacePosition->x;
		colliderBox->y = worldSpacePosition->y;
		
	}
}

void Entity::setIdleSprite(std::string name)
{
	idleSprite = name;
}

void Entity::setRunSprite(std::string name)
{
	runSprite = name;
}

void Entity::setJumpSprite(std::string name)
{
	jumpSprite = name;
}

void Entity::setAttackSprite(std::string name)
{
	attackSprite = name;
}

void Entity::render(GameObject& camera)
{
	SDL_Rect cameraSpacePosition = { worldSpacePosition->x - camera.worldSpacePosition->x, worldSpacePosition->y - camera.worldSpacePosition->y, worldSpacePosition->w, worldSpacePosition->h };

	if (DEBUG && collidable) {
		SDL_Rect cameraSpacePosition = { colliderBox->x - camera.worldSpacePosition->x, colliderBox->y - camera.worldSpacePosition->y, colliderBox->w, colliderBox->h };
		SDL_SetRenderDrawColor(rendererPtr, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderDrawRect(rendererPtr, &cameraSpacePosition);
		SDL_SetRenderDrawColor(rendererPtr, 0x00, 0x00, 0x00, 0xFF);
	}

	SDL_RenderCopyEx(rendererPtr, gameObjectTexture->get(), currentSprite, &cameraSpacePosition, rotationDegrees, NULL, flipType);
}

std::vector<int> Entity::checkCollisions(std::unordered_map<std::string, Entity>* objectMap)
{
	for (auto& gameObject : *objectMap) {
		if (objectName != gameObject.second.objectName && gameObject.second.collidable) {
			if (SDL_HasIntersection(colliderBox, gameObject.second.colliderBox)) {
				std::vector<int> ret = { 0,0,0,0 };
				if (colliderBox->x > gameObject.second.colliderBox->x && colliderBox->x < gameObject.second.colliderBox->x + gameObject.second.colliderBox->w) {
					//ret[0] = (gameObject.second.colliderBox->x + gameObject.second.colliderBox->w) - colliderBox->x;
					ret[0] = (gameObject.second.colliderBox->x + gameObject.second.colliderBox->w);
				}
				if (colliderBox->x + colliderBox->w > gameObject.second.colliderBox->x && colliderBox->x + colliderBox->w < gameObject.second.colliderBox->x + gameObject.second.colliderBox->w) {
					//ret[1] = (colliderBox->x + colliderBox->w) - gameObject.second.colliderBox->x;
					ret[1] = gameObject.second.colliderBox->x;
				}
				if (colliderBox->y > gameObject.second.colliderBox->y && colliderBox->y < gameObject.second.colliderBox->y + gameObject.second.colliderBox->h) {
					//ret[2] = (gameObject.second.colliderBox->y + gameObject.second.colliderBox->h) - colliderBox->y;
					ret[2] = (gameObject.second.colliderBox->y + gameObject.second.colliderBox->h);
				}
				if (colliderBox->y + colliderBox->h > gameObject.second.colliderBox->y && colliderBox->y + colliderBox->h < gameObject.second.colliderBox->y + gameObject.second.colliderBox->h) {
					//ret[3] = (colliderBox->y + colliderBox->h) - gameObject.second.colliderBox->y;
					ret[3] = gameObject.second.colliderBox->y;
				}

				//std::cout << ret[0] << ", " << ret[1] << ", " << ret[2] << ", " << ret[3] << "\n";
				//if (ret[0] || ret[1] || ret[2] || ret[3])
				return ret;
			}
		}
	}
	return {};
}