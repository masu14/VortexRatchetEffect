#include <iostream>
#include "Voltex.h"
#include "MD.h"

int main() {
	{
		Paramater paramater = {};
		paramater.voltexNum = 12;		//ボルテックスの数
		paramater.piningSiteNum = 0;	//ピニングサイトの数
		paramater.dt = 0.001;			//時間変化量
		paramater.a = 0.25;				//初期のボルテックスの格子間隔

		unique_ptr<MD> md = std::make_unique<MD>();
		md->Run(paramater);
	}
	
	return 0;
}

