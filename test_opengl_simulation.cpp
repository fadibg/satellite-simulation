#include<windows.h>
#include <gl/glut.h>
#include<gl/glext.h>
#include <ctime>
#include<cmath>
#include<math.h>
#include <iostream>
#include<filesystem>
#include<vector>
#include <fstream>
#include <map>
#include <time.h>
#include <string>
//#define _CRT_SECURE_NO_WARNINGS
//#include"texture.h"
//#include"3dtexture.h"
//#include<string.h>
//#include "../Library/loadpng.h"
//#include "../Library/process_image.h"
//#include "../Library/gl_texture.h"
using namespace std;
//#include "sat.cpp"

//using namespace SAT1;
//#include "3DTexture.h"
//#include "texture.h"
//#include "Camera.h"
//#include "Model_3DS.h"
//#include "Planet.cpp"
GLfloat d =0, retrograde = 0, angle = 0, view = 0;
bool pal = true,cru=false;
int loc = 0;
class Model {
private:
	class Face {
	public:
		int edge;
		int* vertices;
		int* texcoords;
		int normal;

		Face(int edge, int* vertices, int* texcoords, int normal = -1) {
			this->edge = edge;
			this->vertices = vertices;
			this->texcoords = texcoords;
			this->normal = normal;
		}
	};
	std::vector<float*> vertices;
	std::vector<float*> texcoords;
	std::vector<float*> normals;
	std::vector<Face> faces;
	GLuint list;
public:
	void load(const char* filename) {
		std::string line;
		std::vector<std::string> lines;
		std::ifstream in(filename);
		//ifstream in("Model/satellite_obj.obj");
		if (!in.is_open()) {
			printf("Cannot load model %s\n", filename);
			return;
		}
		while (!in.eof()) {
			std::getline(in, line);
			lines.push_back(line);
		}
		in.close();
		float a, b, c;
		for (std::string& line : lines) {
			if (line[0] == 'v') {
				if (line[1] == ' ') {
					sscanf(line.c_str(), "v %f %f %f", &a, &b, &c);
					vertices.push_back(new float[3]{ a, b, c });
				}
				else if (line[1] == 't') {
					sscanf(line.c_str(), "vt %f %f", &a, &b);
					texcoords.push_back(new float[2]{ a, b });
				}
				else {
					sscanf(line.c_str(), "vn %f %f %f", &a, &b, &c);
					normals.push_back(new float[3]{ a, b, c });
				}
			}
			else if (line[0] == 'f') {
				int v0, v1, v2, t0, t1, t2, n;
				sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n);
				int* v = new int[3]{ v0 - 1, v1 - 1, v2 - 1 };
				faces.push_back(Face(3, v, NULL, n - 1));
			}
		}
		list = glGenLists(1);
		glNewList(list, GL_COMPILE);
		for (Face& face : faces) {
			if (face.normal != -1)
				glNormal3fv(normals[face.normal]);
			else
				glDisable(GL_LIGHTING);
			glBegin(GL_POLYGON);
			for (int i = 0; i < face.edge; i++)
				glVertex3fv(vertices[face.vertices[i]]);
			glEnd();
			if (face.normal == -1)
				glEnable(GL_LIGHTING);
		}
		glEndList();
		//printf("Model: %s\n", filename);
		//printf("Vertices: %d\n", vertices.size());
		//printf("Texcoords: %d\n", texcoords.size());
		//printf("Normals: %d\n", normals.size());
		//printf("Faces: %d\n", faces.size());
		for (float* f : vertices)
			delete f;
		vertices.clear();
		for (float* f : texcoords)
			delete f;
		texcoords.clear();
		for (float* f : normals)
			delete f;
		normals.clear();
		faces.clear();
	}
	void draw() { glCallList(list); }
};
static Model model;

class  sat
{
public:
	sat();

