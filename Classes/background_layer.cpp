#include "background_layer.h"

Sprite *createBackGround()
{
	Sprite* s = Sprite::createWithSpriteFrameName("background.png");
	s->setAnchorPoint(Point(0.0, 0.0));
	s->setPosition(0, 0);
	return s;
}

bool BackgroundLayer::init()
{
	bool ret = false;
	do {
		CC_BREAK_IF(!Layer::init());
		
		SpriteBatchNode *nodes = SpriteBatchNode::create("ui/shoot_background.png");
		addChild(nodes);
		m_back1 = createBackGround();
		
		nodes->addChild(m_back1, -10);
		m_back2 = createBackGround();
		m_back2->getTexture()->setAliasTexParameters(); //设置抗锯齿之后才不会有黑线。
		nodes->addChild(m_back2, -5);

		schedule(schedule_selector(BackgroundLayer::backgroundRoll));
		ret = true;
	} while (0);
	return ret;
}

void BackgroundLayer::backgroundRoll(float dt)
{
	float y = m_back1->getPositionY();
	m_back1->setPositionY(y - 2);
	y = m_back1->getPositionY();
	int h = m_back1->getContentSize().height;
	m_back2->setPositionY(y + h - 2);
	if (m_back2->getPositionY() == 0) {
		m_back1->setPositionY(0);
	}
}