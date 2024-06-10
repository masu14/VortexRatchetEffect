#include "Voltex.h"

Voltex::Voltex() {
	position = { -1.0f, -1.0f };		//不正値による初期化
	velocity = { 0.0f,0.0f };			//t=0で速度は0
	force = { 0.0f, 0.0f };				//t=0で外力は0
}

Voltex::~Voltex() {
	/* DO NOTHING */
}

Vector2f Voltex::GetPos() {
	return position;
}

Vector2f Voltex::GetVelocity() {
	return velocity;
}

Vector2f Voltex::GetForce() {
	return force;
}

void Voltex::SetPos(float x, float y) {
	position = { x,y };
}

void Voltex::SetVelocity(float x, float y) {
	velocity = { x,y };
}

void Voltex::SetForce(float x, float y) {
	force = { x,y };
}

void Voltex::AddForce(float x, float y) {
	Vector2f addForce = { x,y };
	force = force + addForce;
}