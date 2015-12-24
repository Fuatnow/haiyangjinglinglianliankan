// pgeRippleSprite.cpp

#include "pgeRippleSprite.h"

pgeRippleSprite* pgeRippleSprite::create(const char* filename)
{
    auto sprite = new (std::nothrow) pgeRippleSprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    
    CC_SAFE_DELETE(sprite);
    return NULL;
}

pgeRippleSprite* pgeRippleSprite::create(CCTexture2D* texture)
{
    auto sprite = new (std::nothrow) pgeRippleSprite();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        return sprite;
    }
    
    CC_SAFE_DELETE(sprite);
    return NULL;
}

pgeRippleSprite::pgeRippleSprite()
:m_texture(NULL),
m_vertice(NULL),
m_textureCoordinate(NULL),
m_rippleCoordinate(NULL),
m_edgeVertice(NULL)
{
}


pgeRippleSprite::~pgeRippleSprite()
{
    CC_SAFE_RELEASE(m_texture);
    CC_SAFE_DELETE_ARRAY(m_vertice);
    CC_SAFE_DELETE_ARRAY(m_textureCoordinate);
    CC_SAFE_DELETE_ARRAY(m_rippleCoordinate);
    CC_SAFE_DELETE_ARRAY(m_edgeVertice);
    
    clearRipples();
}

bool pgeRippleSprite::initWithFile(const char* filename)
{
    return initWithTexture(CCTextureCache::sharedTextureCache()->addImage(filename));
}

bool pgeRippleSprite::initWithTexture(CCTexture2D* texture)
{
    m_texture = texture;
    if (!m_texture) return false;
    m_texture->retain();
    
    m_vertice = NULL;
    m_textureCoordinate = NULL;
    CC_SAFE_DELETE_ARRAY(m_vertice);
    CC_SAFE_DELETE_ARRAY(m_textureCoordinate);
    CC_SAFE_DELETE_ARRAY(m_rippleCoordinate);
    CC_SAFE_DELETE_ARRAY(m_edgeVertice);
    m_quadCountX = RIPPLE_DEFAULT_QUAD_COUNT_X;
    m_quadCountY = RIPPLE_DEFAULT_QUAD_COUNT_Y;
    
    m_inverse = false;
    
    tesselate();
    
    scheduleUpdate();
    
    setContentSize(m_texture->getContentSize());
    //setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));
    setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
    
    return true;
}


void pgeRippleSprite::onDraw(const Mat4& transform, uint32_t flags)
{
    getGLProgram()->use();
    getGLProgram()->setUniformsForBuiltins(transform);
    GL::bindTexture2D(m_texture->getName());
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
    // TODO: use VBO or even VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    float* vertexBuffer = NULL;
    float* coordBuffer = NULL;
    CCPoint* coordSource = (m_rippleList.size() == 0) ? m_textureCoordinate : m_rippleCoordinate;
    
    if (sizeof(CCPoint) == sizeof(ccVertex2F))
    {
        vertexBuffer = (float*)m_vertice;
        coordBuffer = (float*)coordSource;
    }
    else
    {
        // NOTE: clear these soon
        static float* s_vertexBuffer = new float[2 * m_VerticesPrStrip * m_quadCountY];
        static float* s_coordBuffer = new float[2 * m_VerticesPrStrip * m_quadCountY];
        for (int i = 0; i < m_VerticesPrStrip * m_quadCountY; ++i)
        {
            s_vertexBuffer[i * 2] = m_vertice[i].x;
            s_vertexBuffer[i * 2 + 1] = m_vertice[i].y;
            s_coordBuffer[i * 2] = coordSource[i].x;
            s_coordBuffer[i * 2 + 1] = coordSource[i].y;
        }
        vertexBuffer = s_vertexBuffer;
        coordBuffer = s_coordBuffer;
    }
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertexBuffer);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, coordBuffer);
    
    for (int strip = 0; strip < m_quadCountY; ++strip)
    {
        glDrawArrays(GL_TRIANGLE_STRIP, strip * m_VerticesPrStrip, m_VerticesPrStrip);
    }
}

void pgeRippleSprite::clearRipples()
{
    auto iterBegin = m_rippleList.begin();
    
    while (iterBegin != m_rippleList.end())
    {
        RippleData* date = *iterBegin;
        
        CC_SAFE_DELETE(date);
        
        iterBegin++;
    }
    m_rippleList.clear();
}

