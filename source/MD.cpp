#include "MD.h"

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

	//パラメーターをもとに変数を設定する
	voltexNum		= param.voltexNum;
	piningSiteNum	= param.piningSiteNum;
	dt				= param.dt;
	a				= param.a;
	weight			= param.a * 3.0;
	height			= param.a * 2.0 * sqrt(3.0);
	cutoff			= param.cutoff;

	//初期化が成功したときMD法を実行する
	if (InitApp())
	{
		MainLoop();
	}

}

bool MD::InitApp() {

	// ボルテックスの初期化
	if(!InitVolPos())
	{
		std::cout << "!initvoltexpos" << std::endl;
		return false;
	}

	// ピニングサイトの初期化
	if(!InitPinPos())
	{
		std::cout << "!initpinpos" << std::endl;
		return false;
	}

	//ボルテックスへの外力を初期化
	InitForce();
	//std::cout << "mainroopに入ります" << std::endl;
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
	
	PlaceManual();		//ボルテックスが三角格子となるように配置
	
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
	
	piningSites = std::make_unique<PiningSiteCircle[]>(piningSiteNum);

	//piningSite1個配置したときの実験用コード、終わったら消す
	piningSites[0].SetPos(0.5, 0.4);
	piningSites[0].SetR(0.2);
	std::cout << "piningSite[" << 0 << "] pos" << piningSites[0].GetPos().transpose() << std::endl;
	std::cout << "piningSite[" << 0 << "] r  " << piningSites[0].GetR() << std::endl;
	return true;

	for (int i = 0; i < piningSiteNum; i++) {
		double x = voltexs[i].GetPos().x();
		double y = voltexs[i].GetPos().y();
		piningSites[i].SetPos(x, y);
		piningSites[i].SetR(a * (double)(i+1.0) / 16.0);
		std::cout << "piningSite[" << i << "] pos" << piningSites[i].GetPos().transpose() << std::endl;
		std::cout << "piningSite[" << i << "] r  " << piningSites[i].GetR() << std::endl;

	}

	return true;
}

//-------------------------------------------------------------------------------------------------
//     時間発展させるメインループ
//-------------------------------------------------------------------------------------------------
void MD::MainLoop() {

	//今日の日付のディレクトリを作成
	std::string dirName = "output/" + FileHandler::GetCurrentTimeStr();
	FileHandler::CreateDir(dirName);

	//出力ファイルの作成
	FileHandler::SetIndex(dirName);
	FileHandler filePos;
	FileHandler fileVelocity;
	FileHandler fileForce;
	
	filePos.     CreateFile(dirName, OutputType::position);
	fileVelocity.CreateFile(dirName, OutputType::velocity);
	fileForce.   CreateFile(dirName, OutputType::force);

	//ボルテックスの初期分布(位置、速度、外力)の書き込み
	//filePos.WritePos(voltexs, voltexNum);
	fileVelocity.WriteVelocity(voltexs, voltexNum);
	fileForce.WriteForce(voltexs, voltexNum);
	
	//ラベルの書き込み、簡易的なもの
	filePos.     WriteLabel(voltexNum);
	fileVelocity.WriteLabel(voltexNum);
	fileForce.   WriteLabel(voltexNum);
	
	//メインループ
	double time = 0;
	double maxtime = 10.0;
	while (time <= maxtime) {
		//運動方程式を解く
		CalcEOM(time);

		//計算結果をファイルに書き込む
		filePos.     WritePos(time, voltexs, voltexNum);
		fileVelocity.WriteVelocity(time, voltexs, voltexNum);
		fileForce.   WriteForce(time, voltexs, voltexNum);

		time += dt;
	}

	//最終的なボルテックスの分布(位置、速度、外力)の書き込み
	filePos.WritePos(voltexs, voltexNum);
	fileVelocity.WriteVelocity(voltexs, voltexNum);
	fileForce.WriteForce(voltexs, voltexNum);
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
			double f0 = 10.0;	//VVIの係数f0
			
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
			
			Vector2d force = f0 * exp(- difPos.norm() / lambda) * difPos/difPos.norm();	//VVIの式を用いた
			
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
	double kp = 1.0;	//kpはピニング力の大きさを決める係数
	double lp = 1.0;	//lpはピニングサイトにおける常伝導から超伝導への回復長
	
	for (int i = 0; i < voltexNum; i++) {
		for (int j = 0; j < piningSiteNum; j++) {
			Vector2d difPos = voltexs[i].GetPos() - piningSites[j].GetPos();

			//if (difPos.x() < -weight / 2) difPos(0) += weight;
			//if (difPos.x() > weight / 2) difPos(0) -= weight;
			//if (difPos.y() < -height / 2) difPos(1) += height;
			//if (difPos.y() > height / 2) difPos(1) -= height;

			if (difPos.norm() < piningSites[j].GetR()) continue;
			//if (difPos.norm() > cutoff) continue;

			Vector2d force = -kp / pow(cosh((difPos.norm() - piningSites[j].GetR()) / lp), 2.0) * difPos/difPos.norm();
			double xForce = force.x();
			double yForce = force.y();
			voltexs[i].AddForce(xForce, yForce);
		}
	}
}

