#include "plane.h"

Plane::~Plane()
{
    CC_SAFE_RELEASE_NULL(m_destroyAction);
    CC_SAFE_RELEASE_NULL(m_bullets);
}

Plane* Plane::create()
{
    Plane *plane = new Plane();
    auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1.png");
    if (plane && plane->initWithSpriteFrame(spriteFrame)) {
        plane->autorelease();
        plane->initFlyAction();
        plane->initDestroyAction();
        plane->addTouchEvent();
        return plane;
    } else {
        CC_SAFE_RELEASE(plane);
        return nullptr;
    }
}

void Plane::initFlyAction()
{
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.1f);
    animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1.png"));
    animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));
    Animate* animate = Animate::create(animation);
    runAction(RepeatForever::create(animate));
    m_isMoving = false;
}

void planeIsDead(Node *node)
{
    node->setVisible(false);
}

void Plane::initDestroyAction()
{
    Animation *animation = Animation::create();
    auto cache = SpriteFrameCache::getInstance();
    animation->setDelayPerUnit(0.1f);
    animation->addSpriteFrame(cache->getSpriteFrameByName("hero_blowup_n1.png"));
    animation->addSpriteFrame(cache->getSpriteFrameByName("hero_blowup_n2.png"));
    animation->addSpriteFrame(cache->getSpriteFrameByName("hero_blowup_n3.png"));
    animation->addSpriteFrame(cache->getSpriteFrameByName("hero_blowup_n4.png"));
    Blink *blink = Blink::create(1,3);
    Animate *cc = Animate::create(animation);

    FiniteTimeAction *callback = CallFuncN::create(std::bind(planeIsDead, this));
    m_destroyAction = (Sequence*)Sequence::create(cc, blink, callback, NULL);
    m_destroyAction->retain();
}

void Plane::present(const Point &pos)
{
    Blink *link = Blink::create(1, 4);
    runAction(link);
    setPosition(pos);
    m_life = 1;
}

bool Plane::hitOnce()
{
    if (m_life > 0) {
        m_life--;
    }
    if (m_life == 0) {
        m_dead = true;
        runAction(m_destroyAction->clone());
        return true;
    }
    return false;
}

void Plane::addTouchEvent()
{
    EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch* touch, Event *event) {
        Plane *plane = dynamic_cast<Plane*>(event->getCurrentTarget());
        assert((plane && (plane == this)));
        Point screenPos = touch->getLocation();
        Point pos = plane->convertToNodeSpace(screenPos);
        Size s = plane->getContentSize();
        Rect rect(0, 0, s.width, s.height);
        if (rect.containsPoint(pos) && !Director::getInstance()->isPaused()) {
            this->m_isMoving = true;
            return true;
        }
        return false;
    };

    listener->onTouchMoved = [=](Touch *touch, Event *event) {
        Plane *plane = dynamic_cast<Plane*>(event->getCurrentTarget());
        assert((plane && (plane == this)));
        Point pos = touch->getLocation();
        Point pos1 = touch->getPreviousLocation();
        Point diff = pos - pos1;
        if (this->m_isMoving && !this->m_dead) {
            Point newPos = plane->getPosition() + diff;
            Size size = Director::getInstance()->getWinSize();
            Size content = plane->getContentSize();
            float x = fmax(content.width * 0.5, fmin(newPos.x, size.width - content.width * 0.5));
            float y = fmax(content.height * 0.5, fmin(newPos.y, size.height - content.height * 0.5));

            plane->setPosition(Point(x, y));
        };
    };

    listener->onTouchEnded = [=](Touch *touch, Event *event) {
        Plane *plane = dynamic_cast<Plane*>(event->getCurrentTarget());
        assert((plane && (plane == this)));
        this->m_isMoving = false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Plane::startShoot(SpriteBatchNode *nodes)
{
    nodes->retain();
    CC_SAFE_RELEASE(m_bullets);
    m_bullets = nodes;
	schedule(schedule_selector(Plane::addBullet), 0.2f);
}

void Plane::stopShoot()
{
    unschedule(schedule_selector(Plane::addBullet));
}

void bulletArrived(Node *node)
{
    node->setVisible(false);
}

Sprite* createBullet(SpriteFrame* pFrame, const Point &pos, float distance)
{
    Sprite *bullet = Sprite::createWithSpriteFrame(pFrame);
    bullet->setPosition(pos);

    float velocity = 800 / 1.0;
    float duration = distance / velocity;

    MoveTo *move = MoveTo::create(duration, Point(pos.x, pos.y + distance));
    CallFuncN *callback = CallFuncN::create(std::bind(bulletArrived, bullet));
    Sequence *seq = Sequence::create(move, callback, nullptr);
    bullet->runAction(seq);
    return bullet;
}

void Plane::addBullet(float dt)
{
	if (m_bulletType == BULLET) {
        auto pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("bullet1.png");
        Point pos(getPositionX(), getPositionY() + getContentSize().height * 0.5);
        float distance = Director::getInstance()->getWinSize().height + 100;
        Sprite *bullet = createBullet(pFrame, pos, distance);
		m_bullets->addChild(bullet);
	} else if (m_bulletType == DOUBLE_BULLET) {
        auto pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("bullet2.png");
        float sx = getPositionX(); float sy = getPositionY();
        Size content = getContentSize();

        Point pos(sx - content.width * 0.25, sy + content.height * 0.5);
        float distance = Director::getInstance()->getWinSize().height + 100;
        Sprite *bullet1 = createBullet(pFrame, pos, distance);
        m_bullets->addChild(bullet1);

        Point pos2(sx + content.width * 0.25, sy + content.height * 0.5);
        Sprite *bullet2 = createBullet(pFrame, pos2, distance);
        m_bullets->addChild(bullet2);
	}
}

void Plane::setSpecialBullet(BulletType type)
{
    if (m_bulletType == DOUBLE_BULLET) {
        unschedule(schedule_selector(Plane::clearDouble));
    }
    m_bulletType = type;
	scheduleOnce(schedule_selector(Plane::clearDouble), 15.0f);
}

void Plane::clearDouble(float dt)
{
    m_bulletType = BULLET;
}
