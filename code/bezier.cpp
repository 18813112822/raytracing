#include"bezier.h"
#include"floor.h"
#include"intersection.h"
#include<cmath>
#include<algorithm>
#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
using namespace std;
using namespace cv;


class muv
{
public:
	double u0;
	double u1;
	double v0; 
	double v1;
	double m;
	muv(double u00, double v00, double u11, double v11, double mm)
	{
		u0 = u00;
		u1 = u11;
		v0 = v00;
		v1 = v11;
		m = mm;
	}
	muv()
	{

	}
};

bool cmp(const muv& A, const muv& B)
{
	return A.m < B.m;
}

double Min(double a, double b, double c, double d)
{
	double m = a;
	if (m > b)
		m = b;
	if (m > c)
		m = c;
	if (m > d)
		m = d;
	return m;
}

double Max(double a, double b, double c, double d)
{
	double m = a;
	if (m < b)
		m = b;
	if (m < c)
		m = c;
	if (m < d)
		m = d;
	return m;
}

double B(int i, double u)
{
	if (i == 0)
		return (1 - u)*(1 - u);
	if (i == 1)
		return 2*u*(1 - u);
	if (i == 2)
		return u*u;
}

double dB(int i, double u)
{
	if (i == 0)
		return 2*(u - 1);
	if (i == 1)
		return 2 - 4*u;
	if (i == 2)
		return 2*u;
}

point bezier::dpu(double u, double v)
{
	point result(0, 0, 0);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			result = result.add(P[i][j].multiply(dB(i, u)*B(j, v)));
		}
	return result;
}

point bezier::dpv(double u, double v)
{
	point result(0, 0, 0);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			result = result.add(P[i][j].multiply(B(i, u)*dB(j, v)));
		}
	return result;
}

bezier::bezier(double ka, double kd, double ks, double kn, int nn, double R, color co) : material(ka, kd, ks, kn, nn, R, co)
{
	P[0][0] = point(-200, 200, 200);
	P[0][2] = point(-400, 200, 200);
	P[2][0] = point(-200, 400, 200);
	P[2][2] = point(-400, 400, 200);
	P[0][1] = point(-300, 200, 220);
	P[1][0] = point(-200, 300, 300);
	P[2][1] = point(-300, 400, 300);
	P[1][2] = point(-400, 300, 300);
	P[1][1] = point(-300, 300, 400);
}

void bezier::getcuboid(point& A0, point& B0, double u0, double v0, double u1, double v1)
{
	if (u1 - u0 < 0.03)
	{
		double x0 = Min(getpoint(u0, v0).getx(), getpoint(u0, v1).getx(), getpoint(u1, v0).getx(), getpoint(u1, v1).getx());
		double y0 = Min(getpoint(u0, v0).gety(), getpoint(u0, v1).gety(), getpoint(u1, v0).gety(), getpoint(u1, v1).gety());
		double z0 = Min(getpoint(u0, v0).getz(), getpoint(u0, v1).getz(), getpoint(u1, v0).getz(), getpoint(u1, v1).getz());
		A0 = point(x0, y0, z0);
		double x1 = Max(getpoint(u0, v0).getx(), getpoint(u0, v1).getx(), getpoint(u1, v0).getx(), getpoint(u1, v1).getx());
		double y1 = Max(getpoint(u0, v0).gety(), getpoint(u0, v1).gety(), getpoint(u1, v0).gety(), getpoint(u1, v1).gety());
		double z1 = Max(getpoint(u0, v0).getz(), getpoint(u0, v1).getz(), getpoint(u1, v0).getz(), getpoint(u1, v1).getz());
		B0 = point(x1, y1, z1);
	}
	else
	{
		point A1(0, 0, 0), B1(0, 0, 0), A2(0, 0, 0), B2(0, 0, 0), A3(0, 0, 0), B3(0, 0, 0), A4(0, 0, 0), B4(0, 0, 0);
		getcuboid(A1, B1, u0, v0, (u1+u0)/2, (v1+v0)/2);
		getcuboid(A2, B2, (u1 + u0) / 2, v0, u1, (v1 + v0) / 2);
		getcuboid(A3, B3, u0, (v0+v1)/2, (u1+u0)/2, v1);
		getcuboid(A4, B4, (u1 + u0) / 2, (v0+v1)/2, u1, v1);
		double x0 = Min(A1.getx(), A2.getx(), A3.getx(), A4.getx());
		double y0 = Min(A1.gety(), A2.gety(), A3.gety(), A4.gety());
		double z0 = Min(A1.getz(), A2.getz(), A3.getz(), A4.getz());
		A0 = point(x0, y0, z0);
		double x1 = Max(B1.getx(), B2.getx(), B3.getx(), B4.getx());
		double y1 = Max(B1.gety(), B2.gety(), B3.gety(), B4.gety());
		double z1 = Max(B1.getz(), B2.getz(), B3.getz(), B4.getz());
		B0 = point(x1, y1, z1);
	}
}

