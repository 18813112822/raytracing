#ifndef POINT_H
#define POINT_H
class point
{
private:
	double x;
	double y;
	double z;
public:
	point(double, double,double);
	point();
	double getx();
	double gety();
	double getz();
	double length();
	double sqrlength();
	point normalize();
	point negate();
	point add(point);
	point subtract(point);
	point multiply(double);
	point divide(double);
	double dot(point);
	point cross(point);
	void print();
};
#endif