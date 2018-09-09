#ifndef FLOOR_H
#define FLOOR_H
#include"point.h"
#include"ray.h"
#include"material.h"
#include"intersection.h"
class Floor :public material
{

public:
	Floor(double, double, double, double, int, double, color);
	intersection intersect(ray);
};
#endif