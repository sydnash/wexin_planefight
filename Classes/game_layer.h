#ifndef __game_layer_h__
#define __game_layer_h__

#include "cocos2d.h"
using namespace cocos2d;

class Plane;
class Ufo;
class Enemy;

class GameLayer : public Layer
{
public:
    ~GameLayer();
	CREATE_FUNC(GameLayer);
	virtual bool init();
    virtual void update(float delta);
private:
    void addSmallEnemy(float dt);
    void addMiddleEnemy(float dt);
    void addLargeEnemy(float dt);
    void createEnemyRoad(float speed, float range, Point &p1, Point &p2, float &duration);

    void addUfo(float dt);
    void ufoFinished(Node *sender);
    void acceptUfo(Ufo *ufo);
    void addBomb();
	void bombCallback(Object *sender);
	void pauseCallback(Object *sender);
    void addMenu();
    void addScore(Enemy *e);
private:
    SpriteBatchNode *m_enemys;
    SpriteBatchNode *m_bullets;
    SpriteBatchNode *m_ufos;
    Plane *m_hero;

    bool m_isOver = false;

	int m_bomb = 0;
	MenuItemImage *m_bombItem;
	Menu *m_bombMenu;
	LabelTTF *m_bombCount;

	MenuItemImage *m_item1;
	MenuItemImage *m_item2;
	Menu *m_menu;
	LabelTTF *m_scoreLabel;
	int m_score = 0;
    bool m_isPause = false;
};

#endif //__game_layer_h__
