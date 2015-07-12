#ifndef HILBERT3d_GL_H_INCLUDED
#define HILBERT3d_GL_H_INCLUDED

class hilbert3d_gl{
private:	
	GLuint dlist;
	double step;
public:
	hilbert3d_gl(int order,double size):dlist(0)
	{
		make_display_list(order,size);
	}

	~hilbert3d_gl()
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
		int s=(1<<order)-1;
		
		step=size/s;
		
		dlist=glGenLists(1);		
		glNewList(dlist,GL_COMPILE);
	
		rec(order);
	
		glEndList();	
	}
	
	void rec(int order)
	{	
		if(order==0) return;
		
		int size=1<<(order-1);
		
		glPushMatrix();
		glRotated( 90.0,1,0,0);
		glRotated(-90.0,0,0,1);
		rec(order-1);
		glPopMatrix();

		glTranslated(0,0,-step*(size-1));	
		glBegin(GL_LINES);
		glVertex3d(0,0,0);	
		glVertex3d(0,0,-step);
		glEnd();
		glTranslated(0,0,-step);	
	
		glPushMatrix();
		glRotated( 90.0,0,0,1);
		glRotated(-90.0,1,0,0);
		rec(order-1);
		glPopMatrix();
		
		glTranslated(0,step*(size-1),0);	
		glBegin(GL_LINES);
		glVertex3d(0,0,0);	
		glVertex3d(0,step,0);	
		glEnd();
		glTranslated(0,step,0);	
	
		glPushMatrix();
		glRotated( 90.0,0,0,1);
		glRotated(-90.0,1,0,0);
		rec(order-1);
		glPopMatrix();
	
		glTranslated(0,step*(size-1),0);
		glBegin(GL_LINES);
		glVertex3d(0,0,0);	
		glVertex3d(0,0,step);	
		glEnd();
		glTranslated(0,0,step);
	
		glPushMatrix();
		glRotated(-180.0,1,0,0);
		rec(order-1);
		glPopMatrix();
	
	
		glTranslated(step*(size-1),0,0);
		glBegin(GL_LINES);
		glVertex3d(0,0,0);	
		glVertex3d(step,0,0);	
		glEnd();
		glTranslated(step,0,0);
	
	
		glPushMatrix();
		glRotated(-180.0,1,0,0);
		rec(order-1);
		glPopMatrix();
	
		glTranslated(step*(size-1),0,0);
		glBegin(GL_LINES);
		glVertex3d(0,0,0);	
		glVertex3d(0,0,-step);	
		glEnd();
		glTranslated(0,0,-step);
	
	
		glPushMatrix();
		glRotated( 90.0,0,1,0);
		glRotated(-90.0,0,0,1);
		rec(order-1);
		glPopMatrix();
	
	
		glTranslated(0,-step*(size-1),0);
		glBegin(GL_LINES);
		glVertex3d(0,0,0);	
		glVertex3d(0,-step,0);	
		glEnd();
		glTranslated(0,-step,0);
	
		glPushMatrix();
		glRotated( 90.0,0,1,0);
		glRotated(-90.0,0,0,1);
		rec(order-1);
		glPopMatrix();
	
		glTranslated(0,-step*(size-1),0);
		glBegin(GL_LINES);
		glVertex3d(0,0,0);	
		glVertex3d(0,0,step);
		glEnd();
		glTranslated(0,0,step);
	
		glPushMatrix();
		glRotated(90.0,1,0,0);
		glRotated(90.0,0,0,1);
		rec(order-1);
		glPopMatrix();       
	}
};

#endif
