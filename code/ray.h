#ifndef RAY_H
#define RAY_H
#include"point.h"
class ray
{
private:
	point o;//�����
	point d;//��������
public:
	ray(point, point);
	point getray(int t);
	point geto();
	point getd();
};
#endif