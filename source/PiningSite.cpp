#include "PiningSite.h"

PiningSite::PiningSite()
{
	position = { -1.0, -1.0 };	//不正な値で初期化
	pinR = -1.0;				//不正な値で初期化
}

PiningSite::~PiningSite(){
	/* DO NOTHING */
}

Vector2d PiningSite::GetPinPos() {
	return position;
}

void PiningSite::SetPos(double x, double y) {
	position = { x,y };
}