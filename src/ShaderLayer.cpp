#include "ShaderLayer.h"
using namespace std;

#define APP_SHADER_SOURCE(...) #__VA_ARGS__

const char* VERTEX_SHADER = APP_SHADER_SOURCE(
    attribute vec4 a_position;
    attribute vec2 a_texCoord;
    attribute vec4 a_color;

    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;

    void main() {                            
        gl_Position = CC_MVPMatrix * a_position;
        v_fragmentColor = a_color;
        v_texCoord = a_texCoord;
    }
);

const char* FRAG_SHADER = APP_SHADER_SOURCE(
    varying vec4 v_fragmentColor;    
    varying vec2 v_texCoord;

    uniform sampler2D u_texture;
    //CC_Time[1] is time
    
    void main() {
        vec4 c = v_fragmentColor * texture2D(u_texture, v_texCoord);
        c = (c + vec4(0.07, 0.07, 0.07, 0.0)) * vec4(1.6,abs(cos(CC_Time[1])), abs(sin(CC_Time[1])), 1.0);
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0) * c;
    }
);

xSLayer::xSLayer()
{
    p = nullptr;
}

xSLayer::~xSLayer()
{
}

xSLayer* xSLayer::create()
{
    xSLayer *pRet = new xSLayer();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool xSLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    p = new CCGLProgram;
    
	p->initWithVertexShaderByteArray(VERTEX_SHADER, FRAG_SHADER);

	p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);

	p->link();

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    renderTexture = CCRenderTexture::create(visibleSize.width, visibleSize.height);
    addChild(renderTexture);
    
    rendTexSprite = CCSprite::create();
    rendTexSprite->setTexture(renderTexture->getSprite()->getTexture());
    rendTexSprite->setTextureRect(CCRect(0, 0, rendTexSprite->getTexture()->getContentSize().width, rendTexSprite->getTexture()->getContentSize().height));
    rendTexSprite->setPosition(CCPoint(0,0));
    rendTexSprite->setAnchorPoint(CCPoint(0,0));
    rendTexSprite->setFlipY(true);
    addChild(rendTexSprite);

    rendTexSprite->setShaderProgram(p);
    
    return true;
}

void xSLayer::visit()
{
    renderTexture->beginWithClear(0, 0, 0, 0);

	CCArray* children = getChildren();

    for (int i = 0; i<children->count(); i++)
    {
		CCNode* child = reinterpret_cast<CCNode*>(children->objectAtIndex(i));

        if (child != renderTexture && child != rendTexSprite)
            child->visit();
    }
    renderTexture->end();

    rendTexSprite->visit();
}