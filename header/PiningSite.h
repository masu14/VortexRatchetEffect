// 1. マクロを定義．
#define EIGEN_NO_DEBUG // コード内のassertを無効化．
#define EIGEN_DONT_VECTORIZE // SIMDを無効化．
#define EIGEN_DONT_PARALLELIZE // 並列を無効化．
#define EIGEN_MPL2_ONLY // LGPLライセンスのコードを使わない．

#pragma once
#include <iostream>
#include <math.h>			//基本的な関数を扱うための標準ライブラリ
#include <Eigen/Core>		//ベクトル演算を行うためのライブラリ

using namespace Eigen;


/// <summary>
/// PiningSite
/// ピニングサイトの座標や形状の情報をまとめたクラス、とりあえず円形のピニングサイトを考えるが
/// 他の形状を作る可能性があるため継承を使って書き直す予定
/// </summary>
class PiningSite {
public:
	//=========================================================================================
	// public variables
	//=========================================================================================
	static double potentialE;		//ピニングサイトが形成するポテンシャルエネルギー
	static double kp;
	static double lp;

	//=========================================================================================
	// public methods
	//=========================================================================================
	PiningSite();				//不正な値で初期化(初期化もれした際にエラーを出すため)
	~PiningSite();

	static void Setkp(double kp);
	static void Setlp(double lp);

	Vector2d GetPos();					//ピニングサイトの中心座標を得る
	void SetPos(double x, double y);	//ピニングサイトの座標を書き込む、初期化以降は変更しない
	void AddPos(double x, double y);

	double GetPotentialE();
	void SetPotentialE(double energy);
	void AddPotentialE(double energy);
	virtual Vector2d CalcPiningForce(Vector2d difPos) const = 0;	//派生先の形状に適した計算を行う
	virtual double CalcPotentialE(Vector2d pos) const = 0;		//派生先の形状に適した計算を行う

private:
	//=========================================================================================
	// private variables
	//=========================================================================================
	Vector2d position;		//ピニングサイトの座標
	

	//=========================================================================================
	// private methods
	//=========================================================================================
};

/// <summary>
/// 円形ピニングサイトのクラス
/// PiningSiteクラスを継承している
/// </summary>
class PiningSiteCircle : public PiningSite 
{
public:
	//=========================================================================================
	// public variables
	//=========================================================================================
	/* NOTHING */

	//=========================================================================================
	// public methods
	//=========================================================================================
	PiningSiteCircle();
	~PiningSiteCircle();

	double GetR();
	void SetR(double r);

	Vector2d CalcPiningForce(Vector2d difPos) const override;
	double CalcPotentialE(Vector2d pos) const override;
private:
	//=========================================================================================
	// private variables
	//=========================================================================================
	double r=-1.0;

	//=========================================================================================
	// private methods
	//=========================================================================================
};

/// <summary>
/// 円形ピニングサイトのクラス
/// PiningSiteクラスを継承している
/// </summary>
class PiningSiteLine : public PiningSite
{
public:
	//=========================================================================================
	// public variables
	//=========================================================================================
	/* NOTHING */

	//=========================================================================================
	// public methods
	//=========================================================================================
	PiningSiteLine();
	~PiningSiteLine();

	double GetLength();
	void SetLength(double length);

	Vector2d CalcPiningForce(Vector2d difPos) const override;
	double CalcPotentialE(Vector2d pos) const override;



private:
	//=========================================================================================
	// private variables
	//=========================================================================================
	double length = -1.0;

	//=========================================================================================
	// private methods
	//=========================================================================================

};