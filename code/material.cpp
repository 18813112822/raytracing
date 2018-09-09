#include"material.h"
using namespace std;

material::material(double ka, double kd, double ks, double kn, int nn, double r, color c)
{
	Ka = ka;
	Kd = kd;
	Ks = ks;
	Kn = kn;
	n = nn;
	rn = r;
	Color = c;
}

material::material()
{
	
}

