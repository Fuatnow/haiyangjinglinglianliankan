//
//  ActiveGrass.h
//  SeaLink
//
//  Created by Fuatnow on 15-5-3.
//
//

#ifndef __SeaLink__ActiveGrass__
#define __SeaLink__ActiveGrass__
#include "BasicLayer.h"
class ActiveGrass : public Sprite
{
public:
    static ActiveGrass* create(const char* filename,float bendFactor = 0.2f);
    bool init(const char* filename,float bendFactor);
    void initData(float bendFactor);
    void initView();
    void update(float dt);
private:
    int		m_nTimeUniformLocation;
    int     m_bendFactorUniformLocation;
    float	m_fTime;
    float   m_bendFactor;
};

#endif /* defined(__SeaLink__ActiveGrass__) */
