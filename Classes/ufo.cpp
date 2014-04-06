#include "ufo.h"

Ufo* Ufo::createUfo(int type)
{
    SpriteFrame *pFrame = nullptr;
    if (type == Ufo::BOMB) {
        pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ufo2.png");
    } else if (type == Ufo::BULLET) {
        pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ufo1.png");
    }
    Ufo *pobSprite = new Ufo();
    if (pFrame && pobSprite && pobSprite->initWithSpriteFrame(pFrame))
    {
        pobSprite->autorelease();
        pobSprite->setType(type);
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}
