#pragma once
#include "../Exports/GLib/GLib.h"
#include "SmartPointer.h"
#include <vector>
#include "GameObject.h"
namespace RenderingSystem {
	void RenderSprites(std::vector<SmartPointer<GameObject>> renderList);
	GLib::Sprites::Sprite * CreateSprite(const char * i_pFilename);
}


