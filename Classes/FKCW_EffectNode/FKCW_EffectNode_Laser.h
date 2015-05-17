//*************************************************************************
//	��������:	2014-11-17
//	�ļ�����:	FKCW_EffectNode_Laser.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_EffectNode_Mesh.h"
#include "FKCW_EffectNode_IndexVBO.h"
#include "FKCW_EffectNode_GLProgramWithUnifos.h"
//-------------------------------------------------------------------------
class FKCW_EffecrNode_HalfLaser : public Sprite
{
public:
	FKCW_EffecrNode_HalfLaser();
	virtual~FKCW_EffecrNode_HalfLaser();

	void setBlendFunc(GLenum src,GLenum dst);
	void updateStartAndEnd();
	void setStart(const CCPoint&start);
	void setEnd(const CCPoint&end);
	bool init();
	void draw();
	void update(float dt);
	void setPorN(float value){m_PorN=value;}
public:
	FKCW_EffectNode_GLProgramWithUnifos*	m_program;
	Texture2D*							m_maskTex;
	Texture2D*							m_maskTex2;
	Texture2D*							m_noiseTex;
	Texture2D*							m_noiseTex2;
	float									m_highlight;
	float									m_time;
	float									m_PorN;
	Vec2									m_start;
	Vec2									m_end;
	float									m_kLightColor;
	float									m_kGlowColor;
	float									m_noiseScale;
};
//-------------------------------------------------------------------------
class FKCW_EffecrNode_Laser:public Node
{
public:
	FKCW_EffecrNode_Laser();

	bool init();
	void setStart(const CCPoint&start);
	void setEnd(const CCPoint&end);
private:
	FKCW_EffecrNode_HalfLaser*	m_halfLaser1;
	FKCW_EffecrNode_HalfLaser*	m_halfLaser2;
	FKCW_EffecrNode_HalfLaser*	m_slimHalfLaser1;
	FKCW_EffecrNode_HalfLaser*	m_slimHalfLaser2;
	Vec2						m_start;
	Vec2						m_end;
};