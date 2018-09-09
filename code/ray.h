#ifndef RAY_H
#define RAY_H
#include"point.h"
class ray
{
private:
	point o;//发射点
	point d;//方向向量
public:
	ray(point, point);
	point getray(int t);
	point geto();
	point getd();
};
#endif