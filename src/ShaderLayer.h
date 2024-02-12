#ifndef __xSLayer_H__
#define __xSLayer_H__

#include "cocos2d.h"
#include <string>
USING_NS_CC;

class xSLayer : public cocos2d::CCLayer
{
    CCGLProgram* p;
public:
    xSLayer();
    ~xSLayer();

    virtual bool init();  
    static xSLayer* create();

    CCRenderTexture* renderTexture;
    CCSprite* rendTexSprite;

    virtual void visit() override;
};

#endif