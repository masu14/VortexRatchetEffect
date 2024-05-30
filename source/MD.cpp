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
		5,	//ボルテックスの数
		5,	//ピニングサイトの数
	};
	voltexs= InitVolPos(paramater);
	for (int i = 0; i < paramater.voltexNum; i++) {
		std::cout << voltexs[i].GetPos().transpose() << std::endl;
	}
	
	piningSites = InitPinPos(paramater);
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