	void sat1() {
	
		GLfloat Y[] = { 1,0.4,0 };
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		glScalef(1, 0.05, 1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Y);
		glutSolidSphere(0.5, 30, 30);
		glPopMatrix();
		glPopAttrib();

		glPushMatrix();
		glTranslatef(0, -0.5, 0);
		glScalef(0.4, 1, 0.4);
		glutSolidCube(1);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-0.5, -0.45, 0);
		glScalef(0.75, 0.01, 0.3);
		glutSolidCube(1);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.5, -0.45, 0);
		glScalef(0.75, 0.01, 0.3);
		glutSolidCube(1);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 0.1, 0);
		glScalef(0.01, 0.5, 0.01);
		glutSolidCube(1);
		glPopMatrix();
		
		glPopMatrix();
	
	}

	~sat();
	int mod = 0;// Ê÷⁄ «·„œ«— Ã€—«›Ì À«»  √Ê „ Õ—ﬂ 
	int r = 63781370;//‰’› ﬁÿ— «·√—÷ 
	float orbit = 0;// «·„œ«— 
	int h = 1000;// «·«— ›«⁄ ⁄‰ ”ÿÕ «·»Õ— 
	
	int v = 35764;// ”—⁄… «·ﬁ„— 
	int m = 1600;//ﬂ ·… «·ﬁ„— 
	float grav() {//«·Ã«–»Ì… ⁄‰œ «— ›«⁄ «·ﬁ„— «·’‰«⁄Ì 

		float g0 = (9.7803267714 * (1 + 0.00193185138639 * pow(sin(orbit), 2)) / sqrt(1 - 0.00669437999013 * pow(sin(orbit), 2)));//0orbit= q
		g01 = g0 * pow((r / (r + h)), 2);
		//cout << "r = " << r << "\n";
		//cout << "h = " << h << "\n";
		soso = h + r;
		final1 = r / soso;
		/*cout << "soso =" << soso << "\n";
		cout << "final = " << g0 * pow(final1, 2)<< "\n";*/
		//cout<<"gravvvvvvvv  = "<< r / (r + h) <<"\n";
		return g0 * pow(final1, 2);
	}
	float Centrifugal_force() {// ﬁÊ… «·ÿ—œ «·„—ﬂ“Ì… 
		float temp1 = (m * pow(v, 2)) / soso;
		return temp1;
	}
	float Motivational_Balance_System() {//‰Ÿ«„ «· Ê«“‰ »«·œÊ«›⁄ 
		//cout << "cf =" << Centrifugal_force() << "\n" << "grv =" << grav() << "\n";
		if (v > 0) {
			if (pal) {
				if (mod == 0) {// Õ«·… «·„œ«— «·Ã€—«›Ì «·À«» 
					if (abs(Centrifugal_force() - grav()) <1)
					{
						//cout << "###################################\n";
						//cout << "wade = " << Centrifugal_force() - grav() << "\n";
						//cout << v << "      00" << "\n";
						f = f + 0.00046388888888888888888888888888889;

						//open file for writing

					}
					else if (Centrifugal_force() > grav()) {//Œ—ÊÃ ⁄‰ „œ«—Â Ê ÷Ì«⁄Â ›Ì «·›÷«¡
						v -= 50;

						f = f + 0.00046388888888888888888888888888889;
						/*cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
						cout << v << "    11" << "\n";
						cout << h << "    11" << "\n";*/
					}
					else if (Centrifugal_force() < grav()) {// «’ œ«„ «·ﬁ„— »«·√—÷ 

						v += 50;
						f = f + 0.00046388888888888888888888888888889;
						/*cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
						cout << v << "    22" << "\n";*/
					}
					if (f > 360)
					{
						f = 0;

					}


				}
			}
			else {
				//cout << "h" << grav() << "\n";
				if (h <= 400) {
					cout << "the satellite is crashing \n";
					cru = true;

				}
				else if (abs(Centrifugal_force() - grav()) > 1&&grav()<Centrifugal_force())
				{
					h += 10;
					cout << "The satellite is out of orbit in space \n";
				}
				else if (abs(Centrifugal_force() - grav()) < 1)//abs (gc)
				{
					//cout << "###################################\n";
					//cout << "wade = " << Centrifugal_force() - grav() << "\n";
					//cout << v << "      00" << "\n";
					f = f + 0.00046388888888888888888888888888889;

					//open file for writing

				}
				else if (Centrifugal_force() > grav()) {//Œ—ÊÃ ⁄‰ „œ«—Â Ê ÷Ì«⁄Â ›Ì «·›÷«¡
					h +=10;

					f = f + 0.00046388888888888888888888888888889;
					//cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
					/*cout << v << "    11" << "\n";
					cout << h << "    11" << "\n";*/
				}
				else if (Centrifugal_force() < grav()) {// «’ œ«„ «·ﬁ„— »«·√—÷ 

					h-=10;
					f = f + 0.00046388888888888888888888888888889;
					/*cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
					cout << v << "    22" << "\n";*/
				}
				if (f > 360)
				{
					f = 0;

				}
			}
		}
		 if( mod ==1)//Õ«·… «·„œ—« «·„‰Œ›÷ √Ê «·„ Ê”ÿ √Ê «·⁄«·Ì 
		{
			 if (pal) {
				 if (abs(Centrifugal_force() - grav()) < 1)//abs (gc)
				 {
					 //cout << "###################################\n";
					 //cout << "wade = " << Centrifugal_force() - grav() << "\n";
					 //cout << v << "      00" << "\n";
					 f = f + (low_orbit_calculation() / 360);

					 //open file for writing

				 }
				 else if (Centrifugal_force() > grav()) {//Œ—ÊÃ ⁄‰ „œ«—Â Ê ÷Ì«⁄Â ›Ì «·›÷«¡
					 v-= 50;

					 f = f + (low_orbit_calculation() / 360);
					 /*cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
					 cout << v << "    11" << "\n";
					 cout << h << "    11" << "\n";*/
				 }
				 else if (Centrifugal_force() < grav()) {// «’ œ«„ «·ﬁ„— »«·√—÷ 

					 v+=50;
					 f = f + (low_orbit_calculation() / 360);
					 /*cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
					 cout << v << "    22" << "\n";*/
				 }
				 if (f > 360)
				 {
					 f = 0;

				 }
			 }
			 else
			 {
				 if (h <= 400) {
					 cout << "the satellite is crashing \n";

					 cru = true;

				 }
				 else if (abs(Centrifugal_force() - grav()) > 1 && grav() < Centrifugal_force())
				 {
					 cout << "The satellite is out of orbit in space \n";
					 h += 10;
				 }
				 else if (abs(Centrifugal_force() - grav()) < 1)//abs (gc)
				 {
					 //cout << "###################################\n";
					 //cout << "wade = " << Centrifugal_force() - grav() << "\n";
					 //cout << v << "      00" << "\n";
					 f = f + (low_orbit_calculation() / 360);

					 //open file for writing

				 }
				 else if (Centrifugal_force() > grav()) {//Œ—ÊÃ ⁄‰ „œ«—Â Ê ÷Ì«⁄Â ›Ì «·›÷«¡
					 h += 10;

					 f = f + (low_orbit_calculation() / 360);
					 /*cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
					 cout << v << "    11" << "\n";
					 cout << h << "    11" << "\n";*/
				 }
				 else if (Centrifugal_force() < grav()) {// «’ œ«„ «·ﬁ„— »«·√—÷ 

					 h -=10;
					 f = f + (low_orbit_calculation() / 360);
					 /*cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
					 cout << v << "    22" << "\n";*/
				 }
				 if (f > 360)
				 {
					 f = 0;

				 }
				
			 }
			//cout << low_orbit_calculation();
			 
		}
		return f;
	}
	float low_orbit_calculation() {
		//Õ”«» «·ﬁÊ… «·Ê“‰Ì… 
		float G = grav() * m * 5.51 * r * (4 * 3.14) / 3;//5.51 ﬂÀ«›… «·«—÷ 
		//Õ”«» «·«· ”«—⁄ «·„—ﬂ“Ì 
		float g = grav() * 5.51 * r * (4 * 3.14) / 3;
		v = sqrt(r * g);
		return (2 * 3.14 * r) / v;
	}
