//
//  ActiveGrass.cpp
//  SeaLink
//
//  Created by Fuatnow on 15-5-3.
//
//

#include "ActiveGrass.h"

//--------------------------------------------------------
static const GLchar* szGrassShader =
"																									\n\
#ifdef GL_ES																						\n\
precision mediump float;																		\n\
#endif																								\n\
\n\
varying vec2 v_texCoord;																			\n\
uniform sampler2D u_texture;																		\n\
uniform float u_bendFactor;																			\n\
uniform float u_time;																				\n\
\n\
// 1																								\n\
const float speed = 2.0;																			\n\
const float bendFactor = 0.2;																		\n\
void main()																							\n\
{																									\n\
// 获得高度，texCoord从下到上为0到1																	\n\
float height = 1.0 - v_texCoord.y;																	\n\
// 获得偏移量，一个幂函数，值愈大，导数越大，偏移量愈大													\n\
float offset = pow(height, 3.5);																	\n\
// 偏移量随时间变化，并乘以幅度，设置频率																\n\
offset *= (sin(u_time * speed) * bendFactor);														\n\
// 使x坐标偏移，fract取区间值（0，1）																	\n\
vec4 normalColor = texture2D(u_texture, fract(vec2(v_texCoord.x + offset, v_texCoord.y)));		\n\
gl_FragColor = normalColor;																		\n\
}";
//--------------------------------------------------------
ActiveGrass* ActiveGrass::create(const char* filename,float bendFactor)
{
    auto ag = new ActiveGrass;
    ag->init(filename,bendFactor);
    ag->autorelease();
    return ag;
}

bool ActiveGrass::init(const char* filename,float bendFactor)
{
    initWithFile(filename);
    initData(bendFactor);
    initView();
    return true;
}

void ActiveGrass::initData(float bendFactor)
{
    m_fTime = 0.0f;
    m_bendFactor = bendFactor;
}

void ActiveGrass::initView()
{
    // 加载顶点着色器和片元着色器;
    GLProgram* pShader = new GLProgram();
    pShader->initWithByteArrays(ccPositionTextureA8Color_vert, szGrassShader );
    this->setGLProgram( pShader );
    pShader->release();
    CHECK_GL_ERROR_DEBUG();
    
    
    // 启用顶点着色器的attribute变量，坐标、纹理坐标、颜色
    this->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
    this->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
    CHECK_GL_ERROR_DEBUG();
    
    // 自定义着色器链接
    this->getGLProgram()->link();
    CHECK_GL_ERROR_DEBUG();
    
    // 设置移动、缩放、旋转矩阵
    this->getGLProgram()->updateUniforms();
    CHECK_GL_ERROR_DEBUG();
    
    m_nTimeUniformLocation = glGetUniformLocation(this->getGLProgram()->getProgram(), "u_time");
    m_bendFactorUniformLocation =glGetUniformLocation(this->getGLProgram()->getProgram(), "u_bendFactor");
    this->getGLProgram()->use();
    // 开启帧更新
    this->scheduleUpdate();
}

void ActiveGrass::update(float dt)
{
    m_fTime  += dt;
    this->getGLProgram()->use();
    glUniform1f(m_bendFactorUniformLocation, m_bendFactor);
    glUniform1f(m_nTimeUniformLocation, m_fTime);
}