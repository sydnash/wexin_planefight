#ifndef __background_layer_h__
#define __background_layer_h__

#include "cocos2d.h"
using namespace cocos2d;

class BackgroundLayer : public Layer
{
public:
	CREATE_FUNC(BackgroundLayer);

	virtual bool init();
	void backgroundRoll(float dt);
private:
	Sprite *m_back1;
	Sprite *m_back2;
};
#endif //__background_layer_h__
