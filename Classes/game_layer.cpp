#include "game_layer.h"
#include "enemy.h"
#include "plane.h"
#include "ufo.h"
#include "gameover_layer.h"

GameLayer::~GameLayer()
{
    CC_SAFE_RELEASE_NULL(m_bombItem);
    CC_SAFE_RELEASE_NULL(m_bombMenu);
    CC_SAFE_RELEASE_NULL(m_bombCount);
    CC_SAFE_RELEASE_NULL(m_item2);
    CC_SAFE_RELEASE_NULL(m_menu);
    CC_SAFE_RELEASE_NULL(m_scoreLabel);
}

bool GameLayer::init()
{
	auto ret = false;
	do {
		CC_BREAK_IF(!Layer::init());
        //create enemy batch node.
        Size s = Director::getInstance()->getWinSize();
        m_enemys = SpriteBatchNode::create("ui/shoot.png", 40);
        addChild(m_enemys);

        //crate hero
        m_hero = Plane::create();
        m_hero->setPosition(Point(200, 400));
        addChild(m_hero);
		Point pos(s.width * 0.5, m_hero->getContentSize().height * 0.5);
        m_hero->present(pos);

        //start shoot
		m_bullets = SpriteBatchNode::create("ui/shoot.png", 50);
		addChild(m_bullets);
        m_hero->startShoot(m_bullets);

        //start box hit
        scheduleUpdate();

        //create ufo
		m_ufos = SpriteBatchNode::create("ui/shoot.png", 2);
        addChild(m_ufos);
		schedule(schedule_selector(GameLayer::addUfo), 25.0);

        //create enemy
        schedule(schedule_selector(GameLayer::addSmallEnemy), 0.6f);
        schedule(schedule_selector(GameLayer::addMiddleEnemy), 12.6f);
        schedule(schedule_selector(GameLayer::addLargeEnemy), 30.6f);

        //add menu
        addMenu();
        ret = true;
	} while (0);
    return ret;
}

void GameLayer::createEnemyRoad(float speed, float range, Point &p1, Point &p2, float &duration)
{
    Size size = Director::getInstance()->getWinSize();
    float w = (float)size.width; float h = (float)size.height;
    w = CCRANDOM_0_1() * w;
    int bb = m_hero->getContentSize().width * 0.5;
    w = MAX(bb, MIN(w, size.width - bb));

    float length = h;
    float t = CCRANDOM_0_1() * range;
    speed = (speed + t) / 1; //速度
    duration = length / speed; //飞行时间

    p1 = Point(w, h);
    p2 = Point(w, 0);
}

void GameLayer::addSmallEnemy(float dt)
{
    Enemy *en = Enemy::create(Enemy::SMALL);
    assert(en);
    Point start, end;
    float duration;
    float speed = 200;

    createEnemyRoad(speed, 100, start, end, duration);

    en->startMove(start, end, duration);

    m_enemys->addChild(en);
}

void GameLayer::addMiddleEnemy(float dt)
{
    Enemy *en = Enemy::create(Enemy::MIDDLE);
    assert(en);
    Point start, end;
    float duration;
    float speed = 150;
    createEnemyRoad(speed, 100, start, end, duration);

    en->startMove(start, end, duration);
    m_enemys->addChild(en);
}

void GameLayer::addLargeEnemy(float dt)
{
    Enemy *en = Enemy::create(Enemy::LARGE);
    assert(en);
    Point start, end;
    float duration;
    float speed = 80;
    createEnemyRoad(speed, 50, start, end, duration);

    en->startMove(start, end, duration);
    m_enemys->addChild(en);
}

