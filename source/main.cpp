﻿#include <iostream>
#include "Voltex.h"
#include "MD.h"

int FileHandler::index = 0;

int main() {
	{
		
		//パラメーターの設定
		Paramater paramater = {};
		paramater.voltexNum = 6;		//ボルテックスの数
		paramater.piningSiteNum = 6;	//ピニングサイトの数
		paramater.dt = 0.001;			//時間変化量
		paramater.a = 6;				//初期のボルテックスの格子間隔
		paramater.cutoff = 16;

		//分子動力学法の実行
		unique_ptr<MD> md = std::make_unique<MD>();
		md->Run(paramater);

		
	}
	
	return 0;
}

