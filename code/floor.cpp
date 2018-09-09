#include"floor.h"
#include"intersection.h"
#include<cmath>
using namespace std;

Floor::Floor(double ka, double kd, double ks, double kn, int nn, double R, color co) : material(ka, kd, ks, kn, nn, R, co)
{
}

intersection Floor::intersect(ray Ray)
{
	intersection result;
	if (Ray.getd().getz() < 0)
	{
		point z(0, 0, -1);
		result.distance = Ray.geto().getz() / (Ray.getd().dot(z));
		if (result.distance >= 100000)
			result.distance = 99999;
		result.position = Ray.getray(result.distance);
		result.normal = point(0, 0, 1);
		result.ob = this;
	}
	
	return result;
}