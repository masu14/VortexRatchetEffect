#pragma once
#include <memory>
#include "Voltex.h"
#include "PiningSite.h"

template<typename T> using unique_ptr = std::unique_ptr<T>;	//長くて冗長なので省略してるだけ

//クラスの前方宣言
class Voltex;		
class PiningSite;

/// <summary>
/// ExperimentalParam
/// 実験条件をまとめた構造体、本シミュレーションのパラメータはすべてここで管理する。
/// </summary>
struct Paramater {
	int voltexNum;				//ボルテックスの数
	int piningSiteNum;			//ピニングサイトの数
	float dt = 0.01f;			//時間変化量
	int height = 1;				//シミュレーションボックスの高さ
	int weight = 1;				//シミュレーションボックスの幅
	const int CUTOFF = 4;		//ボルテックスへ相互作用を及ぼす対象の有効範囲
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
	unique_ptr<Voltex[]> voltexs;			//ボルテックスのインスタンス、voltexNum個の配列として扱う
	unique_ptr<PiningSite[]> piningSites;	//ピニングサイトのインスタンス、piningSiteNum個の配列として扱う
	bool noPiningSite = false;					//ピニングサイト無しの場合のフラグ

	//=======================================================================================
	// private methods.
	//=======================================================================================
	unique_ptr<Voltex[]> InitVolPos(const Paramater& param);			//ボルテックスの初期配置を行う
	unique_ptr<PiningSite[]> InitPinPos(const Paramater& param);		//ピニングサイトの初期配置を行う

	float CalcVVI(Paramater param);				//ボルテックス・ボルテックス相互作用(VVI)を計算する
	float CalcPiningForce();		//ピニング力を計算する
	float CalcLorentzForce();		//ローレンツ力を計算する	
	float CalcThermalForce();		//サーマル力を計算する
	float CalcEOM();				//運動方程式を解いて位置、速度を更新する

	Vector2f AjustPeriod(Vector2f dPos, Paramater param);
};

