#pragma once
#include <memory>
#include <math.h>			//基本的な関数を扱うための標準ライブラリ
#include "Voltex.h"
#include "PiningSite.h"

template<typename T> using unique_ptr = std::unique_ptr<T>;		//長くて冗長なので「std::」を省略してるだけ


//クラスの前方宣言
class Voltex;		
class PiningSite;

/// <summary>
/// Paramater構造体
/// 実験条件をまとめた構造体、本シミュレーションのパラメータはすべてここで管理する。
/// </summary>
struct Paramater {
	int voltexNum;				//ボルテックスの数
	int piningSiteNum;			//ピニングサイトの数
	double dt = 0.001;			//時間変化量
	double height = 5;				//シミュレーションボックスの高さ
	double weight = 5;				//シミュレーションボックスの幅
	int cutoff = 4;		//ボルテックスへ相互作用を及ぼす対象の有効範囲
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
	void Run(Paramater param);		//分子動力学法の実行

private:
	//=======================================================================================
	// private variables.
	//=======================================================================================
	int voltexNum;				//ボルテックスの数
	int piningSiteNum;			//ピニングサイトの数
	double dt = 0.001;			//時間変化量
	double height = 5;				//シミュレーションボックスの高さ
	double weight = 5;				//シミュレーションボックスの幅
	int cutoff = 4;		//ボルテックスへ相互作用を及ぼす対象の有効範囲

	unique_ptr<Voltex[]> voltexs;			//ボルテックスのインスタンス、voltexNum個の配列として扱う
	
	unique_ptr<PiningSite[]> piningSites;	//ピニングサイトのインスタンス、piningSiteNum個の配列として扱う
	bool noPiningSite = false;					//ピニングサイト無しの場合のフラグ
	double lambda = 1.0;

	//=======================================================================================
	// private methods.
	//=======================================================================================
	bool InitApp();
	bool InitVolPos();			//ボルテックスの初期配置を行う
	bool InitPinPos();		//ピニングサイトの初期配置を行う

	void MainLoop();
	void InitForce();							//外力を0に初期化する
	void CalcVVI();		//ボルテックス・ボルテックス相互作用(VVI)を計算する
	void CalcPiningForce();						//ピニング力を計算する
	void CalcLorentzForce();					//ローレンツ力を計算する	
	void CalcResistForce();						//粘性抵抗による力を計算する
	void CalcThermalForce();					//サーマル力を計算する
	void CalcEOM(double time);				//運動方程式を解いて位置、速度を更新する
	std::string GetCurrentTimeStr();		//ファイル出力用、現在時刻を取得し文字列で返す
	void SaveFile();
	
	//void TermApp();
};

