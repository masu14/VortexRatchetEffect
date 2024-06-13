#include <iostream>
#include "Voltex.h"
#include "MD.h"


int main() {
	{
		Paramater paramater = {};
		paramater.voltexNum = 25;		//ボルテックスの数
		paramater.piningSiteNum = 5;	//ピニングサイトの数
		paramater.dt = 0.01f;			//時間変化量
		paramater.height = 5;			//周期的境界条件の高さ
		paramater.weight = 5;			//周期的境界条件の幅

		unique_ptr<MD> md = std::make_unique<MD>();
		md->Run(paramater);
	}
	
	return 0;
}