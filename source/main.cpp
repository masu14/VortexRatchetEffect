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
		Paramater<double> param = inputReader.GetParam();
		
		double var1Start, var1end, var1step,
			   var2Start, var2end, var2step;

		param.voltexNum = 6;		//ボルテックスの数
		param.piningSiteNum = 6;	//ピニングサイトの数
		param.dt = 0.001;			//時間変化量
		param.maxTime = 1;			//計算時間
		param.a = 6;				//初期のボルテックスの格子間隔
		param.cutoff = 16;
		param.eta = 1.0;

		//分子動力学法の実行
		unique_ptr<MD> md = std::make_unique<MD>();
		//md->Run(param);

		
	}
	
	return 0;
}

