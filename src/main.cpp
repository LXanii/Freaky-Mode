#include <Geode/Geode.hpp>
#include <Geode/modify/MoreOptionsLayer.hpp>
#include <Geode/modify/GameLevelOptionsLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/ShaderLayer.hpp>

#include "ShaderManager.h"

using namespace geode::prelude;

bool IsFreaky = false;

class freaky {
	public:
		void getting_freaky(CCObject*);
};

void freaky::getting_freaky(CCObject*) {
		CCScene* Scene = CCDirector::sharedDirector()->getRunningScene();
		CCArray* children = Scene->getChildren();
		IsFreaky = !IsFreaky;
		ShaderManager::get()->isActive = IsFreaky;
	}

class $modify(MoreOptionsLayer) {

	struct Fields {
		CCSprite * FREAKY;
		bool freaked = true;
	};

	bool init() {
		bool result = MoreOptionsLayer::init();
		m_fields->FREAKY = CCSprite::create("FREAKY.png"_spr);
		m_fields->FREAKY->setPosition({226,42});
		m_fields->FREAKY->setScale(0.85);

		CCLayer* layer = static_cast<CCLayer*>(getChildren()->objectAtIndex(0));
		CCLayer* layer2 = static_cast<CCLayer*>(layer->getChildren()->objectAtIndex(6));

		layer2->addChild(m_fields->FREAKY);

		CCMenu* freak_menu = CCMenu::create();

		CCMenuItemToggler* freak_btn = CCMenuItemToggler::create(CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"), CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), this, menu_selector(freaky::getting_freaky));
		freak_btn->setScale(0.82);
		freak_btn->toggle(IsFreaky);
		freak_menu->setPosition({125,46.8});
		freak_menu->addChild(freak_btn);
		layer2->addChild(freak_menu);
		handleTouchPriority(this);
		return result;
	}
};

class $modify(GameLevelOptionsLayer) {

	struct Fields {
		CCSprite* FREAKY;
	};

	void setupOptions() {
		GameLevelOptionsLayer::setupOptions();
		m_fields->FREAKY = CCSprite::create("FREAKY.png"_spr);
		m_fields->FREAKY->setPosition({226,178});
		m_fields->FREAKY->setScale(0.85);

		CCMenu* freak_menu = CCMenu::create();

		CCMenuItemToggler* freak_btn = CCMenuItemToggler::create(CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"), CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), this, menu_selector(freaky::getting_freaky));
		freak_btn->setScale(0.82);
		freak_btn->toggle(IsFreaky);
		freak_menu->setPosition({125,182.8});
		freak_menu->addChild(freak_btn);
		addChild(m_fields->FREAKY);
		addChild(freak_menu);
		handleTouchPriority(this);
	}
};