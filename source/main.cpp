#pragma once
//#include <iostream>
#include "MD.h"

int FileHandler::index = 0;

int main() {
	{
		//シミュレーションの設定パラメータをinput.iniファイルから取得する
		InputReader inputReader;
		inputReader.ReadParam("input/input.ini");

		//パラメーターの設定
		Paramater<double> param = inputReader.GetParam();
		
		std::cout << "[Paramater]" << std::endl;
		std::cout << "voltexNum: " << param.voltexNum << std::endl;
		std::cout << "piningSiteNum: " << param.piningSiteNum << std::endl;
		std::cout << "dt: " << param.dt << std::endl;
		std::cout << "maxTime: " << param.maxTime << std::endl;
		std::cout << "a: " << param.a << std::endl;
		std::cout << "cutoff: " << param.cutoff << std::endl;
		std::cout << "eta: " << param.eta << std::endl;

		std::cout << "[Variable]" << std::endl;
		std::cout << param.var1name <<": " << param.var1[0] << "," << param.var1[1] << "," << param.var1[2] << std::endl;
		std::cout << param.var2name <<": " << param.var2[0] << "," << param.var2[1] << "," << param.var2[2] << std::endl;

		if(param.var1name == "lorentzForce") param.lorentzForce = param.var1[0];


		while (param.lorentzForce <= param.var1[1]) {
			//分子動力学法の実行
			unique_ptr<MD> md = std::make_unique<MD>();
			md->Run(param);
			param.lorentzForce += param.var1[2];
		}
	}
	
	return 0;
}

