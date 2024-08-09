#include "MD.h"
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>

//コンストラクタ
MD::MD()
{
	/* DO NOTHING */
}

//デストラクタ
MD::~MD() {
	/* DO NOTHING */
}

//main.cppでparamaterを設定し、Runメソッドでmd計算を実行する
void MD::Run(Paramater param) {

	voltexNum		= param.voltexNum;
	piningSiteNum	= param.piningSiteNum;
	dt				= param.dt;
	weight			= param.weight;
	height			= param.height;
	cutoff			= param.cutoff;

	if (InitApp())
	{
		MainLoop();
	}

	//TermApp();
}

bool MD::InitApp() {

	// ボルテックスの初期化
	if(!InitVolPos())
	{
		return false;
	}
	// ピニングサイトの初期化
	if(!InitPinPos())
	{
		return false;
	}
	//ボルテックスへの外力を初期化
	InitForce();

	return true;
}

//-------------------------------------------------------------------------------------------------
//		ボルテックスを初期位置に配置する
//-------------------------------------------------------------------------------------------------
bool MD::InitVolPos() {
	if (voltexNum <= 0) {
		std::cout << "vortexNumに不正な値が入力されました" << std::endl;
		return false;
	}
	voltexs = std::make_unique<Voltex[]>(voltexNum);
	double a = 0.25;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			if(i%2==0) voltexs[]
		}
		voltexs[i].SetPos((double)i/5.0, 0.0);
	}
	return true;
}

