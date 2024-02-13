// fig

#include "ShaderManager.h"

#include <Geode/modify/CCScheduler.hpp>
#include <Geode/modify/CCEGLView.hpp>

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

const char* FRAGMENT_SHADER = APP_SHADER_SOURCE(
    varying vec4 v_fragmentColor;    
    varying vec2 v_texCoord;

    uniform sampler2D u_texture;

    void main() {
        vec4 c = v_fragmentColor * texture2D(u_texture, v_texCoord);

        vec3 freakyColor = mix(vec3(0.79, 0.22, 0.69), vec3(0.97, 0.09, 0.27), v_texCoord.x);
        vec3 freakierColor = mix(vec3(0.0), freakyColor, v_texCoord.y * v_texCoord.y);
        vec3 freakiestColor = mix(c.rgb, freakierColor, 1.5 * distance(v_texCoord, vec2(0.5)));

        gl_FragColor = vec4(freakiestColor, 1.0);
    }
);

$execute {
    auto mgr = ShaderManager::get();

    mgr->shader = new CCGLProgram;
    
	mgr->shader->initWithVertexShaderByteArray(VERTEX_SHADER, FRAGMENT_SHADER);

	mgr->shader->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	mgr->shader->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	mgr->shader->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);

	mgr->shader->link();

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    mgr->texture = CCRenderTexture::create(visibleSize.width, visibleSize.height);
    
    mgr->sprite = CCSprite::create();
    mgr->sprite->setTexture(mgr->texture->getSprite()->getTexture());
    mgr->sprite->setTextureRect(CCRect(0, 0, mgr->sprite->getTexture()->getContentSize().width, mgr->sprite->getTexture()->getContentSize().height));
    mgr->sprite->setPosition(CCPoint(0,0));
    mgr->sprite->setAnchorPoint(CCPoint(0,0));
    mgr->sprite->setFlipY(true);

    mgr->sprite->setShaderProgram(mgr->shader);
    
    mgr->sprite->retain();
    mgr->texture->retain();
    mgr->shader->retain();
}

class $modify(CCScheduler) {
    void update(float delta) {
        CCScheduler::update(delta);

        auto mgr = ShaderManager::get();
        if (mgr->isActive) mgr->texture->beginWithClear(0, 0, 0, 0);
    }
};

class $modify(CCEGLView) {
    void swapBuffers() {
        auto mgr = ShaderManager::get();
        if (!mgr->isActive) return CCEGLView::swapBuffers();
        
        mgr->texture->end();
        mgr->sprite->visit();

        CCEGLView::swapBuffers();
    }
};