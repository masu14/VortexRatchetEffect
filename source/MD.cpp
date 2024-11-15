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
void MD::Run(Paramater<double> param) {

	//パラメーターをもとに変数を設定する
	vortexNum		= param.vortexNum;
	piningSiteNum	= param.piningSiteNum;
	dt				= param.dt;
	maxTime			= param.maxTime;
	a				= param.a;
	weight			= param.a * 3.0;
	height			= param.a * 2.0 * sqrt(3.0) * vortexNum / 12.0;
	cutoff			= param.cutoff;
	eta				= param.eta;
	lorentzForce    = param.lorentzForce;
	

	//初期化が成功したときMD法を実行する
	if (InitApp())
	{
		MainLoop();
	}

}

bool MD::InitApp() {

	// ピニングサイトの初期化
	if(!InitPinPos())
	{
		std::cout << "!initpinpos" << std::endl;
		return false;
	}

	// ボルテックスの初期化
	if (!InitVorPos())
	{
		std::cout << "!initvortexpos" << std::endl;
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
bool MD::InitVorPos() {
	if (vortexNum <= 0) {
		std::cout << "vortexNumに不正な値が入力されました" << std::endl;
		return false;
	}
	vortexs = std::make_unique<Vortex[]>(vortexNum);
	
	PlaceVorSquare();		//ボルテックスが三角格子となるように配置
	
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
		return true;
	}
	
	piningSites = std::make_unique<PiningSiteCircle[]>(piningSiteNum);

	PlacePinManual();
	return true;

}

//-------------------------------------------------------------------------------------------------
//     時間発展させるメインループ
//-------------------------------------------------------------------------------------------------
void MD::MainLoop() {

	//今日の日付のディレクトリを作成
	std::string dirName = "../output/" + FileHandler::GetCurrentTimeStr();
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
	//filePos.WritePos(vortexs, vortexNum);
	//fileVelocity.WriteVelocity(vortexs, vortexNum);
	//fileForce.WriteForce(vortexs, vortexNum);
	
	//ラベルの書き込み、簡易的なもの
	filePos.WritePinPos(piningSites, piningSiteNum);
	filePos.     WriteLabel(vortexNum);
	fileVelocity.WriteLabel(vortexNum);
	fileForce.   WriteLabel(vortexNum);
	
	//メインループ
	double time = 0;
	
	while (time <= maxTime) {
		//運動方程式を解く
		CalcEOM(time);

		//計算結果をファイルに書き込む
		filePos.     WritePos(time, vortexs, vortexNum);
		fileVelocity.WriteVelocity(time, vortexs, vortexNum);
		fileForce.   WriteForce(time, vortexs, vortexNum);

		time += dt;
	}

	//最終的なボルテックスの分布(位置、速度、外力)の書き込み
	//filePos.WritePos(vortexs, vortexNum);
	//fileVelocity.WriteVelocity(vortexs, vortexNum);
	//fileForce.WriteForce(vortexs, vortexNum);
}

//-------------------------------------------------------------------------------------------------
//		外力を0に初期化する
//-------------------------------------------------------------------------------------------------
void MD::InitForce() {
	for (int i = 0; i < vortexNum; i++) {
		vortexs[i].SetForce(0.0, 0.0);
	}
}

//-------------------------------------------------------------------------------------------------
//		ボルテックス・ボルテックス相互作用(VVI)を計算する
//-------------------------------------------------------------------------------------------------
void MD::CalcVVI() {
	for (int i = 0; i < vortexNum -1 ; i++) {
		for (int j = i+1; j < vortexNum; j++) {
			double f0 = 3.0;	//VVIの係数f0
			
			Vector2d difPos = vortexs[i].GetPos() - vortexs[j].GetPos();		//ベクトルの差
			//std::cout << i << difPos.transpose() << std::endl;
			
			//周期的に繰り返すボルテックスのうち、近いボルテックスを計算する
			//周期的境界条件に対してカットオフ長さが短ければこの計算でもうまくいくが要検討
			if (difPos.x() < -weight / 2) difPos(0) += weight;
			if (difPos.x() >  weight / 2) difPos(0) -= weight;
			if (difPos.y() < -height / 2) difPos(1) += height;
			if (difPos.y() >  height / 2) difPos(1) -= height;
			
			//以下、ボルテックス同士の距離がカットオフ長さより長ければ計算しない
			//if (difPos.norm() > cutoff) continue;						
			
			Vector2d force = f0 * exp(- difPos.norm() / lambda) * difPos/difPos.norm();	//VVIの式を用いた
			
			double xForce = force.x();				//VVIのx成分
			double yForce = force.y();				//VVIのy成分
			
			vortexs[i].AddForce(xForce, yForce);	//作用
			vortexs[j].AddForce(-xForce, -yForce);	//反作用
		}
	}
	
}

//-------------------------------------------------------------------------------------------------
//		ピニング力を計算する
//-------------------------------------------------------------------------------------------------
void MD::CalcPiningForce() {
	double kp = 1.0;	//kpはピニング力の大きさを決める係数
	double lp = 0.3*sqrt(2.0);	//lpはピニングサイトにおける常伝導から超伝導への回復長
	
	for (int i = 0; i < vortexNum; i++) {
		for (int j = 0; j < piningSiteNum; j++) {
			Vector2d difPos = vortexs[i].GetPos() - piningSites[j].GetPos();

			if (difPos.x() < -weight / 2) difPos(0) += weight;
			if (difPos.x() > weight / 2) difPos(0) -= weight;
			//if (difPos.y() < -height / 2) difPos(1) += height;
			//if (difPos.y() > height / 2) difPos(1) -= height;

			if (difPos.norm() < piningSites[j].GetR()) continue;
			if (difPos.norm() > cutoff) continue;

			Vector2d force = -kp / pow(cosh((difPos.norm() - piningSites[j].GetR()) / lp), 2.0) * difPos/difPos.norm();
			double xForce = force.x();
			double yForce = force.y();
			vortexs[i].AddForce(xForce, yForce);
		}
	}
}

//-------------------------------------------------------------------------------------------------
//		ローレンツ力を計算する	
//-------------------------------------------------------------------------------------------------
void MD::CalcLorentzForce() {
	for (int i = 0; i < vortexNum; i++) {
		vortexs[i].AddForce(lorentzForce, 0.0);
	}
}

//-------------------------------------------------------------------------------------------------
//		粘性抵抗による力を計算する
//-------------------------------------------------------------------------------------------------
void MD::CalcResistForce() {
	for (int i = 0; i < vortexNum; i++) {
		Vector2d velocity = vortexs[i].GetVelocity();	//ボルテックスの速度を取得する
		Vector2d force = -eta * velocity;				//粘性抵抗による力を計算する
		vortexs[i].AddForce(force.x(), force.y());		//ボルテックスへの外力に加える
	}
}

//-------------------------------------------------------------------------------------------------
//		サーマル力を計算する
//-------------------------------------------------------------------------------------------------
void MD::CalcThermalForce() {
	
}

//-------------------------------------------------------------------------------------------------
//		運動方程式を解いてボルテックスの位置、速度、外力を更新する
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

		unique_ptr<Vector2d[]> v1 = std::make_unique<Vector2d[]>(vortexNum);	//速度v(t)の動的配列、v(t+dt)の計算に使う
		unique_ptr<Vector2d[]> f1 = std::make_unique<Vector2d[]>(vortexNum);	//外力F(t)、v(t+dt)の計算に使う
		for (int i = 0; i < vortexNum; i++) {

			Vector2d r1 = vortexs[i].GetPos();		//位置r(t)
			v1[i] = vortexs[i].GetVelocity();		//速度v(t)
			f1[i] = vortexs[i].GetForce();			//外力F(t)

			//位置r(t+dt)を計算し、更新する
			Vector2d r2 = r1 + v1[i] * dt + (f1[i] / eta) / 2 * dt * dt;	//位置r(t+dt)の計算
			
			//周期的境界条件
			if (r2.x() < 0)      r2(0) += weight;
			if (r2.x() > weight) r2(0) -= weight;
			if (r2.y() < 0)      r2(1) += height;
			if (r2.y() > height) r2(1) -= height;
			
			vortexs[i].SetPos(r2.x(), r2.y());								//位置r(t+dt)の更新
		}

		//外力の再計算を行い、F(t+dt)を計算する
		InitForce();	//ボルテックスへの外力を初期化

		//F(t+dt)の計算
		CalcVVI();
		CalcPiningForce();
		CalcLorentzForce();
		CalcResistForce();

		//v(t),F(t),F(t+dt)を用いて速度v(t+dt)を計算し、更新する
		for (int i = 0; i < vortexNum; i++) {
			
			Vector2d f2 = vortexs[i].GetForce();
			Vector2d v2 = v1[i] + (f1[i] + f2) / (2 * eta) * dt;	//速度v(t+dt)の計算
			
			vortexs[i].SetForce(f2.x(), f2.y());					//外力F(t+dt)の更新、次の時間発展の位置r(t)計算で使う
			vortexs[i].SetVelocity(v2.x(), v2.y());					//速度v(t+dt)の更新
		}
	}
}

