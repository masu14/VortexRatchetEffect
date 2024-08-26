#include "Voltex.h"

Voltex::Voltex() {
	position = { -1.0, -1.0 };		//不正値による初期化
	velocity = { 0.0,0.0 };			//t=0で速度は0
	force = { 0.0, 0.0 };				//t=0で外力は0
}

Voltex::~Voltex() {
	/* DO NOTHING */
}

Vector2d Voltex::GetPos() const{
	return position;
}

Vector2d Voltex::GetVelocity() {
	return velocity;
}

Vector2d Voltex::GetForce() {
	return force;
}

void Voltex::SetPos(double x, double y) {
	position = { x,y };
}

void Voltex::SetVelocity(double x, double y) {
	velocity = { x,y };
}

void Voltex::SetForce(double x, double y) {
	force = { x,y };
}

void Voltex::AddForce(double x, double y) {
	Vector2d addForce = { x,y };
	force = force + addForce;
}