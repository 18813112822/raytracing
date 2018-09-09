#include<iostream>
#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<cmath>
#include"point.h"
#include"color.h"
#include"ray.h"
#include"sphere.h"
#include"intersection.h"
#include"material.h"
#include"floor.h"
#include"wall.h"
#include"bezier.h"
using namespace cv;
using namespace std;

point camera(250, 300, 300);
color white(1, 1, 1);
color red(1, 0, 0);
color green(0.2, 0.2, 1);
color blue(0, 1, 0);

Floor Fl(0.4, 0.3, 0, 0, 12, 0, white);
wall w1(0, 0, 0.9, 0, 12, 0, white);
wall w2(0.4, 0.3, 0, 0, 12, 0, green);
bezier B(0.4, 0.5, 0.4, 0, 12, 0, red);

point o1(-200, 200, 200);
point o2(-200, 450, 200);
double r1 = 100;
sphere ball1(0.4, 0.5, 0.4, 0, 12, 0, red, o1, r1); //红色不透明球
sphere ball2(0, 0, 0.3, 0.7, 12, 1.5, white, o2, r1);//透明球
point Light = point(500, 250, 500); //点光源

int min(int a, int b)
{
	if (a > b)
		return b;
	else
		return a;
}



bool shade(intersection p)
{
	point L = Light.subtract(p.position).normalize();
	ray Ray(p.position, L);
	intersection result1 = ball1.intersect(Ray);
	intersection result2 = ball2.intersect(Ray);
	if (result1.distance < 100000 || result2.distance < 100000)
		return true;
	return false;
}


void RayTracer(ray Ray, int Depth, color &Color, color Ip)
{
	intersection result1 = ball1.intersect(Ray);
	intersection result2 = Fl.intersect(Ray);
	intersection result3 = ball2.intersect(Ray);
	intersection result4 = w1.intersect(Ray);

	if (result1.distance < 100000 || result2.distance < 100000 || result3.distance < 100000 || result4.distance < 100000)
	{
		intersection P = result1;
		if (result1.distance < result2.distance && result1.distance < result3.distance && result1.distance < result4.distance)
			P = result1;
		else
			if (result2.distance < result1.distance && result2.distance < result3.distance  && result2.distance < result4.distance)
				P = result2;
			else
				if (result3.distance < result1.distance && result3.distance < result2.distance  && result3.distance < result4.distance)
					P = result3;
				else
					P = result4;
		if (!shade(P))
		{
			point L = Light.subtract(P.position).normalize();
			double NdotL = P.normal.dot(L);
			point H = Light.subtract(Ray.getd()).normalize();
			double NdotH = P.normal.dot(H);
			double Id, Is;
			if (NdotL > 0)
				Id = P.ob->Kd*NdotL;
			else
				Id = 0;
			if (NdotH > 0)
				Is = P.ob->Ks*pow(NdotH, P.ob->n);
			else
				Is = 0;
			double I = Id + Is + P.ob->Ka;
			if (I >= 1)
				I = 1;
			Color.r += Ip.r*I*P.ob->Color.r;
			Color.b += Ip.b*I*P.ob->Color.b;
			Color.g += Ip.g*I*P.ob->Color.g;
			if (result2.distance < 100000 && result2.distance == P.distance)
			{
				int x = floor(P.position.getx() * 0.01) + floor(P.position.gety() * 0.01);
				if (abs( x % 2) < 1)
				{
					Color.r = 0;
					Color.g = 0;
					Color.b = 0;
				}
				
			}
		}

		if (Depth > 1)
		{  // 递归终止：一定次数.
		   //反射
			if (P.ob->Ks > 0)
			{
				point dr;	//反射光线方向向量
				dr = Ray.getd().add(P.normal.multiply(2 * P.normal.dot(Ray.getd().negate())));
				ray ReflectedRay(P.position.add(dr.multiply(3)), dr);
				color Ir(0, 0, 0);
				color RefColor(0, 0, 0);
				Ir.r = Ip.r * P.ob->Ks * P.ob->Color.r;
				Ir.b = Ip.b * P.ob->Ks * P.ob->Color.b;
				Ir.g = Ip.g * P.ob->Ks * P.ob->Color.g;
				RayTracer(ReflectedRay, Depth - 1, RefColor, Ir);
				Color.r += RefColor.r;
				Color.b += RefColor.b;
				Color.g += RefColor.g;
			}

			//折射
			if (P.ob->Kn > 0)
			{
				point dr; //折射光线方向向量
				if (Ray.geto().subtract(o2).length() <= r1 + 10)
				{
					if (1 - (1 - pow(P.normal.dot(Ray.getd().negate()), 2)) * pow(P.ob->rn, 2) > 0)
						dr = P.normal.negate().multiply(P.normal.negate().dot(Ray.getd().negate()) * P.ob->rn - sqrt(1 - (1 - pow(P.normal.negate().dot(Ray.getd().negate()), 2)) * pow(P.ob->rn, 2))).add(Ray.getd().multiply(P.ob->rn));
					else
						dr = Ray.getd().add(P.normal.negate().multiply(2 * P.normal.negate().dot(Ray.getd().negate())));
				}
				else
					dr = P.normal.multiply(P.normal.dot(Ray.getd().negate()) / P.ob->rn - sqrt(1 - (1 - pow(P.normal.dot(Ray.getd().negate()), 2)) / pow(P.ob->rn, 2))).add(Ray.getd().multiply(1 / P.ob->rn));

				ray KnRay(P.position, dr);
				/*cout << Depth << endl;
				cout << P.position.getx() << " " << P.position.gety() << " " << P.position.getz() << endl;
				cout << dr.getx() << " " << dr.gety() << " " << dr.getz() << endl;
				cout << result1.distance << " " << result2.distance << " " << result3.distance << endl;
				system("pause");*/
				color Ir(0, 0, 0);
				color KnColor(0, 0, 0);
				Ir.r = Ip.r * P.ob->Kn * P.ob->Color.r;
				Ir.b = Ip.b * P.ob->Kn * P.ob->Color.b;
				Ir.g = Ip.g * P.ob->Kn * P.ob->Color.g;
				RayTracer(KnRay, Depth - 1, KnColor, Ir);
				Color.r += KnColor.r;
				Color.b += KnColor.b;
				Color.g += KnColor.g;
			}

		}
	}
}


int main()
{

	Mat img(500, 500, CV_8UC3, Scalar(200, 200, 200));
	for (int x = 0; x < 500; x++)
		for (int y = 0; y < 500; y++)
		{
			color Color(0, 0, 0);
			point p(0, x, y);
			ray Ray(camera, p.subtract(camera).normalize());
			int Depth = 5;
			RayTracer(Ray, Depth, Color, color(255, 255, 255));
			//	if(Color.r > 0)
			//	cout << Color.r << " " << Color.g << " " << Color.b << endl;
			if (Color.r > 255)
				Color.r = 255;
			if (Color.b > 255)
				Color.b = 255;
			if (Color.g > 255)
				Color.g = 255;
			img.at<Vec3b>(x, y)[0] = Color.b;
			img.at<Vec3b>(x, y)[1] = Color.g;
			img.at<Vec3b>(x, y)[2] = Color.r;
		}
	imwrite("3.jpg", img);
	system("pause");
	return 0;

}