#include "Voltex.h"

Voltex::Voltex() {
	volPos = { -1.0f, -1.0f };		//不正値による初期化
	volVelocity = { 0.0f,0.0f };	//t=0で速度は0
}

Vector2f Voltex::GetPos() {
	return volPos;
}

void Voltex::SetPos(float x, float y) {
	volPos = { x,y };

}