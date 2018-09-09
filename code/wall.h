#ifndef WALL_H
#define WALL_H
#include"point.h"
#include"ray.h"
#include"material.h"
#include"intersection.h"
class wall :public material
{
public:
	point A;
	point B;
	wall(double, double, double, double, int, double, color);
	intersection intersect(ray);
	intersection intersect2(ray);
};
#endif