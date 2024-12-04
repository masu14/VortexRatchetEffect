#pragma once
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>		//std::setw, std::setfill
#include <filesystem>	//
#include <regex>		//正規表現
#include "Vortex.h"
#include "PiningSite.h"
#include "InputReader.h"

enum class OutputType
{
	position,
	velocity,
	force
};

template<typename T> using unique_ptr = std::unique_ptr<T>;

class Vortex;
class PiningSiteCircle;

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
	static std::string FixedValueStr(int num, double var);

	void CreateFile(std::string dirName, OutputType type);
	void CreateFile(std::string dirName, std::string filename);

	static std::string  GetIndex();
	std::string GetName() const;
	
	void WriteParam(Paramater<double> param);
	void WritePinPos(const unique_ptr<PiningSiteCircle[]>& piningSites, int pinNum);
	void WriteLabel(int vortexNum);//csvファイル書き込み用、ラベルを記載する
	
	void WritePos(double time, const unique_ptr<Vortex[]>& vortexs, int vortexNum);
	void WriteVelocity(double time, const unique_ptr<Vortex[]>& vortexs, int vortexNum);
	void WriteForce(double time, const unique_ptr<Vortex[]>& vortexs, int vortexNum);

	void WritePos(const unique_ptr<Vortex[]>& vortexs, int vortexNum);
	void WriteVelocity(const unique_ptr<Vortex[]>& vortexs, int vortexNum);
	void WriteForce(const unique_ptr<Vortex[]>& vortexs, int vortexNum);

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