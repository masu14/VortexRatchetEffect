#include <iostream>
#include "Voltex.h"
#include "MD.h"

int main() {
	{
		
		//パラメーターの設定
		Paramater paramater = {};
		paramater.voltexNum = 6;		//ボルテックスの数
		paramater.piningSiteNum = 3;	//ピニングサイトの数
		paramater.dt = 0.001;			//時間変化量
		paramater.a = 0.25;				//初期のボルテックスの格子間隔

		//分子動力学法の実行
		unique_ptr<MD> md = std::make_unique<MD>();
		md->Run(paramater);
	}
	
	return 0;
}

