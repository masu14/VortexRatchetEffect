#include "MD.h"

MD::MD()
{
	/* DO NOTHING */
}

MD::~MD() {
	/* DO NOTHING */
}

void MD::Run(Paramater param) {
	
	

	//ボルテックスを初期配置に並べる
	if (param.voltexNum > 0) {
		voltexs = InitVolPos(param);
	}
	else {
		std::cout << "ボルテックスの数に不正な値が入力されました" << std::endl;
		return;
	}
	//テスト用、ボルテックスの座標を列挙
	for (int i = 0; i < param.voltexNum; i++) {
		std::cout << voltexs[i].GetPos().transpose() << std::endl;
	}
	
	//ピニングサイトを初期配置に並べる
	if (param.piningSiteNum > 0) {
		piningSites = InitPinPos(param);
	}
	else if(param.piningSiteNum==0){
		noPiningSite = true;
	}
	else {
		std::cout << "ピニングサイトの数に不正な値が入力されました" << std::endl;
		return;
	}
	//テスト用、ピニングサイトの座標を列挙
	for (int i = 0; i < param.piningSiteNum; i++) {
		std::cout << piningSites[i].GetPinPos().transpose() << std::endl;
	}

	forceToVoltexs = std::make_unique<float[]>(param.voltexNum);
	forceToVoltexs = {};
}

//ボルテックスを初期位置に配置する
unique_ptr<Voltex[]> MD::InitVolPos(const Paramater& param) {
	//仮の配置処理、ボルテックスの配置方法は正方格子か三角格子なのでそれぞれ用意する予定
	std::unique_ptr<Voltex[]> voltex = std::make_unique<Voltex[]>(param.voltexNum);
	for (int i = 0; i < param.voltexNum; i++) {
		voltex[i].SetPos(i, i);
	}
	return voltex;
}

//ピニングサイトを初期位置に配置する
unique_ptr<PiningSite[]> MD::InitPinPos(const Paramater& param) {
	std::unique_ptr<PiningSite[]> piningSite = std::make_unique<PiningSite[]>(param.piningSiteNum);
	for (int i = 0; i < param.piningSiteNum; i++) {
		piningSite[i].SetPinPos(i, i);
	}
	return piningSite;
}

//ボルテックス・ボルテックス相互作用(VVI)を計算する
float MD::CalcVVI(const Paramater& param) {
	for (int i = 0; i < param.voltexNum ; i++) {
		float force = 0;
		for (int j = 0; j < param.voltexNum; j++) {
			if (j == i) continue;
			Vector2f difPos = voltexs[i].GetPos() - voltexs[j].GetPos();	//ベクトルの差
			if (difPos.x() < -param.weight / 2) difPos.x() += param.weight;
			if (difPos.x() >  param.weight / 2) difPos.x() -= param.weight;
			if (difPos.y() < -param.height / 2) difPos.y() += param.height;
			if (difPos.y() <  param.height / 2) difPos.y() -= param.height;
			float r2 = difPos.dot(difPos);
			if (r2 > param.CUTOFF * param.CUTOFF) continue;
			
			force += r2;
		}
	}
	return 0;
}

//ピニング力を計算する
float MD::CalcPiningForce() {
	return 0;
}

//ローレンツ力を計算する	
float MD::CalcLorentzForce() {
	float force = 5;
	return force;
}

//サーマル力を計算する
float MD::CalcThermalForce() {
	return 0;
}

//運動方程式を解いて位置、速度を更新する
float MD::CalcEOM(Paramater param) {
	
	return 0;

}


void MD::AjustPeriod(Vector2f difPos, Paramater param) {
	if (difPos.x() < -param.weight / 2) difPos.x() += param.weight;
	if (difPos.x() > param.weight / 2)  difPos.x() -= param.weight;
	if (difPos.y() < -param.height / 2)difPos.y()  += param.height;
	if (difPos.y() > param.height / 2)difPos.y()   -= param.height;

}
