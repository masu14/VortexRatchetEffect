#include <iostream>
#include "Voltex.h"
#include "MD.h"

int main() {
	{
		Paramater paramater = {};
		paramater.voltexNum = 12;		//ボルテックスの数
		paramater.piningSiteNum = 0;	//ピニングサイトの数
		paramater.dt = 0.001;			//時間変化量
		paramater.height = 2.0*sqrt(3);			//周期的境界条件の高さ
		paramater.weight = 3.0;			//周期的境界条件の幅

		unique_ptr<MD> md = std::make_unique<MD>();
		md->Run(paramater);
	}
	
	return 0;
}

