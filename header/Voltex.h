﻿// 1. マクロを定義．
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

class Voltex {

public:
	//=======================================================================================
	// public variables.
	//=======================================================================================
	/* NOTHING */

	//=======================================================================================
	// public methods.
	//=======================================================================================
	Voltex();		//コンストラクタでボルテックスの速度を初期化する
	~Voltex();
	
	Vector2f GetPos();
	Vector2f GetVelocity();
	Vector2f GetForce();

	void SetPos(float x, float y);
	void SetVelocity(float x, float y);
	void SetForce(float x, float y);
	void AddForce(float x, float y);
	

private:
	//=======================================================================================
	// private variables.
	//=======================================================================================
	float eta = 10;							//ボルテックスの粘性抵抗
	Vector2f position;							//ボルテックスの位置
	Vector2f velocity = { 0.0f, 0.0f };		//ボルテックスの速度
	Vector2f force = { 0.0f, 0.0f };			//ボルテックスへの外力

	//=======================================================================================
	// private methods.
	//=======================================================================================
	/* NOTHING */
};

	
