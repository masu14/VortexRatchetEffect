#include "PiningSite.h"

//----------------------------------------------------------------------------------------
//  PiningSiteクラス(基底クラス)
// ピニングサイトは形状を変えて配置する可能性があるため、共通の性質をこのクラスに定義し、
// 形状による処理の違いは派生クラスで定義する。
//----------------------------------------------------------------------------------------
PiningSite::PiningSite()
{
	position = { -1.0, -1.0 };	//不正な値で初期化
	
}

PiningSite::~PiningSite(){
	/* DO NOTHING */
}

void PiningSite::Setkp(double value) {
	kp = value;
}

void PiningSite::Setlp(double value) {
	lp = value;
}



Vector2d PiningSite::GetPos() {
	return position;
}

void PiningSite::SetPos(double x, double y) 
{
	position = { x,y };
}

void PiningSite::AddPos(double x, double y)
{
	Vector2d addPos = { x, y };
	position = position + addPos;
}

double PiningSite::GetPotentialE()
{
	return potentialE;
}

void PiningSite::SetPotentialE(double energy)
{
	potentialE = energy;
}

void PiningSite::AddPotentialE(double energy)
{
	potentialE += energy;
}

//----------------------------------------------------------------------------------------
// PiningSiteCircleクラス(派生クラス)
// 円形のピニングサイトのクラス
//----------------------------------------------------------------------------------------
PiningSiteCircle::PiningSiteCircle() {
	SetPos(-1.0, -1.0);
	r = -1.0;
}

PiningSiteCircle::~PiningSiteCircle() {
	/* DO NOTHING */
}

double PiningSiteCircle::GetR() {
	return r;
}

void PiningSiteCircle::SetR(double radian) {
	r = radian;
}

Vector2d PiningSiteCircle::CalcPiningForce(Vector2d difPos) const
{
	const double eps = 1e-10;
	Vector2d force = -kp / pow(cosh((difPos.norm() - r) / lp), 2.0) * difPos / (difPos.norm() + eps);
	
	return force;
}

double PiningSiteCircle::CalcPotentialE(Vector2d difPos) const
{
	double energy = kp;
	return energy;
}

//----------------------------------------------------------------------------------------
// PiningSiteLineクラス(派生クラス)
// 線状のピニングサイトのクラス
//----------------------------------------------------------------------------------------
PiningSiteLine::PiningSiteLine() 
{
	SetPos(-1.0, -1.0);
	length = -1.0;
}

PiningSiteLine::~PiningSiteLine()
{
	/* DO NOTHING */
}

double PiningSiteLine::GetLength()
{
	return length;
}

void PiningSiteLine::SetLength(double l)
{
	length = l;
}

Vector2d PiningSiteLine::CalcPiningForce(Vector2d difPos) const
{
	const double eps = 1e-10;
	Vector2d force;
	if (abs(difPos.x()) <= (length / 2.0)) {
		Vector2d dify = { 0, difPos.y() };
		force = -kp / pow(cosh((abs(difPos.y())) / lp), 2.0) * dify / (dify.norm() + eps);
	}
	else if (difPos.x() < -(length / 2.0)) {
		Vector2d l = { length/2.0, 0.0 };
		double d = (difPos+l).norm();
		force = -kp / pow(cosh(d / lp), 2.0) * difPos / (difPos.norm() + eps);
	}
	else if (difPos.x() > (length / 2.0)) {
		Vector2d l = { length / 2.0, 0.0 };
		double d = (difPos - l).norm();
		force = -kp / pow(cosh(d / lp), 2.0) * difPos / (difPos.norm() + eps);
	}
	
	return force;
}

double PiningSiteLine::CalcPotentialE(Vector2d difPos) const
{
	double force=0;
	return force;
}