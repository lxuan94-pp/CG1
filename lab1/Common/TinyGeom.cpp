#include "Common/TinyGeom.h" 

using namespace TinyGeom; 

Tri2::Tri2():Geom2(){ 
	init(); 

	Tri2 def(Pt2(0,0),50); 
	for(int j=0;j<3;j++)
		_pts[j] = def._pts[j]; 
}

Tri2::Tri2(const Pt2& a, const Pt2& b, const Pt2& c){
	init(); 
	_pts[0] = a; 
	_pts[1] = b; 
	_pts[2] = c; 
}

Tri2::Tri2(const Pt2& c, double r){
	init(); 
	double degs[3] = {-M_PI/6,M_PI/2,M_PI/6*7}; 
	for(int j=0;j<3;j++){
		double dx = cos(degs[j])*r; 
		double dy = sin(degs[j])*r; 
		_pts[j] = c+Vec2(dx,dy,0); 
	}
}

Quad2::Quad2():Geom2(){ 
	init(); 
	Quad2 def(Pt2(0,0),50); 
	for(int j=0;j<size();j++)
		_pts[j] = def._pts[j]; 
}

Quad2::Quad2(const Pt2& c, double r){
	init(); 
	double degs[4] = {-M_PI/4,M_PI/4,M_PI/4*3,-M_PI/4*3}; 
	for(int j=0;j<4;j++){
		double dx = cos(degs[j])*r; 
		double dy = sin(degs[j])*r; 
		_pts[j] = c+Vec2(dx,dy,0); 
	}
}

Hex2::Hex2() :Geom2() {
	init();
	Hex2 def(Pt2(0, 0), 50);
	for (int j = 0; j < size(); j++)
		_pts[j] = def._pts[j];
}


Hex2::Hex2(const Pt2& c, double r) {
	init();
	double degs[6] = { 0, M_PI / 3,  M_PI / 3 * 2, M_PI / 3 * 3, M_PI / 3 * 4, M_PI / 3 * 5 };
	for (int j = 0; j < 6; j++) {
		double dx = cos(degs[j]) * r;
		double dy = sin(degs[j]) * r;
		_pts[j] = c + Vec2(dx, dy, 0);
	}
}


Hex2::Hex2(const Pt2& a, const Pt2& b, const Pt2& c, const Pt2& d, const Pt2& e, const Pt2& f) {
	init();
	_pts[0] = a;
	_pts[1] = b;
	_pts[2] = c;
	_pts[3] = d;
	_pts[4] = e;
	_pts[5] = f;
}


Oct2::Oct2() :Geom2() {
	init();
	Oct2 def(Pt2(0, 0), 50);
	for (int j = 0; j < size(); j++)
		_pts[j] = def._pts[j];
}



Oct2::Oct2(const Pt2& c, double r) {
	init();
	double degs[8] = { 0, M_PI / 4,  M_PI / 4 * 2, M_PI / 4 * 3, M_PI / 4 * 4, M_PI / 4 * 5, M_PI / 4 * 6, M_PI / 4 * 7 };
	for (int j = 0; j < 8; j++) {
		double dx = cos(degs[j]) * r;
		double dy = sin(degs[j]) * r;
		_pts[j] = c + Vec2(dx, dy, 0);
	}
}


Cir2::Cir2() :Geom2() {
	init();
	Cir2 def(Pt2(0, 0), 50);
	for (int j = 0; j < size(); j++)
		_pts[j] = def._pts[j];
}

Cir2::Cir2(const Pt2& c, double r) {
	init();
	//double degs[8] = { 0, M_PI / 4,  M_PI / 4 * 2, M_PI / 4 * 3, M_PI / 4 * 4, M_PI / 4 * 5, M_PI / 4 * 6, M_PI / 4 * 7 };
	double degs[100] = { 0, };
	for (int i = 0; i < 100; i++) {
		degs[i] = i * 2 * M_PI / 100;
	}
	for (int j = 0; j < 100; j++) {
		double dx = cos(degs[j]) * r;
		double dy = sin(degs[j]) * r;
		_pts[j] = c + Vec2(dx, dy, 0);
	}
}



 Star2::Star2() :Geom2() {
 	init();
 	Star2 def(Pt2(0, 0), 50);
 	for (int j = 0; j < size(); j++)
 		_pts[j] = def._pts[j];
 }

 Star2::Star2(const Pt2& c, double r) {
 	init();

 	double degs[12] = { 0, };
 	for (int i = 0; i < 12; i++) {
 		degs[i] = i * M_PI / 6;
 	}
 	int flag = -1;
 	for (int j = 0; j < 12; j++) {
 		double temp_r = 0;
 		if (flag == -1) {
 			temp_r = r / 1.732;
 		}
 		else {
 			temp_r = r;
 		}
 		flag *= -1;
 		double dx = cos(degs[j]) * temp_r;
 		double dy = sin(degs[j]) * temp_r;
 		_pts[j] = c + Vec2(dx, dy, 0);
 	}
 }





double Utils::cross2d(const Vec2& v, const Vec2& w){
	return v[0]*w[1] - v[1]*w[0]; 
}

double Utils::dist2d(const Pt2& a, const Pt2& b){
	double dx = a[0]-b[0]; 
	double dy = a[1]-b[1]; 
	return sqrt(dx*dx + dy*dy); 
}

bool Utils::isPtInterior(Geom2* g, const Pt2& p){
	if(g->size()<3) 
		return false; 

	Vec2 v1 = (*g->get(0))-p; 
	Vec2 v2 = (*g->get(1))-p; 
	double d = cross2d(v1,v2); 
	int sign = d<0 ? -1 : 1; 

	for(int j=1;j<g->size();j++){
		int next = (j+1)%g->size(); 
		v1 = (*g->get(j))-p; 
		v2 = (*g->get(next))-p; 
		d = cross2d(v1,v2); 
		int nsign = d<0? -1 : 1; 
		if(sign!=nsign)
			return false; 
	}

	return true; 
}

bool Utils::isConvex(Geom2* g){
	if(g->size()<3) return false; 

	for(int j=0;j<g->size();j++){
		int a = (j-1+g->size())%g->size(); 
		int b = j; 
		int c = (j+1)%g->size(); 

		Vec2 cb = (*g->get(c))-(*g->get(b)); 
		Vec2 ab = (*g->get(a))-(*g->get(b)); 

		if(cross2d(cb,ab)<0) return false; 
	}

	return true; 
}

Pt2 Utils::centroid(Geom2* g){
	Pt2 p(0,0); 
	for(int j=0;j<g->size();j++){
		p+=(*g->get(j)); 
	}
	if(g->size()>0)
		p/=g->size(); 
	return p; 
}
