#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <numeric>
#include <regex>

class Analysis {

public:
	//=======================================================================================
	// public variables.
	//=======================================================================================
	void MakeVelFile(const std::string& filename);
	//=======================================================================================
	// public methods.
	//=======================================================================================
	Analysis();
	~Analysis();
private:
	//=======================================================================================
	// private variables.
	//=======================================================================================

	//=======================================================================================
	// private methods.
	//=======================================================================================
	double CalcVelAve(const std::string& filePath);
	std::pair<std::string, std::string> GetVarValues(const std::string& path);
	void WriteResult(const std::string& filename);
};