private:
	GLfloat f=0;
	float g01;
	float soso;
	float final1;

};

sat::sat(){
	
}

sat::~sat()
{
}

// physics department
void law_of_universal_attraction(){
}


// 4 Flag variables for angle and keyboard controls
static int menuRetrograde, menuAngle, menuView, menuArt;
//Variables used for menu
int menu_id;
int submenu_id;
int value = 0;
//static float angle = 0;

//Model_3DS *earth;

GLfloat a[100] = { 0.55, 0.54, 0.97, 27, 0.64, 0.14, 1.0, 0.48,
				 0.73, 0.32, 0.24, 0.1, 0.37, 0.94, 0.1, 0.4, 0.74, 0.29, 0.91,		
				 0.63, 0.05, 0.1, 0.43, 0.02, 0.0, 0.67, 0.79, 0.43, 0.49, 0.58,
				 0.41, 0.76, 0.53, 0.77, 0.44, 0.97, 0.93, 0.08, 0.88, 0.13, 0.95,
				 0.44, 0.23, 0.86, 0.83, 0.37, 0.96, 0.4, 0.06, 0.67, 0.89, 0.76, 1.0,
				 0.01, 0.1, 0.6, 0.66, 0.49, 0.93, 0.57, 0.13, 0.79, 0.8, 0.88, 0.91, 0.15,
				 0.38, 0.59, 0.99, 0.13, 0.23, 0.52, 0.68, 0.99, 0.71, 0.53, 0.86, 0.26, 0.39,
				 0.65, 0.23, 0.53, 0.66, 0.94, 0.86, 0.52, 0.86, 0.64, 0.79, 0.91, 0.78, 0.4,
				 0.04, 0.24, 0.8, 0.87, 0.64
};

