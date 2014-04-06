#ifndef __ufo_h__
#define __ufo_h__

#include "cocos2d.h"
using namespace cocos2d;

class Ufo : public Sprite
{
public:
    enum {
        BOMB = 0,
        BULLET = 1,
    };
    static Ufo* createUfo(int type);
    void initUfo(int type);
    int getType() const { return m_type; }
    void setType(int type) { m_type = type; }
private:
    int m_type;
};

#endif //__ufo_h__
