#include "enemy.h"

void arrivedPlace(Node *node)
{
    Enemy* en = dynamic_cast<Enemy*>(node);
    en->setVisible(false);
}

static const std::vector<std::string> LARGE_DESTROY = {
    "enemy3_down1.png",
    "enemy3_down2.png",
    "enemy3_down3.png",
    "enemy3_down4.png",
    "enemy3_down5.png",
    "enemy3_down6.png",
};

static const std::vector<std::string> LARGE_HIT = {
    "enemy3_hit.png",
    "enemy3_n1.png",
};

static const std::vector<std::string> MIDDLE_DESTROY = {
    "enemy2_down1.png",
    "enemy2_down2.png",
    "enemy2_down3.png",
    "enemy2_down4.png",
};

static const std::vector<std::string> MIDDLE_HIT = {
    "enemy2_hit.png",
    "enemy2.png",
};

static const std::vector<std::string> SMALL_DESTROY = {
    "enemy1_down1.png",
    "enemy1_down2.png",
    "enemy1_down3.png",
    "enemy1_down4.png",
};

Enemy::~Enemy()
{
    CC_SAFE_RELEASE_NULL(m_destroyAction);
    CC_SAFE_RELEASE_NULL(m_hitAction);
}

bool Enemy::initSelf(EnemyType type)
{
    initSpriteAction(type);
    initDestroyAction(type);
    initHitAction(type);
    initLife(type);
    return true;
}


bool Enemy::initHitAction(EnemyType type)
{
    std::vector<std::string> name;
    switch (type) {
    case EnemyType::SMALL:
        return true;
    case EnemyType::MIDDLE:
        name = MIDDLE_HIT;
        break;
    case EnemyType::LARGE:
        name = LARGE_HIT;
        break;
    default:
        assert(false);
        return true;
    }
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    Animation *animation = Animation::create();
    animation->setDelayPerUnit(0.1f);
    for (auto t : name) {
        animation->addSpriteFrame(cache->getSpriteFrameByName(t));
    }
    m_hitAction = Animate::create(animation);
    m_hitAction->retain();
    return true;
}

bool Enemy::initDestroyAction(EnemyType type)
{
    std::vector<std::string> name;
    switch (type) {
    case EnemyType::SMALL:
        name = SMALL_DESTROY;
        break;
    case EnemyType::MIDDLE:
        name = MIDDLE_DESTROY;
        break;
    case EnemyType::LARGE:
        name = LARGE_DESTROY;
        break;
    default:
        assert(false);
        break;
    }
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    Animation *animation = Animation::create();
    animation->setDelayPerUnit(0.2f);
    for (size_t i = 0; i < name.size(); ++i) {
        animation->addSpriteFrame(cache->getSpriteFrameByName(name[i]));
    }
    Animate* animate = Animate::create(animation);
    FiniteTimeAction *callback = CallFuncN::create(std::bind(arrivedPlace, this));
    m_destroyAction = Sequence::create(animate, callback, nullptr);
    m_destroyAction->retain();
    m_destroyAction->setTag(2);
    return true;
}

void Enemy::destory()
{
    m_isDestroying = true;
    stopAllActions();
    runAction(m_destroyAction->clone());
}

bool Enemy::initSpriteAction(EnemyType type)
{
    if (EnemyType::LARGE == type) {
        auto cache = SpriteFrameCache::getInstance();
        Animation *animation = Animation::create();
        animation->addSpriteFrame(cache->getSpriteFrameByName("enemy3_n1.png"));
        animation->addSpriteFrame(cache->getSpriteFrameByName("enemy3_n2.png"));
        animation->setDelayPerUnit(0.1f);
        Animate *animate = Animate::create(animation);
        animate->setTag(1);
        runAction(RepeatForever::create(animate));
    }
    return true;
}

Enemy* Enemy::create(EnemyType type)
{
    SpriteFrame *spriteFrame = nullptr;
    auto cache = SpriteFrameCache::getInstance();
    switch (type) {
    case EnemyType::SMALL:
        spriteFrame = cache->getSpriteFrameByName("enemy1.png");
        break;
    case EnemyType::MIDDLE:
        spriteFrame = cache->getSpriteFrameByName("enemy2.png");
        break;
    case EnemyType::LARGE:
        spriteFrame = cache->getSpriteFrameByName("enemy3_n1.png");
    }
    Enemy *en = new Enemy();
    if (spriteFrame && en && en->initWithSpriteFrame(spriteFrame)) {
        en->autorelease();
        en->initSelf(type);
        en->setVisible(false);
        return en;
    } else {
        delete en;
        return nullptr;
    }
}

bool Enemy::startMove(const Point &start, const Point &end, float duation)
{
    this->setPosition(start);
    MoveTo *move = MoveTo::create(duation, end);
    FiniteTimeAction *callback = CallFuncN::create(std::bind(arrivedPlace, this));
    Sequence *seq = Sequence::create(move, callback, NULL);
    setVisible(true);
    seq->setTag(1);
    runAction(seq);
    return true;
}

void Enemy::initLife(EnemyType type)
{
    switch (type) {
    case SMALL:
        m_life = 1;
        m_score = 1000;
        break;
    case MIDDLE:
        m_life = 5;
        m_score = 5000;
        break;
    case LARGE:
        m_life = 10;
        m_score = 10000;
        break;
    default:
        assert(false);
        break;
    }
}

bool Enemy::hitOnce()
{
	if (m_life == 0) {
		return true;
	}
	m_life--;
	if (m_life != 0 && m_hitAction) {
        this->runAction((Action*)m_hitAction->clone());
	}
	return (m_life == 0);
}
