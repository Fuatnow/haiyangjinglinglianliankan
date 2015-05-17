#include "Dogleg.h"

const char* dogvert="\
attribute vec4 a_position; \
attribute vec4 a_color; \
attribute vec2 TextureCoord;\
varying vec2 v_texCoord;\
varying vec4 v_fragmentColor;\
uniform vec2 ldpos;\
uniform float val;\
uniform vec2 resolution;\
void main(void) { \
	v_texCoord = TextureCoord;\
	v_fragmentColor=a_color;\
	vec4 centerpos=vec4(ldpos.x+resolution.x/2.0,ldpos.y+resolution.y/2.0,0.0,0.0);\
	float d=distance(centerpos,vec4(a_position.x,a_position.y,0.0,0.0));\
	float d0=d/(resolution.x/2.0);\
	float w=0.1+(1-(d0-0.2)*(d0-0.2))*val;\
	float c=cos(w);\
	float s=sin(w);\
	mat4 m1=mat4(c,s,0.0,0.0,-s,c,0.0,0.0,0.0,0.0,1.0,0.0,\
		(1.0-c)*centerpos.x+s*centerpos.y,-s*centerpos.x+(1.0-c)*centerpos.y,0.0,1.0);\
	vec4 vrs=CC_MVPMatrix *m1* a_position;\
	gl_Position =vrs;\
}";
const char* dogfrag="\
varying vec4 v_fragmentColor;\
varying vec2 v_texCoord;\
void main()\
{\
	vec4 color=texture2D(CC_Texture0,v_texCoord);\
	gl_FragColor =color;\
}";
void Dogleg::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(Dogleg::onDraw, this);
	renderer->addCommand(&_customCommand);
}
void Dogleg::onDraw()
{    
	glEnable(GL_BLEND);
	BlendFunc bf=BlendFunc::ALPHA_NON_PREMULTIPLIED;
	glBlendFunc(bf.src, bf.dst);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	pg->use();
	pg->setUniformsForBuiltins();
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*((meshrow+1)*(meshcol+1)),Vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*iarr.size(),iarr.data(),GL_STATIC_DRAW);

	GLint _positionLocation = glGetAttribLocation(pg->getProgram(), "a_position");
	GLint _colorLocation = glGetAttribLocation(pg->getProgram(), "a_color");
	GLint _textureLocation = glGetAttribLocation(pg->getProgram(), "TextureCoord");
	GLint _textureUniform = glGetUniformLocation(pg->getProgram(), "CC_Texture0");
	GLint _ldpos = glGetUniformLocation(pg->getProgram(), "ldpos");
	GLint val = glGetUniformLocation(pg->getProgram(), "val");
	GLint resolution = glGetUniformLocation(pg->getProgram(), "resolution");
	glUniform1f(val,angle);
	static int direction=1;
	angle+=direction*speed;
	if(angle>5||angle<-5)
	{
		angle-=direction*speed; 
		direction=-direction;
	}
	glUniform2f(resolution,texSize.width,texSize.height);
	glUniform2f(_ldpos,Vertices[0].Position[0],Vertices[0].Position[1]);
	glEnableVertexAttribArray(_positionLocation);
	glEnableVertexAttribArray(_colorLocation);
	glEnableVertexAttribArray(_textureLocation);

	glVertexAttribPointer(_positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));

	glVertexAttribPointer(_colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid*)offsetof(Vertex, Color));

	glVertexAttribPointer(_textureLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, TexCoord));
	GL::bindTexture2DN(0, texture2d->getName());
	glUniform1i(_textureUniform, 0);
	glDrawElements(GL_TRIANGLE_STRIP,iarr.size(), GL_UNSIGNED_SHORT, 0);
	glDisable(GL_DEPTH_TEST);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Dogleg::CreateMesh(int meshrow,int meshcol,Texture2D* tex,std::vector<GLushort> &iarr)
{
	int xgap=tex->getContentSize().width/meshcol;
	int ygap=tex->getContentSize().height/meshrow;
	for(int i=0;i<=meshrow;++i)
		for(int j=0;j<=meshcol;++j)
		{
			Vertices[i*(meshcol+1)+j].Position[0]=j*xgap;
			Vertices[i*(meshcol+1)+j].Position[1]=i*ygap;
			Vertices[i*(meshcol+1)+j].Position[2]=0;
			Vertices[i*(meshcol+1)+j].Color[0]=CCRANDOM_0_1();
			Vertices[i*(meshcol+1)+j].Color[1]=CCRANDOM_0_1();
			Vertices[i*(meshcol+1)+j].Color[2]=1;
			Vertices[i*(meshcol+1)+j].Color[3]=1;
			Vertices[i*(meshcol+1)+j].TexCoord[0]=((float)j)/meshcol;
			Vertices[i*(meshcol+1)+j].TexCoord[1]=1-((float)i)/meshrow;
		}
#define  up 1
#define  rd 2
#define  ld 3
		int direction=0;
		GLushort curidx=0;  bool isover=0; int xdir=rd;
		while(1)
		{
			if(isover) 
				break;
			switch (direction)
			{
			case 0:
				iarr.push_back(curidx);
				direction=up;
				break;
			case up:
				{
					curidx=curidx+meshcol+1;
					if(curidx<=(meshrow+1)*(meshcol+1)-1)
					{
						iarr.push_back(curidx);
						direction=xdir;
					}
					else
					{
						isover=1;
					}
					break;
				}
			case rd:
				{
					int currow=curidx/(meshcol+1)+1;
					int rowmax=currow*(meshcol+1)-1;
					curidx+=1;
					if(curidx<=rowmax)
					{
						curidx-=(meshcol+1);
						iarr.push_back(curidx);
						direction=up;
					}
					else 
					{
						direction=up;
						xdir=ld;
						curidx-=1;
						if(curidx!=(meshrow+1)*(meshcol+1)-1)
							iarr.pop_back();
					}
					break;
				}
			case ld:
				{
					int currow=curidx/(meshcol+1)+1;
					int rowmin=currow*(meshcol+1)-meshcol-1;
					curidx-=1;
					if(curidx>=rowmin)
					{
						curidx-=(meshcol+1);
						iarr.push_back(curidx);
						direction=up;
					}
					else
					{
						direction=up;
						xdir=rd;
						curidx+=1;
						if(curidx!=(meshrow+1)*(meshcol+1)-1-meshcol)
							iarr.pop_back();
					}
					break;
				}
			}
		} 
}
Dogleg::~Dogleg()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	delete[] Vertices;
	delete pg;
}
void Dogleg::setPosition(Vec2 pos)
{
	for(int i=0;i<=meshrow;++i)
		for(int j=0;j<=meshcol;++j)
		{
			Vertices[i*(meshcol+1)+j].Position[0]+=pos.x;
			Vertices[i*(meshcol+1)+j].Position[1]+=pos.y;
		}
}
bool Dogleg::init(char* name)
{
	glGenBuffers( 1, &vertexBuffer );
	glGenBuffers( 1, &indexBuffer );
	meshrow=50;
	meshcol=50;
	angle=1;
	speed=0.1;
	Vertices=new Vertex[(meshrow+1)*(meshcol+1)];
	texture2d=TextureCache::getInstance()->textureForKey(name);
	if(texture2d==0)
	{
		texture2d=TextureCache::getInstance()->addImage(name);
	}
	pg=new GLProgram();
	pg->initWithVertexShaderByteArray(dogvert,dogfrag);
	pg->link();
	pg->updateUniforms();
	texSize=texture2d->getContentSize();
	CreateMesh(meshrow,meshcol,texture2d,iarr);
	return true;
}