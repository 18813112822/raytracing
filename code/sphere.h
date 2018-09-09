#ifndef SPHERE_H
#define SPHERE_H
#include"point.h"
#include"ray.h"
#include"material.h"
#include"intersection.h"
class sphere:public material
{
private:
	point c;
	double r;
	double sqr_r;
public:
	sphere(double, double, double, double, int, double,color,  point, double);
	intersection intersect(ray);
};
#endif
