#include "Vortex.h"

Vortex::Vortex() {
	position = { -1.0, -1.0 };		//不正値による初期化
	velocity = { 0.0,0.0 };			//t=0で速度は0
	force = { 0.0, 0.0 };				//t=0で外力は0
}

Vortex::~Vortex() {
	/* DO NOTHING */
}

Vector2d Vortex::GetPos() const{
	return position;
}

Vector2d Vortex::GetVelocity() {
	return velocity;
}

Vector2d Vortex::GetForce() {
	return force;
}

void Vortex::SetPos(double x, double y) {
	position = { x,y };
}

void Vortex::SetVelocity(double x, double y) {
	velocity = { x,y };
}

void Vortex::SetForce(double x, double y) {
	force = { x,y };
}

void Vortex::AddForce(double x, double y) {
	Vector2d addForce = { x,y };
	force = force + addForce;
}