#ifndef __GD_H__
#define __GD_H__

#include <cocos2d.h>

#include "gdMacros.h"

namespace gd {
	inline auto base = reinterpret_cast<char*>(GetModuleHandle(0));

	inline bool init() {
		//if the base address is valid, all other values should be valid.
		#if (__WARN_DEBUG__ == true)
		static_assert(sizeof(std::string) == 24,
			"Any classes containing member strings or using strings in functions will break!\n"
			"");
		#endif
		return base;
	}
}

#include "GDProtocols.h"

#include "actions/CCCircleWave.h"

#include "layers_scenes_transitions_nodes/FLAlertLayer.h"
#include "layers_scenes_transitions_nodes/GJDropDownLayer.h"
#include "layers_scenes_transitions_nodes/GJListLayer.h"
#include "layers_scenes_transitions_nodes/LoadingLayer.h"
#include "layers_scenes_transitions_nodes/MenuLayer.h"
#include "layers_scenes_transitions_nodes/ScrollingLayer.h"
#include "layers_scenes_transitions_nodes/LoadingCircle.h"
#include "layers_scenes_transitions_nodes/CCBlockLayer.h"
#include "layers_scenes_transitions_nodes/PauseLayer.h"
#include "layers_scenes_transitions_nodes/GJBaseGameLayer.h"
#include "layers_scenes_transitions_nodes/PlayLayer.h"

#include "manager_nodes/GManager.h"
#include "manager_nodes/GameManager.h"
#include "manager_nodes/GameSoundManager.h"

#include "menu_nodes/CCMenuItemSpriteExtra.h"
#include "menu_nodes/CCMenuItemToggler.h"

#include "audio_nodes/FMODAudioEngine.h"

#include "sprite_nodes/ButtonSprite.h"
#include "sprite_nodes/GameObject.h"
#include "sprite_nodes/CCAnimatedSprite.h"
#include "sprite_nodes/AnimatedShopKeeper.h"

#include "text_input_nodes/CCTextInputNode.h"

#endif