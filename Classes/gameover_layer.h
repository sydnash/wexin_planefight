#ifndef __game_over_h__
#define __game_over_h__

#include "cocos2d.h"
using namespace cocos2d;

class GameOver : public Layer
{
public:
    CREATE_FUNC(GameOver);
    ~GameOver() { CC_SAFE_RELEASE_NULL(m_scoreLabel); }
    virtual bool init();
    void setCurrentScore(int score);
private:
    LabelTTF *m_scoreLabel = nullptr;
    int m_score;
};

#endif //__game_over_h__
