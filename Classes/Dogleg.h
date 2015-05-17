#ifndef __DOGLEG_H__
#define __DOGLEG_H__
#include "cocos2d.h"
USING_NS_CC;

typedef struct {
	float Position[3];
	float Color[4];
	float TexCoord[2];
} Vertex;

class Dogleg:public Node
{
public:
	~Dogleg();
	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
    static Dogleg* create(char* name)
	{
       Dogleg *p=new Dogleg;
	   p->init(name);
	   p->autorelease();
	   return p;
	};
	bool init(char* name);
	void onDraw();
	void setPosition(Vec2 pos);
	GLProgram * pg;
	std::vector<GLushort> iarr;
	CustomCommand _customCommand;
	void CreateMesh(int meshrow,int meshcol,Texture2D* tex,std::vector<GLushort> &iarr);
	Vertex *Vertices;
	Size texSize;
	Texture2D *texture2d;
	float angle,speed;
	int meshrow;
	int meshcol;
	GLuint vertexBuffer;
	GLuint indexBuffer;

};

#endif