void GameLayer::update(float delta)
{
    if (m_isOver) {
        if (!m_hero->isVisible()) {
            m_hero->stopShoot();
            //show game over scene.
            GameOver *g = GameOver::create();
            Scene *s = Scene::create();
            s->addChild(g);
            Director::getInstance()->replaceScene(s);
        }
        return;
    }
    Vector<Node*> ens = m_enemys->getChildren();
    Vector<Node*> bus = m_bullets->getChildren();

    Vector<Node*> btod; //bullets to be delete
    Vector<Node*> etod; //enemys to be delete

    //remove all enemys arrived position
    for (auto o : ens) {
        if (!o->isVisible()) {
            etod.pushBack(o);
        }
    }
    for (auto o : etod) {
        m_enemys->removeChild(o, true);
    }
    etod.clear();
    ens.clear();
    ens = m_enemys->getChildren();

    //remove all bullets arrived position
    for (auto o : bus) {
        if (!o->isVisible()) {
            btod.pushBack(o);
        }
    }
    for (auto o : btod) {
        m_bullets->removeChild(o, true);
    }
    btod.clear();
    bus.clear();
    bus = m_bullets->getChildren();

    for (auto o1 : bus) {
        Sprite *b = dynamic_cast<Sprite*>(o1);
        Rect re1 = b->getBoundingBox();
        for (auto o2 : ens) {
            Enemy *e = dynamic_cast<Enemy*>(o2);
            if (e->isDestroying()) {
                continue;
            }
            Rect re2 = e->getBoundingBox();
            bool isremove = false;
            if (re1.intersectsRect(re2)) {
                isremove = true;
                b->stopAllActions();
                b->setVisible(false);
                if (e->hitOnce()) {
                    addScore(e);
                    e->destory();
                }
            }

            if (isremove) {
                if (!btod.contains(b)) {
                    btod.pushBack(b);
                }
            }
        }
    }

    for (auto o : btod) {
        m_bullets->removeChild(o, true);
    }
    btod.clear();

    Rect re1 = m_hero->getBoundingBox();

#if 1
    for (auto o : ens) {
        Enemy *e = (Enemy*)o;
        if (!e->isVisible() || e->isDestroying()) {
            continue;
        }
        Rect re2 = e->getBoundingBox();
        if (re1.intersectsRect(re2)) {
            //plane die.
            if (!m_isOver) {
                m_isOver = true;
                m_hero->hitOnce();
            }
        }
    }
    if (m_isOver) {
        return;
    }
#endif
    Vector<Node*> ufos = m_ufos->getChildren();
    for (auto o : ufos) {
        Ufo *ufo = (Ufo*)o;
        Rect re2 = ufo->getBoundingBox();
        if (re1.intersectsRect(re2)) {
            acceptUfo(ufo);
        }
    }
}

void GameLayer::acceptUfo(Ufo *ufo) 
{
	switch (ufo->getType()) {
		case Ufo::BOMB:
			m_ufos->removeChild(ufo, true);
			addBomb();
			break;
		case Ufo::BULLET:
			m_ufos->removeChild(ufo, true);
            m_hero->setSpecialBullet(Plane::DOUBLE_BULLET);
			break;
		default:
			break;
	}
}

void GameLayer::addUfo(float dt)
{
	double k = CCRANDOM_0_1();
	Ufo *m_u;
	if (k < 0.5) {
		m_u = Ufo::createUfo(Ufo::BOMB);
	} else {
		m_u = Ufo::createUfo(Ufo::BULLET);
	}

	Size size = Director::getInstance()->getWinSize();
	int w = size.width; int h = size.height;
	w = CCRANDOM_0_1() * w;
	int bb = m_hero->getContentSize().width * 0.5;
	w = MAX(bb, MIN(w, size.width - bb));
	m_u->setPosition(Point(w, h));

	float length = h + 100;//飞行距离，超出屏幕即结束
	float velocity = (300) / 1;//飞行速度：420pixel/sec  
	float realMoveDuration = length / velocity;//飞行时间  

	FiniteTimeAction* actionMove1 = MoveTo::create(h * 0.4 / (velocity * 2), Point(w, h * 0.6));
	FiniteTimeAction* actionMove2 = MoveTo::create(h * 0.4 / (velocity * 2), Point(w, h));
	FiniteTimeAction* actionMove3 = MoveTo::create(realMoveDuration, Point(w, 0));
	FiniteTimeAction* actionDone = CallFuncN::create(this,callfuncN_selector(GameLayer::ufoFinished));//回调一个子弹结束处理函数  

	Sequence* sequence = (Sequence*)Sequence::create(actionMove1, actionMove2, actionMove3, actionDone, NULL);
	sequence->setTag(1);

	m_u->runAction(sequence);

	m_ufos->addChild(m_u);
}

