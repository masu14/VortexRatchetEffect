#pragma once
//#include <iostream>
#include "MD.h"

int FileHandler::index = 0;

int main() {
	{
		//シミュレーションの設定パラメータをinput.iniファイルから取得する
		InputReader inputReader;
		inputReader.ReadParam("input.ini");

		//パラメーターの設定
		Paramater paramater = {};
		paramater.voltexNum = 6;		//ボルテックスの数
		paramater.piningSiteNum = 6;	//ピニングサイトの数
		paramater.dt = 0.001;			//時間変化量
		paramater.maxTime = 1;			//計算時間
		paramater.a = 6;				//初期のボルテックスの格子間隔
		paramater.cutoff = 16;
		paramater.eta = 1.0;

		//分子動力学法の実行
		unique_ptr<MD> md = std::make_unique<MD>();
		//md->Run(paramater);

		
	}
	
	return 0;
}

