#include <cmath> 
#include "Rendering/Transformation.h" 

void Transformation::setAsIdentity(){
	_mat.identity(); 
}

void Transformation::setAsTranslate(const Vec2& v){
	//TODO: fill this in
	_mat[0][0] = 1;
	_mat[0][1] = 0;
	_mat[0][2] = 0;

	_mat[1][0] = 0;
	_mat[1][1] = 1;
	_mat[1][2] = 0;

	_mat[2][0] = v[0];
	_mat[2][1] = v[1];
	_mat[2][2] = 1;
}


void Transformation::setAsRotate(double r, const Pt2& q){
	//TODO: fill this in
	double rad = r / 180 * M_PI;
	_mat[0][0] = cos(rad);
	_mat[0][1] = sin(rad);
	_mat[0][2] = 0;
	_mat[1][0] = -sin(rad);
	_mat[1][1] = cos(rad);
	_mat[1][2] = 0;
	_mat[2][0] = q[0] * (1 - cos(rad)) + q[1] * sin(rad);
	_mat[2][1] = q[1] * (1 - cos(rad)) - q[0] * sin(rad);
	_mat[2][2] = 1;

}

void Transformation::setAsScale(double s, const Pt2& p){
	//TODO: fill this in
	_mat[0][0] = s;
	_mat[0][1] = 0;
	_mat[0][2] = 0;

	_mat[1][0] = 0;
	_mat[1][1] = s;
	_mat[1][2] = 0;

	_mat[2][0] = (1 - s) * p[0];
	_mat[2][1] = (1 - s) * p[1];
	_mat[2][2] = 1;
}


void Transformation::setAsNUScale(const Vec2& s, const Pt2& q){
	//TODO: fill this in
	
	Mat3 L;
	Mat3 R;
	double length = sqrt(s[0] * s[0] + s[1] * s[1]);
	double s1 = s[0] / length;
	double s2 = s[1] / length;
	double s3 = s[2] / length;

	L[0][0] = s1;
	L[1][0] = -s2;
	L[2][0] = q[0];

	L[0][1] = s2;
	L[1][1] = s1;
	L[2][1] = q[1];

	L[0][2] = 0;
	L[1][2] = 0;
	L[2][2] = 1;

	R[0][0] = s[0];
	R[1][0] = -s2;
	R[2][0] = q[0];
	
	R[0][1] = s[1];
	R[1][1] = s1;
	R[2][1] = q[1];

	R[0][2] = 0;
	R[1][2] = 0;
	R[2][2] = 1;
	
	_mat = (!L) * R;

}

void Transformation::composeTranslate(const Vec2& v){
	//TODO: fill this in
	Mat3 temp;
	temp[0][0] = 1;
	temp[0][1] = 0;
	temp[0][2] = 0;

	temp[1][0] = 0;
	temp[1][1] = 1;
	temp[1][2] = 0;

	temp[2][0] = v[0];
	temp[2][1] = v[1];
	temp[2][2] = 1;
	_mat = temp * _mat;
}

void Transformation::composeRotate(double r, const Pt2& q){
	//TODO: fill this in
	Mat3 temp;
	double rad = r / 180 * M_PI;
	temp[0][0] = cos(rad);
	temp[0][1] = sin(rad);
	temp[0][2] = 0;
	temp[1][0] = -sin(rad);
	temp[1][1] = cos(rad);
	temp[1][2] = 0;
	temp[2][0] = q[0] * (1 - cos(rad)) + q[1] * sin(rad);
	temp[2][1] = q[1] * (1 - cos(rad)) - q[0] * sin(rad);
	temp[2][2] = 1;
	_mat = temp * _mat;

}

void Transformation::composeScale(double s, const Pt2& q){
	//TODO: fill this in
	Mat3 temp;
	temp[0][0] = s;
	temp[0][1] = 0;
	temp[0][2] = 0;

	temp[1][0] = 0;
	temp[1][1] = s;
	temp[1][2] = 0;

	temp[2][0] = (1 - s) * q[0];
	temp[2][1] = (1 - s) * q[1];
	temp[2][2] = 1;

	_mat = temp * _mat;

}

void Transformation::composeNUScale(const Vec2& s, const Pt2& q){
	//TODO: fill this in
	Mat3 L;
	Mat3 R;
	double length = sqrt(s[0] * s[0] + s[1] * s[1]);
	double s1 = s[0] / length;
	double s2 = s[1] / length;

	L[0][0] = s1;
	L[0][1] = s2;
	L[0][2] = 0;

	L[1][0] = -s2;
	L[1][1] = s1;
	L[1][2] = 0;

	L[2][0] = q[0];
	L[2][1] = q[1];
	L[2][2] = 1;

	R[0][0] = s[0];
	R[0][1] = s[1];
	R[0][2] = 0;

	R[1][0] = -s2;
	R[1][1] = s1;
	R[1][2] = 0;

	R[2][0] = q[0];
	R[2][1] = q[1];
	R[2][2] = 1;

	_mat = (!L) * R * _mat;

}

void Transformation::compose3PtTransform(const Tri2& src, const Tri2& dest){
	//TODO: fill this in
	Pt2 ps1 = *src.get(0);
	Pt2 ps2 = *src.get(1);
	Pt2 ps3 = *src.get(2);

	Pt2 pd1 = *dest.get(0);
	Pt2 pd2 = *dest.get(1);
	Pt2 pd3 = *dest.get(2);
	
	Mat3 L;
	Mat3 R;

	L[0][0] = ps1[0];
	L[0][1] = ps1[1];
	L[0][2] = 1;
	L[1][0] = ps2[0];
	L[1][1] = ps2[1];
	L[1][2] = 1;
	L[2][0] = ps3[0];
	L[2][1] = ps3[1];
	L[2][2] = 1;

	R[0][0] = pd1[0];
	R[0][1] = pd1[1];
	R[0][2] = 1;
	R[1][0] = pd2[0];
	R[1][1] = pd2[1];
	R[1][2] = 1;
	R[2][0] = pd3[0];
	R[2][1] = pd3[1];
	R[2][2] = 1;

	_mat = (!L) * R * _mat;
}


void Transformation::setAs3PtTransform(const Tri2& src, const Tri2& dest){
	//TODO: fill this in
	
	Pt2 ps1 = *src.get(0);
	Pt2 ps2 = *src.get(1);
	Pt2 ps3 = *src.get(2);

	Pt2 pd1 = *dest.get(0);
	Pt2 pd2 = *dest.get(1);
	Pt2 pd3 = *dest.get(2);

	Mat3 L;
	Mat3 R;

	L[0][0] = ps1[0];
	L[0][1] = ps1[1];
	L[0][2] = 1;
	L[1][0] = ps2[0];
	L[1][1] = ps2[1];
	L[1][2] = 1;
	L[2][0] = ps3[0];
	L[2][1] = ps3[1];
	L[2][2] = 1;

	R[0][0] = pd1[0];
	R[0][1] = pd1[1];
	R[0][2] = 1;
	R[1][0] = pd2[0];
	R[1][1] = pd2[1];
	R[1][2] = 1;
	R[2][0] = pd3[0];
	R[2][1] = pd3[1];
	R[2][2] = 1;

	_mat = (!L) * R;
}