point bezier::getpoint(double u, double v)
{
	point result(0, 0, 0);
	for(int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			result = result.add(P[i][j].multiply(B(i, u)*B(j, v)));
		}
	return result;
}

double bezier::getoc(ray Ray, point A0, point B0)
{
	
	double dx = 0, dy = 0, dz = 0;
	if (B0.getx() < Ray.geto().getx())
	{
		if (Ray.getd().getx() >= 0)
			dx = 0;
		else
			dx = -(Ray.geto().getx() - B0.getx()) / Ray.getd().getx();
	}
	else
		if (A0.getx() > Ray.geto().getx())
		{
			if (Ray.getd().getx() <= 0)
				dx = 0;
			else
				dx = (A0.getx() - Ray.geto().getx()) / Ray.getd().getx();
		}
		else
		{
			if (Ray.getd().getx() < 0)
				dx = -(Ray.geto().getx() - A0.getx()) / Ray.getd().getx();
			else
				if (Ray.getd().getx() > 0)
					dx = (B0.getx() - Ray.geto().getx()) / Ray.getd().getx();
				else
					dx = 0;
		}

	if (B0.gety() < Ray.geto().gety())
	{
		if (Ray.getd().gety() >= 0)
			dy = 0;
		else
			dy = -(Ray.geto().gety() - B0.gety()) / Ray.getd().gety();
	}
	else
		if (A0.gety() > Ray.geto().gety())
		{
			if (Ray.getd().gety() <= 0)
				dy = 0;
			else
				dy = (A0.gety() - Ray.geto().gety()) / Ray.getd().gety();
		}
		else
		{
			if (Ray.getd().gety() < 0)
				dy = -(Ray.geto().gety() - A0.gety()) / Ray.getd().gety();
			else
				if (Ray.getd().gety() > 0)
					dy = (B0.gety() - Ray.geto().gety()) / Ray.getd().gety();
				else
					dy = 0;
		}

	if (B0.getz() < Ray.geto().getz())
	{
		if (Ray.getd().getz() >= 0)
			dz = 0;
		else
			dz = -(Ray.geto().getz() - B0.getz()) / Ray.getd().getz();
	}
	else
		if (A0.getz() > Ray.geto().getz())
		{
			if (Ray.getd().getz() <= 0)
				dz = 0;
			else
				dz = (A0.getz() - Ray.geto().getz()) / Ray.getd().getz();
		}
		else
		{
			if (Ray.getd().getz() < 0)
				dz = -(Ray.geto().getz() - A0.getz()) / Ray.getd().getz();
			else
				if (Ray.getd().getz() > 0)
					dz = (B0.getz() - Ray.geto().getz()) / Ray.getd().getz();
				else
					dz = 0;
		}
	double m = dx;
	if (dy > m)
		m = dy;
	if (dz > m)
		m = dz;
	return m;
}

