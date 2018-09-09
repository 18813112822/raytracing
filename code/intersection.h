#ifndef INTERSECTION_H
#define INTERSECTION_H
#include"point.h"
#include"material.h"
class intersection
{
public:
	material *ob;
	double distance;
	point position;
	point normal;

	intersection() {
		distance = 100000;
		position = point(0, 0, 0);
		normal = point(0, 0, 0);
	}
};
#endif