//-------------------------------------------------------------------------------------------------
//		ローレンツ力を計算する	
//-------------------------------------------------------------------------------------------------
void MD::CalcLorentzForce() {
	double force = 1.0;
	for (int i = 0; i < voltexNum; i++) {
		voltexs[i].AddForce(force, 0.0);
	}
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
			
			//周期的境界条件
			//if (r2.x() < 0)      r2(0) += weight;
			//if (r2.x() > weight) r2(0) -= weight;
			//if (r2.y() < 0)      r2(1) += height;
			//if (r2.y() > height) r2(1) -= height;
			
			voltexs[i].SetPos(r2.x(), r2.y());								//位置r(t+dt)の更新
		}

		//外力の再計算を行い、F(t+dt)を計算する
		InitForce();	//ボルテックスへの外力を初期化

		//F(t+dt)の計算
		//CalcVVI();
		CalcPiningForce();
		//CalcLorentzForce();
		//CalcResistForce();

		//v(t),F(t),F(t+dt)を用いて速度v(t+dt)を計算し、更新する
		for (int i = 0; i < voltexNum; i++) {
			
			Vector2d f2 = voltexs[i].GetForce();
			Vector2d v2 = v1[i] + (f1[i] + f2) / (2 * eta) * dt;	//速度v(t+dt)の計算
			
			voltexs[i].SetForce(f2.x(), f2.y());					//外力F(t+dt)の更新、次の時間発展の位置r(t)計算で使う
			voltexs[i].SetVelocity(v2.x(), v2.y());					//速度v(t+dt)の更新
		}
	}
}


//------------------------------------------------------------------------------------------------
//    ボルテックスの初期配置を三角格子にする
//	  ボルテックスの数が6の倍数のときに使うようにする
//------------------------------------------------------------------------------------------------
void MD::PlaceTriangle() {
	double y = a * sqrt(3.0) / 4.0;
	for (int i = 0; i < voltexNum / 3.0; i++) { 
		double x = a / 4.0;
		if (i % 2 == 1) x += a / 2.0;
		
		for (int j = 0; j < 3; j++) {
			voltexs[3 * i + j].SetPos(x + a * (double)j, y + sqrt(3) / 2.0 * a * (double)i);
		}
	}
}

//-----------------------------------------------------------------------------------------------
//    ボルテックスの初期配置をランダムにする
//-----------------------------------------------------------------------------------------------
void MD::PlaceRandom() {

	std::random_device rd;
	std::mt19937_64 gen(rd());
	double xmin = 0.0;
	double xmax = weight;
	double ymin = 0.0;
	double ymax = height;
	std::uniform_real_distribution<> xdis(xmin, xmax);
	std::uniform_real_distribution<> ydis(ymin, ymax);
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			double x = xdis(gen);
			double y = ydis(gen);
			voltexs[3 * i + j].SetPos(x, y);
		}
		
	}
}

//-----------------------------------------------------------------------------------------------
//    デバック用、配列の長さに注意
//　　特定の配置で外力項を変えて実験したいときに使う
//-----------------------------------------------------------------------------------------------
void MD::PlaceManual()
{
	voltexs[0].SetPos(0.1,0.4);
	/*voltexs[1].SetPos(0.4, 0.4);
	voltexs[2].SetPos(0.744743, 0.180336);
	voltexs[3].SetPos(0.172043,0.705768);
	voltexs[4].SetPos(0.135809,0.483818);
	voltexs[5].SetPos(0.550069,0.052258);
	voltexs[6].SetPos(0.551065,0.176116);
	voltexs[7].SetPos(0.292968,0.059621);
	voltexs[8].SetPos(0.081272,0.490255);
	voltexs[9].SetPos(0.351897,0.686276);
	voltexs[10].SetPos(0.113886,0.285068);
	voltexs[11].SetPos(0.610364,0.741576);*/
	
}