sat y;


void spin()
{
	y.Motivational_Balance_System();
	if (d>360) {
		d = 0;
	}
	else
	{
		d = d + 0.00046388888888888888888888888888889;//”—⁄… œÊ—«‰ «·√—÷ ›Ì «·À«‰Ì… 
	}
	glutPostRedisplay();
}

//To resize without changing shape
void reshape(int w, int h)
{
	GLdouble aspect = (GLdouble)w / (GLdouble)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
}

//Randomly Draws points to create stars
void background()
{
	glPointSize(2);
	for (int i = 0; i < 100; i++)
	{
		if (a[i] > 0.15 && a[i - 1] > 0.15 && a[i + 1] > 0.3)
		{
			glBegin(GL_POINTS);
			glVertex3f(a[i], a[i + 1], -a[i - 1]);
			glVertex3f(2 * a[i], 2 * a[i + 1], -2 * a[i - 1]);
			glVertex3f(a[i], 2 * a[i + 1], -a[i - 1]);
			glVertex3f(2 * a[i], a[i + 1], -a[i - 1]);
			glVertex3f(a[i], -a[i + 1], -a[i - 1]);
			glVertex3f(2 * a[i], -a[i + 1], -a[i - 1]);
			glVertex3f(a[i], -2 * a[i + 1], -a[i - 1]);
			glVertex3f(-a[i], a[i + 1], -a[i - 1]);
			glVertex3f(-2 * a[i], 2 * a[i + 1], -2 * a[i - 1]);
			glVertex3f(-2 * a[i], a[i + 1], -a[i - 1]);
			glVertex3f(-a[i], -a[i + 1], -a[i - 1]);
			glVertex3f(-2 * a[i], -a[i + 1], -a[i - 1]);
			glVertex3f(-a[i], -2 * a[i + 1], -a[i - 1]);
			glEnd();
		}
	}
	glFlush();
}

