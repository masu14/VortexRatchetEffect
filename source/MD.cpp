#include "MD.h"
#include "Voltex.h"

//コンストラクタでボルテックスのインスタンスを生成、構造体の初期化はここでやるべきでないかも
MD::MD() {
	

}

//デストラクタでインスタンスを削除する
MD::~MD() {
	delete voltexs;
}

void MD::Run() {
	InitVolPos();
}

//ボルテックスを初期位置に配置する
void MD::InitVolPos() {
	ExperimentalParam params = {
		5,
		5
	};
	voltexs = new Voltex[params.voltexNum];
	for (int i = 0; i < params.voltexNum; i++) {
		voltexs[i].SetPos(i,i);
		std::cout << "voltex[" << i << "]: " << voltexs[i].GetPos() << std::endl;
	}
	
}