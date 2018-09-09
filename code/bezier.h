#ifndef BEZIER_H
#define BEZIER_H
#include"point.h"
#include"ray.h"
#include"material.h"
#include"intersection.h"
class bezier :public material
{
public:
	point P[3][3];
	bezier(double, double, double, double, int, double, color);
	point getpoint(double u, double v);
	point dpu(double u, double v);
	point dpv(double u, double v);
	double getoc(ray Ray, point A0, point B0);
	intersection intersect(ray);
	bool solve(point& P, double u0, double v0, double u1, double v1, ray Ray);
	void getcuboid(point& A0, point& B0, double u0, double v0, double u1, double v1);
};
#endif