void GameLayer::ufoFinished(Node *sender)
{
	m_ufos->removeChild(sender, true);
}


void GameLayer::addBomb()
{
    m_bomb++;
    char p[20];
    sprintf(p, "X%d", m_bomb);
    m_bombCount->setString(p);
    if (m_bomb == 1) {
        m_bombMenu->addChild(m_bombItem);
        addChild(m_bombCount);
    }
}

void GameLayer::bombCallback(Object *sender)
{
	m_bomb--;
	char p[20];
	sprintf(p, "X%d", m_bomb);
	m_bombCount->setString(p);
	Vector<Node*> ens = m_enemys->getChildren();
	for (auto o : ens) {
		Enemy *e = (Enemy*)o;
        addScore(e);
        e->destory();
	}
	if (m_bomb == 0) {
		m_bombMenu->removeChild(m_bombItem, true);
		removeChild(m_bombCount, true);
	}
}

void GameLayer::pauseCallback(Object *sender)
{
	m_isPause = !m_isPause;
	if (m_isPause) {
		m_menu->removeChildByTag(1, false);
		m_menu->addChild(m_item2, 2);
		Director::getInstance()->pause();
	} else {
		m_menu->removeChildByTag(2, false);
		m_menu->addChild(m_item1, 1);
		Director::getInstance()->resume();
	}
}

void GameLayer::addMenu()
{
    Size s = Director::getInstance()->getWinSize();
    //bomb menu
    m_bombItem = MenuItemImage::create("ui/shoot/bomb.png", "ui/shoot/bomb.png", this, menu_selector(GameLayer::bombCallback));
    m_bombItem->retain();
    m_bombItem->setTag(5);
    m_bombItem->setAnchorPoint(Point(0, 0));
    m_bombItem->setPosition(Point(0, 0));
    m_bombMenu = Menu::create();
    m_bombMenu->retain();
    m_bombMenu->setPosition(0, 0);
    addChild(m_bombMenu);
    m_bombCount = LabelTTF::create("0", "Arial", 40);
    m_bombCount->setAnchorPoint(Point(0, 0));
    m_bombCount->setPosition(Point(m_bombItem->getContentSize().width, 0));
    m_bombCount->retain();

    //pause menu
    m_item1 = MenuItemImage::create("ui/shoot/game_pause_nor.png", "ui/shoot/game_pause_pressed.png", this, menu_selector(GameLayer::pauseCallback));
    m_item1->retain();
    m_menu = Menu::create();
    m_menu->retain();
    m_item1->setTag(1);
    m_menu->addChild(m_item1);
    addChild(m_menu, 10);
    m_menu->setPosition(0, 0);
    m_item1->setPosition(0, s.height - m_item1->getContentSize().height);
    m_item1->setAnchorPoint(Point(0, 0));
    m_item2 = MenuItemImage::create("ui/shoot/game_resume_nor.png", "ui/shoot/game_resume_pressed.png", this, menu_selector(GameLayer::pauseCallback));
    m_item2->retain();
    m_item2->setPosition(0, s.height - m_item2->getContentSize().height);
    m_item2->setAnchorPoint(Point(0, 0));
    m_item2->setTag(2);
    m_isPause = false;
    m_scoreLabel = LabelTTF::create("0", "Arial", 28);
    m_scoreLabel->setFontSize(40);
    m_scoreLabel->retain();
    addChild(m_scoreLabel);
    m_scoreLabel->setPosition(Point(m_item1->getPosition().x + m_item1->getContentSize().width, m_item1->getPosition().y));
    m_scoreLabel->setAnchorPoint(Point(0, 0));
    m_score = 0;
}

void GameLayer::addScore(Enemy *e)
{
    int score = e->getScore();
    m_score += score;
    char p[100];
    sprintf(p, "%d", m_score);
    m_scoreLabel->setString(p);
}
