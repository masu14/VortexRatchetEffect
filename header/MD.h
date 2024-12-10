#pragma once
#include <memory>
#include <math.h>			//基本的な関数を扱うための標準ライブラリ
#include <random>
#include "Vortex.h"
#include "PiningSite.h"
#include "FileHandler.h"
#include "InputReader.h"	//Paramater構造体をとる

template<typename T> using unique_ptr = std::unique_ptr<T>;		//長くて冗長なので「std::」を省略してるだけ


//クラスの前方宣言
class Vortex;		
class PiningSite;
class PiningSiteCircle;

enum class PiningType
{
	tripleCircle,
	doubleCircle,
};

/// <summary>
/// 分子動力学法のマネージャークラス、各ボルテックス、ピニングサイトの管理をする
/// ボルテックス、ピニングサイトの初期条件を決め、コンストラクタでインスタンスを生成するし、
/// デストラクタでインスタンスを削除する
/// </summary>
class MD{
public:
	//=======================================================================================
	// public variables.
	//=======================================================================================
	/* NOTHING */

	//=======================================================================================
	// public methods.
	//=======================================================================================
	MD();			//ボルテックス、ピニングサイトの初期化
	~MD();			//インスタンスを削除する
	void Run(Paramater<double> param);		//分子動力学法の実行

private:
	//=======================================================================================
	// private variables.
	//=======================================================================================
	std::string EOM;			//運動方程式の種類、ordinary or overdamp
	std::string condition;		//実験条件
	int vortexNum;				//ボルテックスの数
	int piningSiteNum;			//ピニングサイトの数
	double dt = 0.001;			//時間変化量
	double maxTime = 10.0;		//計算時間
	double a = 0.25;			//初期配置時のボルテックス間距離(三角格子)
	double height = 5;			//シミュレーションボックスの高さ
	double weight = 5;			//シミュレーションボックスの幅
	int cutoff = 4;				//ボルテックスへ相互作用を及ぼす対象の有効範囲
	double eta = 1.0;			//粘性抵抗η
	double lorentzForce;		//ローレンツ力の大きさ
	double siteDistance;		//対象のピニングサイトのずらす値
	double annealTime;			//初めにローレンツ力をかけずにいる時間
	double lorentzFrequency;	//ローレンツ力の向きを切り替える時間
	double f0;					//VVIの係数の大きさ
	double kp;					//ピニング力の係数の大きさ
	double lp;					//
	std::string var1name;
	std::string var2name;

	PiningType piningType;

	unique_ptr<Vortex[]> vortexs;					//ボルテックスのインスタンス、　vortexNum個の配列として扱う
	unique_ptr<PiningSiteCircle[]> piningSites;		//ピニングサイトのインスタンス、piningSiteNum個の配列として扱う
	bool noPiningSite = false;						//ピニングサイト無しの場合のフラグ
	double lambda = 1.0;

	//=======================================================================================
	// private methods.
	//=======================================================================================
	bool InitApp();
	void MainLoop();

	bool InitVorPos();			//ボルテックスの初期配置を行う
	bool InitPinPos();			//ピニングサイトの初期配置を行う

	
	void InitForce();						//外力を0に初期化する
	void CalcVVI();							//ボルテックス・ボルテックス相互作用(VVI)を計算する
	void CalcPiningForce();					//ピニング力を計算する
	void CalcLorentzForce(double time);				//ローレンツ力を計算する	
	void CalcResistForce();					//粘性抵抗による力を計算する
	void CalcThermalForce();				//サーマル力を計算する
	void CalcEOM(double time);				//運動方程式を解いて位置、速度、加速度を更新する
	void CalcEOMOverDamp(double time);

	void PlaceVorTriangle();		//ボルテックスの初期配置を三角格子にする
	void PlaceVorSquare();
	void PlaceVorRandom();			//ボルテックスの初期配置をランダムにする
	void PlaceVorManual();			//ボルテックスの初期配置を一つずつ指定する

	PiningType SetPinType();
	void PlacePin();
	void PlaceCirclePinTriple();
	void PlaceCirclePinDouble();
	void ShiftCirclePinTriple();
	void ShiftCirclePinDouble();

	std::string SetVariableName(std::string varname);

};

