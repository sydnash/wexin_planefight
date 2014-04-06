#ifndef __plane_h__
#define __plane_h__

#include "cocos2d.h"
using namespace cocos2d;

class Plane : public Sprite
{
public:
    enum BulletType {
        BULLET,
        DOUBLE_BULLET,
    };
    ~Plane();
    static Plane* create();
    void present(const Point &pos);
    bool hitOnce();
    void startShoot(SpriteBatchNode *nodes);
    void stopShoot();
    void setSpecialBullet(BulletType type);
    void clearDouble(float dt);
private:
    void initFlyAction();
    void initDestroyAction();
    void addBullet(float dt);
private:
    void addTouchEvent();
    bool m_isMoving = false;
    SpriteBatchNode* m_bullets = nullptr;
    int m_life = 1;
    bool m_dead = false;
    Sequence *m_destroyAction = nullptr;
    BulletType m_bulletType = BULLET;
};
#endif //__plane_h__
