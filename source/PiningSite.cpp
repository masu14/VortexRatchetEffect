#include "PiningSite.h"

PiningSite::PiningSite()
{
	position = { -1.0f, -1.0f };	//不正な値で初期化
	pinR = -1.0f;				//不正な値で初期化
}

PiningSite::~PiningSite(){
	/* DO NOTHING */
}

Vector2f PiningSite::GetPinPos() {
	return position;
}

void PiningSite::SetPinPos(float x, float y) {
	position = { x,y };
}