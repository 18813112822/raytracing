#include"sphere.h"
#include"intersection.h"
#include<cmath>
#include<iostream>
using namespace std;

sphere::sphere(double ka, double kd, double ks, double kn, int nn, double R, color co, point p, double rr): material(ka, kd, ks, kn, nn, R, co)
{
	c = p;
	r = rr;
	sqr_r = r * r;
}

intersection sphere::intersect(ray Ray)
{
	point v = Ray.geto().subtract(c);
	double a0 = v.sqrlength() - sqr_r;
	double DdotV = Ray.getd().dot(v);
	intersection result;
	if (DdotV < 0)
	{
		double discr = DdotV * DdotV - a0;
		if (discr >= 0)
		{
			if(a0 >1000)
				result.distance = -DdotV - sqrt(discr);
			else
			{
				//cout << "right" << endl;
				result.distance = -DdotV *2;
			//	cout << result.distance << endl;
			//	system("pause");
			}
			
			result.position = Ray.getray(result.distance);
			result.normal = result.position.subtract(c).normalize();
				
			result.ob = this;
		}
	}
	return result;
}