#pragma once
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>		//std::setw, std::setfill
#include <filesystem>	//
#include <regex>		//正規表現
#include "Voltex.h"

enum class OutputType
{
	position,
	velocity,
	force
};

template<typename T> using unique_ptr = std::unique_ptr<T>;

class Voltex;

class FileHandler {

public:
	//=======================================================================================
	// public variables.
	//=======================================================================================
	static int index;

	//=======================================================================================
	// public methods.
	//=======================================================================================
	FileHandler();
	~FileHandler();
	static void SetIndex(const std::string& dirName);
	static std::string GetCurrentTimeStr();
	static void CreateDir(std::string dirName);

	void CreateFile(std::string dirName, OutputType type);

	int  GetIndex() const;
	std::string GetName() const;

	
	void WriteLabel(int voltexNum);//csvファイル書き込み用、ラベルを記載する
	
	void WritePos(double time, const unique_ptr<Voltex[]>& voltexs, int voltexNum);
	void WriteVelocity(double time, const unique_ptr<Voltex[]>& voltexs, int voltexNum);
	void WriteForce(double time, const unique_ptr<Voltex[]>& voltexs, int voltexNum);

private:
	//=======================================================================================
	// private variables.
	//=======================================================================================
	std::string fileName;
	std::ofstream file;

	//=======================================================================================
	// private methods.
	//=======================================================================================
	std::string CreatePosFile(std::string dir);			//csvファイル作成用、位置を書き込むファイル
	std::string CreateVelFile(std::string dir);			//csvファイル作成用、速度を書き込むファイル
	std::string CreateForceFile(std::string dir);		//csvファイル作成用、外力を書き込むファイル
};