//-------------------------------------------------------------------------------------------------
//		ピニングサイトを初期位置に配置する
//-------------------------------------------------------------------------------------------------
bool MD::InitPinPos() {
	if (piningSiteNum < 0) {
		std::cout << "piningSiteNumに不正な値が入力されました" << std::endl;
		return false;
	}
	else if(piningSiteNum == 0){
		std::cout << "ピニングサイトを配置していません" << std::endl;
		noPiningSite = true;
	}
	
	piningSites = std::make_unique<PiningSite[]>(piningSiteNum);
	for (int i = 0; i < piningSiteNum; i++) {
		piningSites[i].SetPos(i, i);
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
//     時間発展させるメインループ
//-------------------------------------------------------------------------------------------------
void MD::MainLoop() {
	std::string currentTime = GetCurrentTimeStr();
	std::string output_file = "output/test_" + currentTime + ".csv";
	std::ofstream file(output_file);
	file << "time,";
	for (int i = 0; i < voltexNum; i++) {
		file << "x,y,v_x,v_y,f_x,f_y,";
	}
	file << "\n";
	
	double time = 0;
	while (time < 1) {
		CalcEOM(time);
		file << time;
		for (int i = 0; i < voltexNum; i++) {
			file << "," << voltexs[i].GetPos().x() << "," << voltexs[i].GetPos().y() 
				 << ","	<< voltexs[i].GetVelocity().x() << "," << voltexs[i].GetVelocity().y() 
				 << ","	<< voltexs[i].GetForce().x() << "," << voltexs[i].GetForce().y();
		}
		file << "\n";
		
		time += dt;
	}
	file.close();
}

//-------------------------------------------------------------------------------------------------
//		外力を0に初期化する
//-------------------------------------------------------------------------------------------------
void MD::InitForce() {
	for (int i = 0; i < voltexNum; i++) {
		voltexs[i].SetForce(0.0, 0.0);
	}
}

//-------------------------------------------------------------------------------------------------
//		ボルテックス・ボルテックス相互作用(VVI)を計算する
//-------------------------------------------------------------------------------------------------
void MD::CalcVVI() {
	for (int i = 0; i < voltexNum -1 ; i++) {
		for (int j = i+1; j < voltexNum; j++) {
			double f0 = 1.0;	//VVIの係数f0
			
			Vector2d difPos = voltexs[i].GetPos() - voltexs[j].GetPos();		//ベクトルの差
			std::cout << i << difPos.transpose() << std::endl;
			
			//周期的に繰り返すボルテックスのうち、近いボルテックスを計算する
			//周期的境界条件に対してカットオフ長さが短ければこの計算でもうまくいくが要検討
			if (difPos.x() < -weight / 2) difPos(0) += weight;
			if (difPos.x() >  weight / 2) difPos(0) -= weight;
			if (difPos.y() < -height / 2) difPos(1) += height;
			if (difPos.y() >  height / 2) difPos(1) -= height;
			
			//以下、ボルテックス同士の距離がカットオフ長さより長ければ計算しない
			if (difPos.norm() > cutoff) continue;						
			
			Vector2d force = f0 * exp(- difPos.norm() / lambda) * difPos;	//VVI
			std::cout << force.transpose() << std::endl;
			double xForce = force.x();				//VVIのx成分
			double yForce = force.y();				//VVIのy成分
			std::cout << xForce << ", " << yForce << std::endl;
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
//		粘性抵抗による力を計算する
//-------------------------------------------------------------------------------------------------
void MD::CalcResistForce() {
	double eta = 10.0;
	for (int i = 0; i < voltexNum; i++) {
		Vector2d velocity = voltexs[i].GetVelocity();	//ボルテックスの速度を取得する
		Vector2d force = -eta * velocity;				//粘性抵抗による力を計算する
		voltexs[i].AddForce(force.x(), force.y());		//ボルテックスへの外力に加える
	}
}

//-------------------------------------------------------------------------------------------------
//		サーマル力を計算する
//-------------------------------------------------------------------------------------------------
void MD::CalcThermalForce() {
	
}

//-------------------------------------------------------------------------------------------------
//		運動方程式を解いてボルテックスの位置、速度を更新する
//-------------------------------------------------------------------------------------------------
void MD::CalcEOM(double time) 
{
	//速度ベルレ法を用いた時間発展でボルテックスの位置、速度を更新する
	//r(t+dt) = r(t) + v(t)*dt + (1/2)*(F(t)/m)*dt^2
	//v(t+dt) = v(t) + (1/2)*((F(t)+F(t+dt))/m)*dt
	{
		if (time == 0) {
			return;
		}
		double eta = 10.0;							//粘性抵抗η

		unique_ptr<Vector2d[]> v1 = std::make_unique<Vector2d[]>(voltexNum);	//速度v(t)の動的配列、v(t+dt)の計算に使う
		unique_ptr<Vector2d[]> f1 = std::make_unique<Vector2d[]>(voltexNum);	//外力F(t)、v(t+dt)の計算に使う
		for (int i = 0; i < voltexNum; i++) {

			Vector2d r1 = voltexs[i].GetPos();		//位置r(t)
			v1[i] = voltexs[i].GetVelocity();		//速度v(t)
			f1[i] = voltexs[i].GetForce();			//外力F(t)

			//位置r(t+dt)を計算し、更新する
			Vector2d r2 = r1 + v1[i] * dt + (f1[i] / eta) / 2 * dt * dt;	//位置r(t+dt)の計算
			/*
			//周期的境界条件
			if (r2.x() < 0)      r2(0) += weight;
			if (r2.x() > weight) r2(0) -= weight;
			if (r2.y() < 0)      r2(1) += height;
			if (r2.y() > height) r2(1) -= height;
			*/
			voltexs[i].SetPos(r2.x(), r2.y());								//位置r(t+dt)の更新
		}

		//外力の再計算を行い、F(t+dt)を計算する
		InitForce();	//ボルテックスへの外力を初期化

		//F(t+dt)の計算
		CalcVVI();		
		//CalcResistForce();

		//v(t),F(t),F(t+dt)を用いて速度v(t+dt)を計算し、更新する
		for (int i = 0; i < voltexNum; i++) {
			
			Vector2d f2 = voltexs[i].GetForce();
			Vector2d v2 = v1[i] + (f1[i] + f2) / (2 * eta) * dt;	//速度v(t+dt)の計算
			
			//voltexs[i].SetForce(f2.x(), f2.y());					//外力F(t+dt)の更新、次の時間発展の位置r(t)計算で使う
			voltexs[i].SetVelocity(v2.x(), v2.y());					//速度v(t+dt)の更新
		}
	}
}

void MD::SaveFile() {

}

std::string MD::GetCurrentTimeStr() {
	auto now = std::chrono::system_clock::now();
	auto inTimeT = std::chrono::system_clock::to_time_t(now);

	std::tm buf;
	localtime_s(&buf, &inTimeT);
	std::stringstream ss;
	ss << std::put_time(&buf, "%Y%m%d%H%M%S");

	return ss.str();
}
