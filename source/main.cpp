#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Voltex.h"
#include "MD.h"

// 文字列を真偽値に変換する関数
bool stringToBool(const std::string& str) {
	if (str == "true" || str == "True" || str == "1") return true;
	if (str == "false" || str == "False" || str == "0") return false;
	throw std::invalid_argument("Invalid boolean string: " + str);
}

//設定ファイルを読み込んでパラメータをマップに格納
std::map<std::string, std::string> readSettings(const std::string& filename) {
	std::map<std::string, std::string> settings;
	std::ifstream file(filename);
	std::string line;
	while (std::getline(file, line)) {
		if (line[0] == '/' || line.empty()) continue;
		size_t delimiterPos = line.find("=");
		std::string key = line.substr(0, delimiterPos);
		std::string value = line.substr(delimiterPos + 1);
		settings[key] = value;
	}
	return settings;
}


int FileHandler::index = 0;

int main() {
	{
		/*
		std::map<std::string, std::string> settings = readSettings("input.ini");

		

		//真偽値の設定を取得する
		try {
			bool enableLogging = stringToBool(settings["enable_loggings"]);
			bool useAdvancedFeatures = stringToBool(settings["use_advanced_features"]);
			bool debugMode = stringToBool(settings["debug_mode"]);

			std::cout << "Enable Logging: " << enableLogging << std::endl;
			std::cout << "Use Advanced Features: " << useAdvancedFeatures << std::endl;
			std::cout << "Debug Mode: " << debugMode << std::endl;
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		*/
		
		//パラメーターの設定
		Paramater paramater = {};
		paramater.voltexNum = 6;		//ボルテックスの数
		paramater.piningSiteNum = 6;	//ピニングサイトの数
		paramater.dt = 0.001;			//時間変化量
		paramater.a = 6;				//初期のボルテックスの格子間隔
		paramater.cutoff = 16;

		//分子動力学法の実行
		unique_ptr<MD> md = std::make_unique<MD>();
		md->Run(paramater);

		
	}
	
	return 0;
}