bool bezier::solve(point& P, double u0, double v0, double u1, double v1, ray Ray)
{
	point A0, B0;
	getcuboid(A0, B0, u0, v0, u1, v1);
//	cout << A0.getx() << " " << A0.gety() << " " << A0.getz() << endl;
	//cout << B0.getx() << " " << B0.gety() << " " << B0.getz() << endl;
	double m = getoc(Ray, A0, B0);
	if(m > 0)
	{
		if (Ray.getray(m).getx() > A0.getx() - 3 && Ray.getray(m).getx() < B0.getx() + 3 && Ray.getray(m).gety() > A0.gety() - 3 && Ray.getray(m).gety() < B0.gety() + 3 && Ray.getray(m).getz() > A0.getz() - 3 && Ray.getray(m).getz() < B0.getz() + 3)
		{
 			if (u1 - u0 < 0.3)
			{
				P = point(m, (u0 + u1) / 2, (v0 + v1) / 2);
				return true;
			}
			double m1, m2, m3, m4;
			point A1, B1, A2, B2, A3, B3, A4, B4;
			getcuboid(A1, B1, u0, v0, (u1 + u0) / 2, (v0 + v1) / 2);
			m1 = getoc(Ray, A1, B1);
			getcuboid(A2, B2, (u0+u1)/2, v0, u1, (v0 + v1) / 2);
			m2 = getoc(Ray, A2, B2);
			getcuboid(A3, B3, u0, (v0+v1)/2, (u1 + u0) / 2, v1);
			m3 = getoc(Ray, A3, B3);
			getcuboid(A4, B4, (u0+u1)/2, (v0+v1)/2, u1, v1);
			m4 = getoc(Ray, A4, B4);
			muv M[4];
			M[0] = muv(u0, v0, (u1 + u0) / 2, (v0 + v1) / 2, m1);
			M[1] = muv((u0 + u1) / 2, v0, u1, (v0 + v1) / 2, m2);
			M[2] = muv(u0, (v0 + v1) / 2, (u1 + u0) / 2, v1, m3);
			M[3] = muv((u0 + u1) / 2, (v0 + v1) / 2, u1, v1, m4);
			sort(M, M + 4, cmp);
			for(int i = 0; i < 4; i++)
				if (solve(P, M[i].u0, M[i].v0, M[i].u1, M[i].v1, Ray))
					return true;
		}
		else
		{
			return false;
		}
	}
}

intersection bezier::intersect(ray Ray)
{
	intersection result;
	point A0(0, 0, 0), B0(0, 0, 0);
	point P(0, 0, 0);
	double u0 = 0, v0 = 0, u1 = 1, v1 = 1;
	if (1)
	{
		
		bool flag = false;
	/*	double t = P.getx();
		double u = P.gety();
		double v = P.getz();
	*/
		double u = 0.5;
		double v = 0.5;
		double t = Ray.geto().subtract(getpoint(u, v)).length();
		Mat X(3, 1, CV_64FC1);
		Mat F(3, 1, CV_64FC1);
		Mat F1(3, 3, CV_64FC1);

		for (int i = 0; i < 10; i++)
		{
			//cout << t << " " << u << " " << v << endl;
			if (Ray.getray(t).subtract(getpoint(u, v)).length() < 1)
			{
				flag = true;
				break;
			}
			X.at<double>(0, 0) = t;
			X.at<double>(1, 0) = u;
			X.at<double>(2, 0) = v;

			F.at<double>(0, 0) = Ray.getray(t).subtract(getpoint(u, v)).getx();
			F.at<double>(1, 0) = Ray.getray(t).subtract(getpoint(u, v)).gety();
			F.at<double>(2, 0) = Ray.getray(t).subtract(getpoint(u, v)).getz();

			F1.at<double>(0, 0) = Ray.getd().getx();
			F1.at<double>(1, 0) = Ray.getd().gety();
			F1.at<double>(2, 0) = Ray.getd().getz();
			F1.at<double>(0, 1) = -dpu(u, v).getx();
			F1.at<double>(1, 1) = -dpu(u, v).gety();
			F1.at<double>(2, 1) = -dpu(u, v).getz();
			F1.at<double>(0, 2) = -dpv(u, v).getx();
			F1.at<double>(1, 2) = -dpv(u, v).gety();
			F1.at<double>(2, 2) = -dpv(u, v).getz();

			X = X - F1.inv() * F;
			t = X.at<double>(0, 0);
			u = X.at<double>(1, 0);
			v = X.at<double>(2, 0);
			if (t < 0 || u < 0 || u > 1 || v < 0 || v > 1)
				break;
		}
		if (flag)
		{
			result.position = Ray.getray(t);
			result.distance = t;
			//cout << t << endl;
			result.ob = this;
			result.normal = dpu(u, v).cross(dpv(u, v)).normalize();
			if (result.normal.dot(Ray.getd()) > 0)
				result.normal = result.normal.negate();
		}
	}
	return result;
}