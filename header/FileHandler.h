#pragma once
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>		//std::setw, std::setfill
#include <filesystem>	//
#include <regex>		//正規表現
#include "Voltex.h"

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

	void SetIndex(std::string dirName);
	void SetName(std::string fileName);

	int  GetIndex() const;
	std::string GetName() const;

	std::string CreateFilePos(std::string dir);			//csvファイル作成用、位置を書き込むファイル
	void WriteLabel(std::ofstream& file, int voltexNum);//csvファイル書き込み用、ラベルを記載する
	void WriteAll(std::ofstream& file, double time);	//csvファイル書き込み用、ボルテックスの位置、速度、外力を書き込む
	void WritePos(std::ofstream& file, double time, const unique_ptr<Voltex[]>& voltexs, int voltexNum);

private:
	//=======================================================================================
	// private variables.
	//=======================================================================================
	std::string fileName;

	//=======================================================================================
	// private methods.
	//=======================================================================================
						//csvファイル書き込み用、ボルテックスの位置を書き込む
};