void pgeRippleSprite::tesselate()
{
    CC_SAFE_DELETE_ARRAY(m_vertice);
    CC_SAFE_DELETE_ARRAY(m_textureCoordinate);
    CC_SAFE_DELETE_ARRAY(m_rippleCoordinate);
    CC_SAFE_DELETE_ARRAY(m_edgeVertice);
    
    m_VerticesPrStrip = 2 * (m_quadCountX + 1);
    m_bufferSize = m_VerticesPrStrip * m_quadCountY;
    
    //allocate buffers
    m_vertice = new CCPoint[m_bufferSize];
    m_textureCoordinate = new CCPoint[m_bufferSize];
    m_rippleCoordinate = new CCPoint[m_bufferSize];
    m_edgeVertice = new bool[m_bufferSize];
    
    int vertexPos = 0;
    CCPoint normalized;
    CCSize contentSize = m_texture->getContentSize();
    
    for (int y = 0; y < m_quadCountY; ++y)
    {
        for (int x = 0; x < (m_quadCountX + 1); ++x)
        {
            for (int yy = 0; yy < 2; ++yy)
            {
                // first simply calculate a normalized position into rectangle
                normalized.x = (float)x / (float)m_quadCountX;
                normalized.y = (float)(y + yy) / (float)m_quadCountY;
                
                // calculate vertex by multiplying rectangle ( texture ) size
                m_vertice[vertexPos] = ccp(normalized.x * contentSize.width, normalized.y * contentSize.height);
                
                // adjust texture coordinates according to texture size
                // as a texture is always in the power of 2, maxS and maxT are the fragment of the size actually used
                // invert y on texture coordinates
                m_textureCoordinate[vertexPos] = ccp(normalized.x * m_texture->getMaxS(), m_texture->getMaxT() - (normalized.y * m_texture->getMaxT()));
                
                // check if vertice is an edge vertice, because edge vertices are never modified to keep outline consistent
                m_edgeVertice[vertexPos] = (
                                            (x == 0) ||
                                            (x == m_quadCountX) ||
                                            ((y == 0) && (yy == 0)) ||
                                            ((y == (m_quadCountY - 1)) && (yy > 0)));
                
                // next buffer pos
                ++vertexPos;
            }
        }
    }
}


void pgeRippleSprite::addRipple(const cocos2d::CCPoint &pos, RippleType type, float strength)
{
    // allocate new ripple
    RippleData* newRipple = new RippleData();
    
    // initialize ripple
    newRipple->parent = true;
    for (int count = 0; count < 4; ++count)
    {
        newRipple->childCreated[count] = false;
    }
    newRipple->rippleType = type;
    newRipple->center = pos;
    
    CCSize contentSize = m_texture->getContentSize();
    newRipple->centerCoordinate = ccp(pos.x / contentSize.width * m_texture->getMaxS(), m_texture->getMaxT() - (pos.y / contentSize.height * m_texture->getMaxT()));
    newRipple->radius = RIPPLE_DEFAULT_RADIUS;
    newRipple->strength = strength;
    newRipple->runtime = 0;
    newRipple->currentRadius = 0;
    newRipple->rippleCycle = RIPPLE_DEFAULT_RIPPLE_CYCLE;
    newRipple->lifespan = RIPPLE_DEFAULT_LIFESPAN;
    
    // add ripple to running list
    m_rippleList.push_back(newRipple);
}


void pgeRippleSprite::addRippleChild(RippleData* parent, RippleChildType type)
{
    // allocate new ripple
    RippleData* newRipple = new RippleData();
    CCPoint pos;
    
    // new ripple is pretty much a copy of its parent
    memcpy(newRipple, parent, sizeof(RippleData));
    
    // not a parent
    newRipple->parent = false;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // mirror position
    switch (type) {
        case RippleChildType::Left:
            pos = ccp(-parent->center.x, parent->center.y);
            break;
        case RippleChildType::Top:
            pos = ccp(parent->center.x, winSize.height + (winSize.height - parent->center.y));
            break;
        case RippleChildType::Right:
            pos = ccp(winSize.width + (winSize.width - parent->center.x), parent->center.y);
            break;
        case RippleChildType::Bottom:
        default:
            pos = ccp(parent->center.x, -parent->center.y);
            break;
    }
    
    newRipple->center = pos;
    
    CCSize contentSize = m_texture->getContentSize();
    
    newRipple->centerCoordinate = ccp(pos.x / contentSize.width * m_texture->getMaxS(), m_texture->getMaxT() - (pos.y / contentSize.height * m_texture->getMaxT()));
    newRipple->strength *= RIPPLE_CHILD_MODIFIER;
    
    // indicate child used
    parent->childCreated[(unsigned)type] = true;
    
    // add ripple to running list
    m_rippleList.push_back(newRipple);
}