void Key(unsigned char ch, int x, int y)
{

	//Controlling Artificial Satellite
	if (ch == '+') {
		if (::y.orbit < 101) {
			::y.orbit += 10;
			::y.r -= 15000;
			cout << "orbit" << ::y.orbit << "\n";
		}
	}
	if (ch == '-') {
		if (::y.orbit > -101) {
			::y.orbit -= 10;
			::y.r -= 15000;
			cout << "orbit" << ::y.orbit << "\n";
		}
	}
	if (ch == 'w') {
		
			::y.h += 100;
			cout << "the hight is  "<<::y.h << "\n";
		
	}
	if (ch == 's') {
		if (::y.h > 499) {
			::y.h -= 100;
			cout << "the hight is  " << ::y.h << "\n";
		}
	}
	if (ch == '1') {
		::y.mod = 0;
		cout << "mode fixed geostationary orbit " << "\n";
	}
	if (ch == '2') {
		::y.mod = 1;
		cout << "mode High, medium, or low range " << "\n";
	}
	if (ch == 'o') {
		pal = true;
		cout << "Motives Balance System Activated " << "\n";
	}
	if (ch == 'f') {
		pal = false;
		cout << "The impulse balance system is not activated " << "\n";
	}
	glutPostRedisplay();
}
//Front view of the scene

