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
	
	//ボルテックスへの外力を初期化する
	InitForce(param);
	for (int i = 0; i < param.voltexNum; i++) {
		std::cout << "voltex["<< i<< "]への外力初期化 " << voltexs[i].GetForce().transpose() << std::endl;
	}

	//VVIを計算
	CalcVVI(param);
	std::cout << "VVIを計算" << std::endl;
	//テスト用、ボルテックスへの外力を列挙
	for (int i = 0; i < param.voltexNum; i++) {
		std::cout << "voltex["<<i<<"]への外力 " << voltexs[i].GetForce().transpose() << std::endl;
	}
	//CalcPiningForce();
	//CalcLorentzForce();
	//CalcThermalForce();
	double time = 0.0;
	while (time < 10000) {
		CalcEOM(param);
		time += 0.01;
	}
	for (int i = 0; i < param.voltexNum; i++) {
		std::cout << voltexs[i].GetPos().x() <<", ";
		std::cout << voltexs[i].GetPos().y() << std::endl;
	}
	

}

//-------------------------------------------------------------------------------------------------
//		ボルテックスを初期位置に配置する
//-------------------------------------------------------------------------------------------------
unique_ptr<Voltex[]> MD::InitVolPos(const Paramater& param) {
	//仮の配置処理、ボルテックスの配置方法は正方格子か三角格子なのでそれぞれ用意する予定
	std::unique_ptr<Voltex[]> voltex = std::make_unique<Voltex[]>(param.voltexNum);
	for (int i = 0; i < param.voltexNum; i++) {
		voltex[i].SetPos((double)i/5.0, (double)i/5.0);
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
		voltexs[i].SetForce(0.0, 0.0);
	}
}

//-------------------------------------------------------------------------------------------------
//		ボルテックス・ボルテックス相互作用(VVI)を計算する
//-------------------------------------------------------------------------------------------------
void MD::CalcVVI(const Paramater& param) {
	for (int i = 0; i < param.voltexNum -1 ; i++) {
		for (int j = i+1; j < param.voltexNum; j++) {
			double f0 = 1.0;	//VVIの係数f0
			
			Vector2d difPos = voltexs[i].GetPos() - voltexs[j].GetPos();		//ベクトルの差

			//周期的に繰り返すボルテックスのうち、近いボルテックスを計算する
			//周期的境界条件に対してカットオフ長さが短ければこの計算でもうまくいくが要検討
			if (difPos.x() < -param.weight / 2) difPos.x() += param.weight;
			if (difPos.x() >  param.weight / 2) difPos.x() -= param.weight;
			if (difPos.y() < -param.height / 2) difPos.y() += param.height;
			if (difPos.y() <  param.height / 2) difPos.y() -= param.height;

			//以下、ボルテックス同士の距離がカットオフ長さより長ければ計算しない
			if (difPos.norm() > param.CUTOFF) continue;						
			
			Vector2d force = f0 * exp(- difPos.norm() / lambda) * difPos;	//VVI
			double xForce = force.x();				//VVIのx成分
			double yForce = force.y();				//VVIのy成分
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
	double force = 5;
	
}

//-------------------------------------------------------------------------------------------------
//		サーマル力を計算する
//-------------------------------------------------------------------------------------------------
void MD::CalcThermalForce() {
	
}

//-------------------------------------------------------------------------------------------------
//		運動方程式を解いてボルテックスの位置、速度を更新する
//-------------------------------------------------------------------------------------------------
void MD::CalcEOM(const Paramater& param) 
{
	//速度ベルレ法を用いた時間発展でボルテックスの位置、速度を更新する
	//r(t+dt) = r(t) + v(t)*dt + (1/2)*(F(t)/m)*dt^2
	//v(t+dt) = v(t) + (1/2)*((F(t)+F(t+dt))/m)*dt
	{
		const double dt = param.dt;					//時間変化量
		double eta = 10.0;							//粘性抵抗η

		unique_ptr<Vector2d[]> v1 = std::make_unique<Vector2d[]>(param.voltexNum);	//速度v(t)の動的配列、v(t+dt)の計算に使う
		unique_ptr<Vector2d[]> f1 = std::make_unique<Vector2d[]>(param.voltexNum);	//外力F(t)、v(t+dt)の計算に使う
		for (int i = 0; i < param.voltexNum; i++) {

			Vector2d r1 = voltexs[i].GetPos();		//位置r(t)
			v1[i] = voltexs[i].GetVelocity();		//速度v(t)
			f1[i] = voltexs[i].GetForce();			//外力F(t)

			//位置r(t+dt)を計算し、更新する
			Vector2d r2 = r1 + v1[i] * dt + (f1[i] / eta) / 2 * dt * dt;	//位置r(t+dt)の計算

			//周期的境界条件
			if (r2.x() < 0)            r2.x() += param.weight;
			if (r2.x() > param.weight) r2.x() -= param.weight;
			if (r2.y() < 0)            r2.y() += param.height;
			if (r2.y() > param.height) r2.y() -= param.height;

			voltexs[i].SetPos(r2.x(), r2.y());								//位置r(t+dt)の更新
		}

		//外力の再計算を行い、F(t+dt)を計算する
		InitForce(param);	//ボルテックスへの外力を初期化
		CalcVVI(param);		//F(t+dt)の計算

		//v(t),F(t),F(t+dt)を用いて速度v(t+dt)を計算し、更新する
		for (int i = 0; i < param.voltexNum; i++) {
			
			Vector2d f2 = voltexs[i].GetForce();
			Vector2d v2 = v1[i] + (f1[i] + f2) / (2 * eta) * dt;	//速度v(t+dt)の計算
			
			voltexs[i].SetForce(f2.x(), f2.y());					//外力F(t+dt)の更新、次の時間発展の位置r(t)計算で使う
			voltexs[i].SetVelocity(v2.x(), v2.y());					//速度v(t+dt)の更新
		}
	}
}