void pgeRippleSprite::update(float dt)
{
    // test if any ripples at all
    if (m_rippleList.size() == 0) return;
    
    RippleData* ripple;
    CCPoint pos;
    float distance, correction;
    
    // ripples are simulated by altering texture coordinates
    // on all updates, an entire new array is calculated from the base array
    // not maintaining an original set of texture coordinates, could result in accumulated errors
    memcpy(m_rippleCoordinate, m_textureCoordinate, m_bufferSize * sizeof(CCPoint));
    
    // scan through running ripples
    // the scan is backwards, so that ripples can be removed on the fly
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    auto iterRipple = m_rippleList.rbegin();
    
    while (iterRipple != m_rippleList.rend())
    {
        // get ripple data
        ripple = *iterRipple;
        
        // scan through all texture coordinates
        for (int count = 0; count < m_bufferSize; ++count)
        {
            // don't modify edge vertices
            if (!m_edgeVertice[count])
            {
                // calculate distance
                // you might think it would be faster to do a box check first
                // but it really isn't,
                // ccpDistance is like my sexlife - BAM! - and its all over
                distance = ccpDistance(ripple->center, m_vertice[count]);
                
                // only modify vertices within range
                if (distance <= ripple->currentRadius)
                {
                    // load the texture coordinate into an easy to use var
                    pos = m_rippleCoordinate[count];
                    
                    // calculate a ripple
                    switch (ripple->rippleType)
                    {
                        case RippleType::Rubber:
                            // method A
                            // calculate a sinus, based only on time
                            // this will make the ripples look like poking a soft rubber sheet, since sinus position is fixed
                            correction = sinf(2 * M_PI * ripple->runtime / ripple->rippleCycle);
                            break;
                            
                        case RippleType::Gel:
                            // method B
                            // calculate a sinus, based both on time and distance
                            // this will look more like a high viscosity fluid, since sinus will travel with radius
                            correction = sinf(2 * M_PI * (ripple->currentRadius - distance) / ripple->radius * ripple->lifespan / ripple->rippleCycle);
                            break;
                            
                        case RippleType::Water:
                        default:
                            // method c
                            // like method b, but faded for time and distance to center
                            // this will look more like a low viscosity fluid, like water
                            
                            correction = (ripple->radius * ripple->rippleCycle / ripple->lifespan) / (ripple->currentRadius - distance);
                            if (correction > 1.0f) correction = 1.0f;
                            
                            // fade center of quicker
                            correction *= correction;
                            
                            correction *= sinf(2 * M_PI * (ripple->currentRadius - distance) / ripple->radius * ripple->lifespan / ripple->rippleCycle);
                            break;
                    }
                    
                    // fade with distance
                    correction *= 1 - (distance / ripple->currentRadius);
                    
                    // fade with time
                    correction *= 1 - (ripple->runtime / ripple->lifespan);
                    
                    // adjust for base gain and user strength
                    correction *= RIPPLE_BASE_GAIN;
                    correction *= ripple->strength;
                    
                    // finally modify the coordinate by interpolating
                    // because of interpolation, adjustment for distance is needed,
                    correction /= ccpDistance(ripple->centerCoordinate, pos);
                    pos = ccpAdd(pos, ccpMult(ccpSub(pos, ripple->centerCoordinate), correction));
                    
                    // another approach for applying correction, would be to calculate slope from center to pos
                    // and then adjust based on this
                    
                    // clamp texture coordinates to avoid artifacts
                    pos = ccpClamp(pos, Vec2::ZERO, ccp(m_texture->getMaxS(), m_texture->getMaxT()));
                    
                    // save modified coordinate
                    m_rippleCoordinate[count] = pos;
                }
            }
        }
        
        // calculate radius
        ripple->currentRadius = ripple->radius * ripple->runtime / ripple->lifespan;
        
        // check if ripple should expire
        ripple->runtime += dt;
        if (ripple->runtime >= ripple->lifespan)
        {
            // free memory, and remove from list
            CC_SAFE_DELETE(ripple);
            
            auto it = --iterRipple.base();
            auto it_after_del = m_rippleList.erase(it);
            iterRipple = std::list<RippleData*>::reverse_iterator(it_after_del);
        }
        else
        {
            // check for creation of child ripples
            // NOTE: now we do not need this
            /*
             if (ripple->parent == true)
             {
             // left ripple
             if ((ripple->childCreated[(unsigned)RippleChildType::Left] == false) && (ripple->currentRadius > ripple->center.x))
             {
             addRippleChild(ripple, RippleChildType::Left);
             }
             
             // top ripple
             if ((ripple->childCreated[(unsigned)RippleChildType::Top] == false) && (ripple->currentRadius > winSize.height - ripple->center.y))
             {
             addRippleChild(ripple, RippleChildType::Top);
             }
             
             // right ripple
             if ((ripple->childCreated[(unsigned)RippleChildType::Right] == false) && (ripple->currentRadius > winSize.width - ripple->center.x))
             {
             addRippleChild(ripple, RippleChildType::Right);
             }
             
             // bottom ripple
             if ((ripple->childCreated[(unsigned)RippleChildType::Bottom] == false) && (ripple->currentRadius > ripple->center.y))
             {
             addRippleChild(ripple, RippleChildType::Bottom);
             }
             }
             */
            iterRipple++;
        }
    }
}

void pgeRippleSprite::setInverse(bool inverse)
{
    if (inverse != m_inverse)
    {
        m_inverse = inverse;
        
        for (int i = 0; i < m_VerticesPrStrip * m_quadCountY; ++i)
        {
            m_textureCoordinate[i].y = 1.0f - m_textureCoordinate[i].y;
        }
    }
}

void pgeRippleSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) {
    m_customCommand.init(_globalZOrder);
    m_customCommand.func = CC_CALLBACK_0(pgeRippleSprite::onDraw, this, transform, flags);
    renderer->addCommand(&m_customCommand);
}
