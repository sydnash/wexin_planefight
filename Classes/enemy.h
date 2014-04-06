#ifndef __enemy_h__
#define __enemy_h__
#include "cocos2d.h"
using namespace cocos2d;



class Enemy : public Sprite
{
public:
    enum  EnemyType {
        LARGE = 0,
        MIDDLE = 1,
        SMALL = 2,
    };
    ~Enemy();
    static Enemy *create(EnemyType type);
    void destory();
    bool startMove(const Point &start, const Point &end, float duation);
    bool enemyDelete();
    bool hitOnce();
    bool isDestroying() const { return m_isDestroying; }
    void setDestroying();
    int getScore() const { return m_score; }
private:
    void initLife(EnemyType type);
    bool initSelf(EnemyType type);
    bool initSpriteAction(EnemyType type);
    bool initDestroyAction(EnemyType type);
    bool initHitAction(EnemyType type);
private:
    Sequence *m_destroyAction = nullptr;
    Animate *m_hitAction = nullptr;

    int m_life;
    bool m_isDestroying = false;
    int m_score;
};
#endif //__enemy_h__
