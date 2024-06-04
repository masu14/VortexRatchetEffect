#include "MD.h"

MD::MD()
{
	/* DO NOTHING */
}

MD::~MD() {
	/* DO NOTHING */
}

void MD::Run(Paramater paramater) {
	
	

	//ボルテックスを初期配置に並べる
	if (paramater.voltexNum > 0) {
		voltexs = InitVolPos(paramater);
	}
	else {
		std::cout << "ボルテックスの数に不正な値が入力されました" << std::endl;
		return;
	}
	//テスト用、ボルテックスの座標を列挙
	for (int i = 0; i < paramater.voltexNum; i++) {
		std::cout << voltexs[i].GetPos().transpose() << std::endl;
	}
	
	//ピニングサイトを初期配置に並べる
	if (paramater.piningSiteNum > 0) {
		piningSites = InitPinPos(paramater);
	}
	else if(paramater.piningSiteNum==0){
		noPiningSite = true;
	}
	else {
		std::cout << "ピニングサイトの数に不正な値が入力されました" << std::endl;
		return;
	}
	//テスト用、ピニングサイトの座標を列挙
	for (int i = 0; i < paramater.piningSiteNum; i++) {
		std::cout << piningSites[i].GetPinPos().transpose() << std::endl;
	}
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
float MD::CalcVVI() {
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
float MD::CalcEOM() {
	float force = CalcVVI() + CalcPiningForce() + CalcLorentzForce() + CalcThermalForce();
	return 0;

}