// 1. マクロを定義．
#define EIGEN_NO_DEBUG // コード内のassertを無効化．
#define EIGEN_DONT_VECTORIZE // SIMDを無効化．
#define EIGEN_DONT_PARALLELIZE // 並列を無効化．
#define EIGEN_MPL2_ONLY // LGPLライセンスのコードを使わない．

#pragma once
#include <iostream>
#include <Eigen/Core>		//ベクトル、行列演算を行うためのライブラリ

using namespace Eigen;


/// <summary>
/// Voltex class
/// ボルテックスの位置、粘性(運動方程式における質量に相当するもの)、外部から加わる力から
/// 運動方程式を解き、ボルテックスの速度変化の時間発展を計算する
/// 
/// </summary>

class Vortex {

public:
	//=======================================================================================
	// public variables.
	//=======================================================================================
	/* NOTHING */

	//=======================================================================================
	// public methods.
	//=======================================================================================
	Vortex();		//コンストラクタでボルテックスの速度を初期化する
	~Vortex();
	
	Vector2d GetPos() const;
	Vector2d GetVelocity();
	Vector2d GetForce();

	void SetPos(double x, double y);
	void SetVelocity(double x, double y);
	void SetForce(double x, double y);
	void AddForce(double x, double y);
	

private:
	//=======================================================================================
	// private variables.
	//=======================================================================================
	double eta = 10.0;						//ボルテックスの粘性抵抗
	Vector2d position;						//ボルテックスの位置
	Vector2d velocity = { 0.0, 0.0 };		//ボルテックスの速度
	Vector2d force = { 0.0, 0.0 };			//ボルテックスへの外力

	//=======================================================================================
	// private methods.
	//=======================================================================================
	/* NOTHING */
};

	
