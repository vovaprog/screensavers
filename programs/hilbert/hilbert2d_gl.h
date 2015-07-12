#ifndef HILBERT2D_GL_H_INCLUDED
#define HILBERT2D_GL_H_INCLUDED

#include <hilbert2d.h>

class hilbert2d_gl{
	GLuint dlist;
public:
	hilbert2d_gl(int order,double size):dlist(0)
	{
		make_display_list(order,size);
	}

	~hilbert2d_gl()
	{
		if(dlist!=0) glDeleteLists(dlist,1);
	}
	
	void draw()
	{
		glCallList(dlist);
	}
	
private:
	void make_display_list(int order,double size)
	{		
		hilbert2d h(order);
		
		double cur_x=0.0,cur_y=0.0;		
		double step=size/(1<<order);
		double dx,dy;
		int d;

		dlist=glGenLists(1);		
		glNewList(dlist,GL_COMPILE);		
		glBegin(GL_LINES);
				
		for(unsigned int i=0;i<h.v.size()-1;i++)
		{
			hilbert_point hp0=h.v[i];
			hilbert_point hp1=h.v[i+1];
			
			d=hp1.i-hp0.i;
			dx=d*step;
			
			d=hp1.j-hp0.j;
			dy=d*step;
			
			glVertex3d(cur_x,cur_y,0);
			
			cur_x+=dx;
			cur_y+=dy;
			
			glVertex3d(cur_x,cur_y,0);
		}	
		
		glEnd();
		glEndList();
	}
};

#endif


