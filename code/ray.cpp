#include"ray.h"
using namespace std;
ray::ray(point a, point b)
{
	o = a;
	d = b;
}

point ray::getray(int t)
{
	return o.add(d.multiply(t));
}

point ray::geto()
{
	return o;
}

point ray::getd()
{
	return d;
}