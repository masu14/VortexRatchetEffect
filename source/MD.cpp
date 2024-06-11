#include "MD.h"

MD::MD()
{
	/* DO NOTHING */
}

MD::~MD() {
	/* DO NOTHING */
}

void MD::Run(Paramater param) {
	
	

	//ボルテックスを初期配置に並べる
	if (param.voltexNum > 0) {
		voltexs = InitVolPos(param);
	}
	else {
		std::cout << "ボルテックスの数に不正な値が入力されました" << std::endl;
		return;
	}
	//テスト用、ボルテックスの座標を列挙
	for (int i = 0; i < param.voltexNum; i++) {
		std::cout << "voltex["<<i<<"]の座標 " << voltexs[i].GetPos().transpose() << std::endl;
	}
	
	//ピニングサイトを初期配置に並べる
	if (param.piningSiteNum > 0) {
		piningSites = InitPinPos(param);
	}
	else if(param.piningSiteNum==0){
		noPiningSite = true;
	}
	else {
		std::cout << "ピニングサイトの数に不正な値が入力されました" << std::endl;
		return;
	}
	//テスト用、ピニングサイトの座標を列挙
	for (int i = 0; i < param.piningSiteNum; i++) {
		std::cout << "piningSites["<<i<<"]の座標 " << piningSites[i].GetPinPos().transpose() << std::endl;
	}

	InitForce(param);
	for (int i = 0; i < param.voltexNum; i++) {
		std::cout << "voltex["<< i<< "]への外力 " << voltexs[i].GetForce().transpose() << std::endl;
	}

	//ボルテックスへの外力を初期化する
	//
	CalcVVI(param);
	std::cout << "VVIを計算" << std::endl;
	//テスト用、ボルテックスへの外力を列挙
	for (int i = 0; i < param.voltexNum; i++) {
		std::cout << "voltex["<<i<<"]への外力 " << voltexs[i].GetForce().transpose() << std::endl;
	}
	//CalcPiningForce();
	//CalcLorentzForce();
	//CalcThermalForce();
	//CalcEOM(param);
}

//-------------------------------------------------------------------------------------------------
//		ボルテックスを初期位置に配置する
//-------------------------------------------------------------------------------------------------
unique_ptr<Voltex[]> MD::InitVolPos(const Paramater& param) {
	//仮の配置処理、ボルテックスの配置方法は正方格子か三角格子なのでそれぞれ用意する予定
	std::unique_ptr<Voltex[]> voltex = std::make_unique<Voltex[]>(param.voltexNum);
	for (int i = 0; i < param.voltexNum; i++) {
		voltex[i].SetPos(i, i);
	}
	return voltex;
}

//-------------------------------------------------------------------------------------------------
//		ピニングサイトを初期位置に配置する
//-------------------------------------------------------------------------------------------------
unique_ptr<PiningSite[]> MD::InitPinPos(const Paramater& param) {
	std::unique_ptr<PiningSite[]> piningSite = std::make_unique<PiningSite[]>(param.piningSiteNum);
	for (int i = 0; i < param.piningSiteNum; i++) {
		piningSite[i].SetPinPos(i, i);
	}
	return piningSite;
}

//-------------------------------------------------------------------------------------------------
//		外力を0に初期化する
//-------------------------------------------------------------------------------------------------
void MD::InitForce(const Paramater& param) {
	for (int i = 0; i < param.voltexNum; i++) {
		voltexs[i].SetForce(0.0f, 0.0f);
	}
}

//-------------------------------------------------------------------------------------------------
//		ボルテックス・ボルテックス相互作用(VVI)を計算する
//-------------------------------------------------------------------------------------------------
void MD::CalcVVI(const Paramater& param) {
	for (int i = 0; i < param.voltexNum -1 ; i++) {
		for (int j = i+1; j < param.voltexNum; j++) {
			float f0 = 1.0f;	//VVIの係数f0
			
			Vector2f difPos = voltexs[i].GetPos() - voltexs[j].GetPos();	//ベクトルの差

			//周期的に繰り返すボルテックスのうち、近いボルテックスを計算する
			//周期的境界条件に対してカットオフ長さが短ければこの計算でもうまくいくが要検討
			if (difPos.x() < -param.weight / 2) difPos.x() += param.weight;
			if (difPos.x() >  param.weight / 2) difPos.x() -= param.weight;
			if (difPos.y() < -param.height / 2) difPos.y() += param.height;
			if (difPos.y() <  param.height / 2) difPos.y() -= param.height;

			float difPos2 = difPos.dot(difPos);						//difPos2はボルテックス同士の距離の2乗
			if (difPos2 > param.CUTOFF * param.CUTOFF) continue;	//ボルテックス同士の距離がカットオフ長さより長ければ計算しない
			
			float xForce = f0 * expf(- difPos.x()/ lambda) / difPos.norm() * difPos.x();	//VVIのx成分
			float yForce = f0 * expf(- difPos.y()/ lambda) / difPos.norm() * difPos.y();	//VVIのy成分
			voltexs[i].AddForce(xForce, yForce);	//作用
			voltexs[j].AddForce(-xForce, -yForce);	//反作用
		}
	}
	
}

//-------------------------------------------------------------------------------------------------
//		ピニング力を計算する
//-------------------------------------------------------------------------------------------------
void MD::CalcPiningForce() {
	
}

//-------------------------------------------------------------------------------------------------
//		ローレンツ力を計算する	
//-------------------------------------------------------------------------------------------------
void MD::CalcLorentzForce() {
	float force = 5;
	
}

//-------------------------------------------------------------------------------------------------
//		サーマル力を計算する
//-------------------------------------------------------------------------------------------------
void MD::CalcThermalForce() {
	
}

//-------------------------------------------------------------------------------------------------
//		運動方程式を解いて位置、速度を更新する
//-------------------------------------------------------------------------------------------------
void MD::CalcEOM(const Paramater& param) {
	

}

