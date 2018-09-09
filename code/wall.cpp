#include"wall.h"
#include"intersection.h"
#include<cmath>
using namespace std;

wall::wall(double ka, double kd, double ks, double kn, int nn, double R, color co) : material(ka, kd, ks, kn, nn, R, co)
{
	
}

intersection wall::intersect(ray Ray)
{
	intersection result;

	if (Ray.geto().getx() > -500 && Ray.getd().getx() < 0)
	{
		point r = Ray.getray(-(Ray.geto().getx() + 500) / Ray.getd().getx());
		if (r.getz() > 0)
		{
			result.distance = -(Ray.geto().getx() + 500) / Ray.getd().getx();
			if (result.distance >= 100000)
				result.distance = 99999;
			result.position = r;
			result.normal = point(1, 0, 0);
			result.ob = this;
		}
		
	}

	return result;
}

intersection wall::intersect2(ray Ray)
{
	intersection result;

	if (Ray.geto().getx() < 600 && Ray.getd().getx() > 0)
	{
		point r = Ray.getray((-Ray.geto().getx() + 600) / Ray.getd().getx());
		if (r.getz() > 0)
		{
			result.distance = (-Ray.geto().getx() + 600) / Ray.getd().getx();
			if (result.distance >= 100000)
				result.distance = 99999;
			result.position = r;
			result.normal = point(1, 0, 0);
			result.ob = this;
		}

	}

	return result;
}