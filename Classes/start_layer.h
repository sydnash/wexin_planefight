#ifndef __start_layer_h__
#define __start_layer_h__

#include "cocos2d.h"
using namespace cocos2d;

class StartLayer : public Layer
{
public:
	CREATE_FUNC(StartLayer);
	~StartLayer();
	static Scene* createScene();
	virtual bool init();
};

#endif //__start_layer_h__
