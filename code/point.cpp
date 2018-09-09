#include"point.h"
#include<cmath>
#include<iostream>
using namespace std;

point::point()
{
	x = 0;
	y = 0;
	z = 0;
}
point::point(double a, double b, double c)
{
	x = a;
	y = b;
	z = c;
}

double point::getx()
{
	return x;
}

double point::gety()
{
	return y;
}

double point::getz()
{
	return z;
}

double point::length()
{
	return sqrt(x * x + y * y + z * z);
}

double point::sqrlength()
{
	return x * x + y * y + z * z;
}

point point::normalize()
{
	double l = length();
	return point(x / l, y / l, z / l);
}

point point::negate()
{
	return point(-1 * x, -1 * y, -1 * z);
}

point point::add(point v)
{
	return point(x + v.x, y + v.y, z + v.z);
}

point point::subtract(point v)
{
	return point(x - v.x, y - v.y, z - v.z); 
}

point point::multiply(double t)
{
	return point(x * t, y * t, z * t);
}

point point::divide(double t)
{
	return point(x / t, y / t, z / t);
}

double point::dot(point v)
{
	return x * v.x + y * v.y + z * v.z;
}

point point::cross(point v)
{
	return point(-z * v.y + y * v.z, z * v.x - x * v.z, -y * v.x + x * v.y);
}

void point::print()
{
	cout << x << ' ' << y << " " << z;
}
