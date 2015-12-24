// pgeRippleSprite.h

#ifndef __PGE_RIPPLE_SPRITE_H__
#define __PGE_RIPPLE_SPRITE_H__

#include <list>

#include "cocos2d.h"

USING_NS_CC;

// --------------------------------------------------------------------------
// defines


#define RIPPLE_DEFAULT_QUAD_COUNT_X             32
#define RIPPLE_DEFAULT_QUAD_COUNT_Y             16
#define RIPPLE_BASE_GAIN                        0.1f        // an internal constant
#define RIPPLE_DEFAULT_RADIUS                   500         // radius in pixels
#define RIPPLE_DEFAULT_RIPPLE_CYCLE             0.25f       // timing on ripple ( 1/frequency )
#define RIPPLE_DEFAULT_LIFESPAN                 3.6f        // entire ripple lifespan
#define RIPPLE_CHILD_MODIFIER                   2.0f


// --------------------------------------------------------------------------
// typedefs


enum class RippleType
{
    Rubber,                                 // a soft rubber sheet
    Gel,                                    // high viscosity fluid
    Water                                   // low viscosity fluid
};


enum class RippleChildType
{
    Left,
    Top,
    Right,
    Bottom
};


struct RippleData
{
    bool                    parent;                         // ripple is a parent
    bool                    childCreated[4];              // child created ( in the 4 direction )
    RippleType             rippleType;                     // type of ripple ( se update: )
    Vec2        center;                         // ripple center ( but you just knew that, didn't you? )
    Vec2        centerCoordinate;               // ripple center in texture coordinates
    float                   radius;                         // radius at which ripple has faded 100%
    float                   strength;                       // ripple strength
    float                   runtime;                        // current run time
    float                   currentRadius;                  // current radius
    float                   rippleCycle;                    // ripple cycle timing
    float                   lifespan;                       // total life span
};


// --------------------------------------------------------------------------
// pgeRippleSprite

class pgeRippleSprite : public Node
{
public:
    pgeRippleSprite();
    virtual ~pgeRippleSprite();
    void reset() { clearRipples(); }
    
public:
    static pgeRippleSprite* create(const char* filename);
    static pgeRippleSprite* create(Texture2D* texture);
    bool initWithFile(const char* filename);
    bool initWithTexture(Texture2D* texture);
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags) override;
    void onDraw(const Mat4& transform, uint32_t flags);
    virtual void update(float dt);
    void addRipple(const Vec2& pos, RippleType type, float strength);
    bool getInverse() const { return m_inverse; }
    void setInverse(bool inverse);
protected:
    bool m_inverse; // inverse flag
protected:
    void tesselate();
    void addRippleChild(RippleData* parent, RippleChildType type);
    void clearRipples();
    
protected:
    CC_SYNTHESIZE(Texture2D*, m_texture, Texture)
    CC_SYNTHESIZE(int, m_quadCountX, QuadCountX)
    CC_SYNTHESIZE(int, m_quadCountY, QuadCountY)
    CC_SYNTHESIZE(int, m_VerticesPrStrip, VerticesPrStrip)
    CC_SYNTHESIZE(int, m_bufferSize, BuffSize)
    CC_SYNTHESIZE(Vec2*, m_vertice, Vertice)
    CC_SYNTHESIZE(Vec2*, m_textureCoordinate, TextureCoordinate)
    CC_SYNTHESIZE(Vec2*, m_rippleCoordinate, RippleCoordinate)
    CC_SYNTHESIZE_READONLY(bool*, m_edgeVertice, EdgeVertice)
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::list<RippleData*>, m_rippleList, RippleList)
    
protected:
    // render command
    CustomCommand m_customCommand;
};

#endif
