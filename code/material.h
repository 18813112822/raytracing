#ifndef MATERIAL
#define MATERIAL
#include"ray.h"
#include"color.h"
#include"point.h"
#include"ray.h"
class material
{
public:
	double Ka;  //�����ⷴ��ϵ��
	double Kd;	//������ϵ��
	double Ks;	//���淴��ϵ��
	double Kn;  //����ϵ��
	int n; //����ָ��
	double rn; //������
	color Color;
	material();
	material(double, double, double,double, int, double rn, color c);
};
#endif
