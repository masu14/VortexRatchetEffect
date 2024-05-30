#include "MD.h"


//コンストラクタ、ボルテックス、ピニングサイトの数を初期化
MD::MD()
{
	
}

//デストラクタでインスタンスを削除する
MD::~MD() {
	delete[] voltexs;
	delete[] piningSites;
}

void MD::Run() {
	ExperimentalParam paramater = {
		1,	//ボルテックスの数
		0,	//ピニングサイトの数
	};

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
		piningSites = nullptr;
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
Voltex* MD::InitVolPos(const ExperimentalParam& param) {
	//仮の配置処理、ボルテックスの配置方法は正方格子か三角格子なのでそれぞれ用意する予定
	Voltex *voltex = new Voltex[param.voltexNum];
	for (int i = 0; i < param.voltexNum; i++) {
		voltex[i].SetPos(i, i);
		
	}
	return voltex;
}

PiningSite* MD::InitPinPos(const ExperimentalParam& param) {
	PiningSite* piningSite = new PiningSite[param.piningSiteNum];
	for (int i = 0; i < param.piningSiteNum; i++) {
		piningSite[i].SetPinPos(i, i);
	}
	return piningSite;
}