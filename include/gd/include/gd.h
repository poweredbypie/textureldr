#ifndef __GD_H__
#define __GD_H__

#include <cocos2d.h>

namespace gd {
	inline auto base = reinterpret_cast<char*>(GetModuleHandle(0));

	inline bool init() {
		//if the base address is valid, all other values should be valid.
		return base;
	}
}

#include "GDProtocols.h"

#include "layers_scenes_transitions_nodes/FLAlertLayer.h"
#include "layers_scenes_transitions_nodes/GJDropDownLayer.h"
#include "layers_scenes_transitions_nodes/GJListLayer.h"
#include "layers_scenes_transitions_nodes/LoadingLayer.h"
#include "layers_scenes_transitions_nodes/MenuLayer.h"
#include "layers_scenes_transitions_nodes/ScrollingLayer.h"

#include "manager_nodes/GManager.h"
#include "manager_nodes/GameManager.h"
#include "manager_nodes/GameSoundManager.h"

#include "menu_nodes/CCMenuItemSpriteExtra.h"
#include "menu_nodes/CCMenuItemToggler.h"

#include "audio_nodes/FMODAudioEngine.h"

#include "sprite_nodes/ButtonSprite.h"

#endif