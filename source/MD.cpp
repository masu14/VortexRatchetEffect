#include "MD.h"
#include "Voltex.h"

//コンストラクタ、ボルテックス、ピニングサイトの数を初期化
MD::MD()
{
	
}

//デストラクタでインスタンスを削除する
MD::~MD() {
	//delete voltexs;
}

void MD::Run() {
	ExperimentalParam paramater = {
		5,	
		5,
	};
	InitVolPos(paramater);
	
}

//ボルテックスを初期位置に配置する
Voltex* MD::InitVolPos(ExperimentalParam param) {
	//仮の配置処理、ボルテックスの配置方法は正方格子か三角格子なのでそれぞれ用意する予定
	Voltex *voltex = new Voltex[param.voltexNum];
	for (int i = 0; i < param.voltexNum; i++) {
		voltex[i].SetPos(i, i);
		std::cout << voltex[i].GetPos() << std::endl;
	}
	return voltex;
	
}