//-------------------------------------------------------------------------------------------------
//		終端速度に達した際の過減衰運動方程式を解いてボルテックスの位置、速度、外力を更新する
//-------------------------------------------------------------------------------------------------
void MD::CalcEOMOverDamp(double time) 
{
	//終端速度に達した際の速度からボルテックスの位置、速度を更新する
	//v(t+dt) = F(t+dt) / m
	//r(t+dt) = r(t) + v(t+dt)*dt + (1/2)*((F(t+dt)-F(t))/m)
	if (time == 0) {
		return;
	}
	double eta = 1.0;	//粘性抵抗η

	unique_ptr<Vector2d[]> f1 = std::make_unique<Vector2d[]>(vortexNum);
	for (int i = 0; i < vortexNum; i++) {

		f1[i] = vortexs[i].GetForce();	//f(t)の取得
	}

	InitForce();	//ボルテックスへの外力を初期化

	//F(t+dt)の計算
	CalcVVI();
	CalcPiningForce();
	CalcLorentzForce();
	
	//終端速度を求め、そこから位置を求める
	for (int i = 0; i < vortexNum; i++) {
		Vector2d r1 = vortexs[i].GetPos();							//r(t)の取得
		Vector2d f2 = vortexs[i].GetForce();						//f(t+dt)の取得
		Vector2d v2 = f2 / eta;										//v(t+dt)の計算
		Vector2d r2 = r1 + v2 * dt + (f2 - f1[i]) / (2 * eta) * dt;	//r(t+dt)の計算

		//周期的境界条件
		if (r2.x() < 0)      r2(0) += weight;
		if (r2.x() > weight) r2(0) -= weight;
		if (r2.y() < 0)      r2(1) += height;
		if (r2.y() > height) r2(1) -= height;

		vortexs[i].SetForce(f2.x(), f2.y());		//外力の更新
		vortexs[i].SetVelocity(v2.x(), v2.y());		//速度の更新
		vortexs[i].SetPos(r2.x(), r2.y());			//位置の更新
	}
}