void frontview()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Light Source
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat AmbientLight[] = { 0.1, 0.1, 0.1, 1.0 };  //R,G,B,Alpha
	GLfloat DiffuseLight[] = { 0.5f, 0.5f, 0.5f, 0.1f }; //R,G,B,Alpha
	GLfloat LightPosition[] = { -0.9,0.2, 0.9, 0.1 }; // Set the light position
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	//Colors
	GLfloat Black[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat Cyan[] = { 5.0, 0.0, 0.0, 1.0 };//color planet 
	GLfloat White[] = { 1, 1, 1, 0.5 };
	GLfloat Brown[] = { 0.8, .4, 0.2, 0.2 };
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Cyan);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Cyan);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);
	glRotatef(d, 1, 0, 0);
	glutSolidSphere(0.5, 32, 32);
	glPopMatrix();
	glPopAttrib();
	//
	//glPushMatrix();
	//y.sat1();
	//
	//glScalef(0.02, 0.02, 0.02);
	//glRotatef(y.Motivational_Balance_System() + 20, loc, 0, 0);
	//glTranslatef(-(((y.r + y.h) / 10000000) + 30), loc, 0.0f);
	////cout <<y. Motivational_Balance_System() << "\n";
	if (!cru) {
		//model.draw();
		//glPopMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		//Setting Material Properties
		//glRotatef(y.Motivational_Balance_System(),0,0,y.q/10);
		//cout << y.q;

		glRotatef(y.Motivational_Balance_System(), 0, 1, 0);
		glScalef(0.05, 0.05, 0.05);
		glTranslatef((y.h) / 1000, y.orbit / 10, 10);
		glRotatef(125, 0.2, 1, 0);
		y.sat1();
		glPopMatrix();
		glPopAttrib();
	}
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, White);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, White);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);
	background();
	glPopMatrix();
	glPopAttrib();
	glutSwapBuffers();
}
//
//void topview()
//{
//	glClearColor(0, 0, 0, 0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	//Light Source
//	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	GLfloat AmbientLight[] = { 0.1, 0.1, 0.1, 1.0 };  //R,G,B,Alpha
//	GLfloat DiffuseLight[] = { 0.5f, 0.5f, 0.5f, 0.1f }; //R,G,B,Alpha
//	GLfloat LightPosition[] = { -0.9, 0.9,0.2, 0.1 }; // Set the light position
//	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
//	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
//
//	//Colors
//	GLfloat Black[] = { 0.0, 0.0, 0.0, 1.0 };
//	GLfloat Cyan[] = { 0.0, 1.0, 1.0, 1.0 };
//	GLfloat White[] = { 1, 1, 1, 0.5 };
//	GLfloat Brown[] = { 0.8, .4, 0.2, 0.2 };
//
//	glPushAttrib(GL_ALL_ATTRIB_BITS);
//	glPushMatrix();
//	//Setting Material Properties
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Cyan);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Cyan);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);
//	glRotatef(d, 0, 0, 1);
//	glutSolidSphere(0.5, 32, 32);
//	glPopMatrix();
//	glPopAttrib();
//
//	glPushAttrib(GL_ALL_ATTRIB_BITS);
//	glPushMatrix();
//	glRotatef(f, 0, 0, 1);
//	glRotatef(125, 1, 0.2, 0);
//	glScalef(0.05, 0.05, 0.05);
//	glTranslatef(4, 5, -15);
//	//sat z;
//	glPopMatrix();
//	glPopAttrib();
//
//	if (retrograde == 0)
//	{
//		glPushMatrix();
//		glMaterialfv(GL_FRONT, GL_AMBIENT, Brown);
//		glMaterialfv(GL_FRONT, GL_DIFFUSE, Brown);
//		glMaterialf(GL_FRONT, GL_SHININESS, 0.0);
//		//Angled Motion
//		if (angle == 0)
//			glRotatef(d, 0, 0.1, 1);
//		else //Normal Motion
//			glRotatef(d, 0.2, 0.3, 1);
//		glTranslatef(0.9, 0.1, 0);
//		glScalef(0.1, 0.1, 0.1);
//		glutSolidSphere(0.5, 32, 32);
//		glPopMatrix();
//	}
//	if (retrograde == 1)  //Retrograde Motion
//	{
//		glPushMatrix();
//		glMaterialfv(GL_FRONT, GL_AMBIENT, Brown);
//		glMaterialfv(GL_FRONT, GL_DIFFUSE, Brown);
//		glMaterialf(GL_FRONT, GL_SHININESS, 0.0);
//		if (angle == 0)
//			glRotatef(-d, 0, 0, 1);
//		else
//			glRotatef(-d, 0.2, 0.3, 1);
//		glTranslatef(0.9, 0.1, 0);
//		glScalef(0.1, 0.1, 0.1);
//		glutSolidSphere(0.5, 32, 32);
//		glPopMatrix();
//	}
//
//	glPushAttrib(GL_ALL_ATTRIB_BITS);
//	glPushMatrix();
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, White);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, White);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 00.0);
//	background();
//	glPopMatrix();
//	glPopAttrib();
//	glutSwapBuffers();
//}
//
//void sideview()
//{
//	glClearColor(0, 0, 0, 0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	//Light Source
//	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	GLfloat AmbientLight[] = { 0.1, 0.1, 0.1, 1.0 };  //R,G,B,Alpha
//	GLfloat DiffuseLight[] = { 0.5f, 0.5f, 0.5f, 0.1f }; //R,G,B,Alpha
//	GLfloat LightPosition[] = { 0.9,-0.9,0.2, 0.1 }; // Set the light position
//	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
//	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
//
//	//Colors
//	GLfloat Black[] = { 0.0, 0.0, 0.0, 1.0 };
//	GLfloat Cyan[] = { 0.0, 1.0, 1.0, 1.0 };
//	GLfloat White[] = { 1, 1, 1, 0.5 };
//	GLfloat Brown[] = { 0.8, .4, 0.2, 0.2 };
//	glPushAttrib(GL_ALL_ATTRIB_BITS);
//	glPushMatrix();
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Cyan);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Cyan);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);
//	glRotatef(d, 1, 0, 0);
//	glutSolidSphere(0.5, 32, 32);
//	glPopMatrix();
//	glPopAttrib();
//
//	glPushAttrib(GL_ALL_ATTRIB_BITS);
//	glPushMatrix();
//	glRotatef(f, 1, 0, 0);
//	glRotatef(125, 0.2, 1, 0);
//	glScalef(0.05, 0.05, 0.05);
//	glTranslatef(5, 4, -15);
//	//sat q;
//	glPopMatrix();
//	glPopAttrib();
//
//	if (retrograde == 0)
//	{
//		glPushMatrix();
//
//		glMaterialfv(GL_FRONT, GL_AMBIENT, Brown);
//		glMaterialfv(GL_FRONT, GL_DIFFUSE, Brown);
//		glMaterialf(GL_FRONT, GL_SHININESS, 0.0);
//		//Angled Motion
//		if (angle == 0)
//			glRotatef(d, 1, 0, 0.1);
//		else //Normal Motion
//			glRotatef(d, 1, 0.2, 0.3);
//		glTranslatef(0.1, 0.9, 0);
//		glScalef(0.1, 0.1, 0.1);
//		glutSolidSphere(0.5, 32, 32);
//		glPopMatrix();
//	}
//	if (retrograde == 1)  //Retrograde Motion
//	{
//		glPushMatrix();
//		glMaterialfv(GL_FRONT, GL_AMBIENT, Brown);
//		glMaterialfv(GL_FRONT, GL_DIFFUSE, Brown);
//		glMaterialf(GL_FRONT, GL_SHININESS, 0.0);
//		if (angle == 0)
//			glRotatef(-d, 1, 0, 0);
//		else
//			glRotatef(-d, 1, 0.2, 0.3);
//		glTranslatef(0.1, 0.9, 0);
//		glScalef(0.1, 0.1, 0.1);
//		glutSolidSphere(0.5, 32, 32);
//		glPopMatrix();
//	}
//
//	glPushAttrib(GL_ALL_ATTRIB_BITS);
//	glPushMatrix();
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, White);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, White);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 00.0);
//	background();
//	glPopMatrix();
//	glPopAttrib();
//	glutSwapBuffers();
//}


