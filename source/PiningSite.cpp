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

Vector2d PiningSite::GetPos() {
	return position;
}

void PiningSite::SetPos(double x, double y) {
	position = { x,y };
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