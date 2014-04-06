#include "start_layer.h"
#include "background_layer.h"
#include "game_layer.h"
using namespace std;

Scene* StartLayer::createScene()
{
	Scene *s = Scene::create();
	auto *l = StartLayer::create();
	s->addChild(l);
	return s;
}

static string LOADIMAGE[] = {
	"game_loading1.png",
	"game_loading2.png",
	"game_loading3.png",
	"game_loading4.png",
};

StartLayer::~StartLayer()
{
}

bool StartLayer::init()
{
	bool ret = false;
	do {
		CC_BREAK_IF(!Layer::init());

		Size s = Director::getInstance()->getWinSize();

		SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
		frameCache->addSpriteFramesWithFile("ui/shoot_background.plist", "ui/shoot_background.png");
		frameCache->addSpriteFramesWithFile("ui/shoot.plist");

		Sprite *backGround = Sprite::createWithSpriteFrameName("background.png");
		backGround->setPosition(Point(s.width * 0.5, s.height * 0.5));
		addChild(backGround, -1, 0);

		Sprite *title = Sprite::createWithSpriteFrameName("shoot_copyright.png");
		title->setPosition(Point(s.width * 0.5, s.height * 2 / 3));
		addChild(title, 0, 1);

		Sprite *gameLoad = Sprite::createWithSpriteFrameName(LOADIMAGE[0].c_str());
		gameLoad->setPosition(Point(s.width * 0.5, s.height * 0.5));
		addChild(gameLoad, 0, 2);

		Animation* animation = Animation::create();
		animation->setDelayPerUnit(0.4f);
		animation->addSpriteFrame(frameCache->getSpriteFrameByName(LOADIMAGE[0].c_str()));
		animation->addSpriteFrame(frameCache->getSpriteFrameByName(LOADIMAGE[1].c_str()));
		animation->addSpriteFrame(frameCache->getSpriteFrameByName(LOADIMAGE[2].c_str()));
		animation->addSpriteFrame(frameCache->getSpriteFrameByName(LOADIMAGE[3].c_str()));
		Animate* animate = Animate::create(animation);
		gameLoad->runAction(RepeatForever::create(animate));

		auto touchListener = EventListenerTouchOneByOne::create();
		touchListener->setSwallowTouches(true);
		touchListener->onTouchEnded = [](Touch *touch, Event *event) {
			Scene *st = Scene::create();
			BackgroundLayer *back = BackgroundLayer::create();
			st->addChild(back);
            GameLayer *gameLayer = GameLayer::create();
            st->addChild(gameLayer);

			Director::getInstance()->replaceScene(st);
			return;
		};
		touchListener->onTouchMoved = [](Touch* touch, Event* event){
			return;
		};

		touchListener->onTouchBegan = [](Touch *touch, Event *event) {
			return true;
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
		ret = true;
	} while (0);
	return ret;
}

