#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>		// 文字列を可変長の配列で処理する
#include <map>			// 辞書(key, value)を使う
#include <algorithm>	// std::remove_if 入力データの文字列処理
#include <cctype>		// std::isspace   入力データの文字列処理

//型エイリアス
using string = std::string;

/// <summary>
/// Paramater構造体
/// 実験条件をまとめた構造体、本シミュレーションのパラメータはすべてここで管理する。
/// <summary>
template <typename T>
struct Paramater {
	int vortexNum;				//ボルテックスの数
	int piningSiteNum;			//ピニングサイトの数
	double dt = 0.001;			//時間変化量
	double maxTime = 10.0;		//計算時間
	double a = 0.25;			//初期配置時のボルテックス間距離(三角格子)
	double weight;				//周期的境界条件の横幅
	double height;				//周期的境界条件の縦幅
	int cutoff = 4;				//ボルテックスへ相互作用を及ぼす対象の有効範囲
	double eta = 1.0;			//粘性抵抗η
	double lorentzForce;		//ローレンツ力の大きさ
	double siteDistance;		//ピニングサイト間の距離
	double annealTime;			//アニールにかける時間
	double lorentzFrequency;	//ローレンツ力の周波数
	double f0;					//VVIの係数
	double kp;					//ピニング力の大きさを決める係数
	double lp;					//ピニングサイトにおける常伝導から超伝導への回復長

	string EOM;
	string condition;

	string var1name;
	string var2name;
	std::array<T,3> var1;				//ループ用、配列には[start, end, step]を入れる
	std::array<T,3> var2;				//ループ用、配列には[start, end, step]を入れる

	bool outPosition;
	bool outVelocity;
	bool outForce;
	bool outPinPotential;
};


/// <summary>
/// InputReaderクラス
/// </summary>
class InputReader
{
public:
	//=========================================================================================
	// public variables
	//=========================================================================================
	

	//=========================================================================================
	// public methods
	//=========================================================================================
	InputReader();
	~InputReader();

	Paramater<double> GetParam();
	void ReadParam(const string& filename);
	
private:
	//=========================================================================================
	// private variables
	//=========================================================================================
	std::map<string, std::map<string, string>> sections;
	Paramater<double> param;

	//=========================================================================================
	// private methods
	//=========================================================================================
	template <typename T>
	T					StringToNumber(const string& str);		// 文字列を数値に変換する関数
	template <typename T>
	std::array<T,3>		ReadRange(const string& str);			// 文字列から変数の定義域と刻み幅を取得する関数
	bool				stringToBool(const string& str);		// 文字列を真偽値に変換する関数
	std::map <string, std::map<string, string>> ReadInputFile(const string& filename);	//入力ファイルを読み込む関数

};