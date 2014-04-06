#include "gameover_layer.h"
#include "background_layer.h"
#include "game_layer.h"

void menuCallback(Object *sender)
{
    Scene *s = Scene::create();
    BackgroundLayer *gLayer = BackgroundLayer::create();
    s->addChild(gLayer);

    GameLayer *pLayer = GameLayer::create();
    s->addChild(pLayer, 1);

    Director *pDirector = Director::getInstance();
    pDirector->replaceScene(s);
}

bool GameOver::init()
{
    bool ret = false;
    do {
        CC_BREAK_IF(!Layer::init());
        Sprite *backGround1 = Sprite::createWithSpriteFrameName("gameover.png");
        backGround1->setAnchorPoint(Point(0, 0));
        backGround1->setPosition(Point(0, 0));
        this->addChild(backGround1, -1, 0);

        Size size = Director::getInstance()->getWinSize();

        MenuItemImage *item = MenuItemImage::create("ui/shoot_background/btn_finish.png"
            , "ui/shoot_background/btn_finish.png", menuCallback);
        Menu *menu = Menu::create(item, NULL);
        addChild(menu);
        item->setAnchorPoint(Point(0, 0));
        menu->setPosition(Point(0, 0));
        item->setPosition(Point(size.width - item->getContentSize().width, 0));

        m_scoreLabel = LabelTTF::create("0", "Arial", 28);
        m_scoreLabel->setFontSize(40);
        addChild(m_scoreLabel);

        m_scoreLabel->setPosition(Point(size.width * 0.5, size.height * 0.5));
        ret = true;
    } while (0);
    return ret;
}

void GameOver::setCurrentScore(int score)
{
    char p[100];
    sprintf(p, "%d", score);
    m_scoreLabel->setString(p);
}
