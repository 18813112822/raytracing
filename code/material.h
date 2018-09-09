#ifndef MATERIAL
#define MATERIAL
#include"ray.h"
#include"color.h"
#include"point.h"
#include"ray.h"
class material
{
public:
	double Ka;  //环境光反射系数
	double Kd;	//漫反射系数
	double Ks;	//镜面反射系数
	double Kn;  //折射系数
	int n; //反射指数
	double rn; //折射率
	color Color;
	material();
	material(double, double, double,double, int, double rn, color c);
};
#endif