//------------------------------------------------------------------------------------------------
//    ボルテックスの初期配置を三角格子にする
//	  ボルテックスの数が6の倍数のときに使うようにする
//------------------------------------------------------------------------------------------------
void MD::PlaceVorTriangle() {
	double y = a * sqrt(3.0) / 4.0;
	for (int i = 0; i < vortexNum / 3.0; i++) { 
		double x = a / 4.0;
		if (i % 2 == 1) x += a / 2.0;
		
		for (int j = 0; j < 3; j++) {
			vortexs[3 * i + j].SetPos(x + a * (double)j, y + sqrt(3) / 2.0 * a * (double)i);
		}
	}
}

//------------------------------------------------------------------------------------------------
//    ボルテックスの初期配置を長方形配置にする
//	  ボルテックスの数が6の倍数のときに使うようにする
//------------------------------------------------------------------------------------------------
void MD::PlaceVorSquare() {
	double y = a * sqrt(3.0) / 4.0;
	for (int i = 0; i < vortexNum / 3.0; i++) {
		double x = a / 4.0;
		for (int j = 0; j < 3; j++) {
			vortexs[3 * i + j].SetPos(x + a * (double)j, y + sqrt(3) / 2.0 * a * (double)i);
		}
	}
}

//-----------------------------------------------------------------------------------------------
//    ボルテックスの初期配置をランダムにする
//-----------------------------------------------------------------------------------------------
void MD::PlaceVorRandom() {

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
			vortexs[3 * i + j].SetPos(x, y);
		}
		
	}
}

//-----------------------------------------------------------------------------------------------
//    デバック用、配列の長さに注意
//　　特定の配置で外力項を変えて実験したいときに使う
//-----------------------------------------------------------------------------------------------
void MD::PlaceVorManual()
{
	vortexs[0].SetPos(0.01, 0.108253);
	/*vortexs[1].SetPos(0.4, 0.4);
	vortexs[2].SetPos(0.744743, 0.180336);
	vortexs[3].SetPos(0.172043,0.705768);
	vortexs[4].SetPos(0.135809,0.483818);
	vortexs[5].SetPos(0.550069,0.052258);
	vortexs[6].SetPos(0.551065,0.176116);
	vortexs[7].SetPos(0.292968,0.059621);
	vortexs[8].SetPos(0.081272,0.490255);
	vortexs[9].SetPos(0.351897,0.686276);
	vortexs[10].SetPos(0.113886,0.285068);
	vortexs[11].SetPos(0.610364,0.741576);*/
	
}

//-----------------------------------------------------------------------------------------------
//    ピニングサイトの初期配置
//-----------------------------------------------------------------------------------------------
void MD::PlacePin()
{
	for (int i = 0; i < piningSiteNum; i++) {
		double x = vortexs[i].GetPos().x();
		double y = vortexs[i].GetPos().y();
		piningSites[i].SetPos(x, y);
		piningSites[i].SetR(a * (double)(i + 1.0) / 16.0);
		std::cout << "piningSite[" << i << "] pos" << piningSites[i].GetPos().transpose() << std::endl;
		std::cout << "piningSite[" << i << "] r  " << piningSites[i].GetR() << std::endl;

	}
}

//-----------------------------------------------------------------------------------------------
//    デバック用、配列の長さに注意
//　　特定の配置で外力項を変えて実験したいときに使う
//-----------------------------------------------------------------------------------------------
void MD::PlacePinManual()
{
	piningSites[0].SetPos(1.5, 2.6);
	piningSites[1].SetPos(7.5, 2.6);
	piningSites[2].SetPos(13.5, 2.6);
	piningSites[3].SetPos(1.5, 7.8);
	piningSites[4].SetPos(7.5, 7.8);
	piningSites[5].SetPos(13.5, 7.8);

	piningSites[0].SetR(1.5);
	piningSites[1].SetR(1.0);
	piningSites[2].SetR(0.5);
	piningSites[3].SetR(1.5);
	piningSites[4].SetR(1.0);
	piningSites[5].SetR(0.5);
}