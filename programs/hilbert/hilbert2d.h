#ifndef HILBERT2D_H_INCLUDED
#define HILBERT2D_H_INCLUDED

#include <stdio.h>
#include <vector>

using namespace std;

struct hilbert_point{
	int i,j,k;
	hilbert_point(int i,int j):i(i),j(j){}
	hilbert_point(int i,int j,int k):i(i),j(j),k(k){}
};

class hilbert2d{
private:
	int cur_i,cur_j;
public:
	vector<hilbert_point> v;

	hilbert2d(int order)
	{
		curve(order);
	}	
	
private:
	void step(int dir)
	{
		v.push_back(hilbert_point(cur_i,cur_j));

		switch(dir & 3){
		case 0:cur_i++;break;
		case 1:cur_j++;break;
		case 2:cur_i--;break;
		case 3:cur_j--;break;
		}	
	}

	void hilbert_rec(int dir,int rot,int order)
	{
		if(order==0) return;
		
		dir+=rot;
		hilbert_rec(dir,-rot,order-1);
		step(dir);
		
		dir-=rot;
		hilbert_rec(dir,rot,order-1);
		step(dir);
		hilbert_rec(dir,rot,order-1);

		dir-=rot;
		step(dir);
		hilbert_rec(dir,-rot,order-1);
	}

public:
	vector<hilbert_point>& curve(int order)
	{
		cur_i=0;cur_j=0;
		v.clear();
		hilbert_rec(0,1,order);
		v.push_back(hilbert_point(cur_i,cur_j));
		return v;
	}
};

#endif
