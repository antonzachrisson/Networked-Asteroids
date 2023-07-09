#include "UFO.hpp"

UFO::UFO()
{
	position.x = static_cast <float> (rand () % 640 + 100);
	position.y = static_cast <float> (rand() % 480 + 100);
	if (!Texture.loadFromFile("assets/UFO.png"))
	{
		std::cout << "error loading file" << std::endl;
		return;
	}
	Sprite.setTexture(Texture);
	Sprite.setPosition(position);
	Sprite.setOrigin(20.0f, 10.0f);
	alive = true;
};

void UFO::Update()
{
	Sprite.setPosition(position);
}