void display()
{
	
	if (value == 1)
		retrograde = 1;
	else if (value == 2)
		retrograde = 0;
	else if (value == 3)
		view = 0;
	else if (value == 4)
		view = 1;
	else if (value == 5)
		view = 2;
	else if (value == 6)
		angle = 1;
	else if (value == 7)
		angle = 0;
	/*else if (value == 8)
	{
		f += 5;
		value = 0;
	}
	else if (value == 9)
	{
		y.Motivational_Balance_System() -= 5;
		value = 0;
	}*/

	if (view == 0)
		frontview();
	/*else if (view == 1)
		topview();
	else
		sideview();*/
	glutPostRedisplay();
}

void menu(int num)
{
	if (num == 0)
		exit(0);
	else
		value = num;
	glutPostRedisplay();
}

void createMenu(void) {
	submenu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Front View (Keyboard Shortcut->1)", 3);
	glutAddMenuEntry("Top View (Keyboard Shortcut->2)", 4);
	glutAddMenuEntry("Side View (Keyboard Shortcut->3)", 5);
	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Retrograde Motion (Keyboard Shortcut->r)", 1);
	glutAddMenuEntry("Non Retrograde Motion (Keyboard Shortcut->n)", 2);
	glutAddSubMenu("Views", submenu_id);
	glutAddMenuEntry("Angular Motion (Keyboard Shortcut->a)", 6);
	glutAddMenuEntry("Non Angular Motion (Keyboard Shortcut->s)", 7);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Move Artificial Satellite Forward (Keyboard Shortcut->+)", 8);
	glutAddMenuEntry("Move Artificial Satellite Backward (Keyboard Shortcut->-)", 9);
	glutAttachMenu(GLUT_LEFT_BUTTON);
}

int main(int C, char* V[])
{
	
	//earth = new Model_3DS();
	//earth->Load("chair.3ds");
	
	do { 
		cout << "pleas inter the orbit \n";
		cin >> y.orbit; 
	} while (abs(y.orbit)>101);
	
	
	//cout << sin(angle)<<"\n";
	//angle =sin( angle * (3.14159265358979323846 / 180));
	//cout << "ff  " << angle;
	glutInit(&C, V);
	glutInitWindowSize(1366, 768);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Simulation Of Satellite Motions Around a Planet");
	glutDisplayFunc(display);
	glutIdleFunc(spin);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(Key);
	//ofstream fw("C:\\Users\\fadi\\Desktop\\tt.txt", std::ofstream::out);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}
