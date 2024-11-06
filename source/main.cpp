#pragma once
#include <iostream>
#include <algorithm>	// std::remove_if 入力データの文字列処理
#include <cctype>		// std::isspace   入力データの文字列処理
#include <fstream>		//
#include <sstream>		// 文字列ストリームを使って文字列を数値に変換
#include <string>
#include <map>
#include <vector>
#include "Voltex.h"
#include "MD.h"

// 文字列を数値に変換する関数
template <typename T>
T stringToNumber(const std::string& str) {

	std::istringstream iss(str);
	T num;
	iss >> num;
	return num;
}

//文字列をカンマ区切りで分割して数値のベクトルに変換
std::vector<double> ParseRange(const std::string& str) {
	std::vector<double> range;
	std::istringstream iss(str);
	std::string token;
	while (std::getline(iss, token, ',')) {
		range.push_back(stringToNumber<double>(token));
	}
	return range;
}

// 文字列を真偽値に変換する関数
bool stringToBool(std::string str) {

	//文字列から真偽値を返す
	if (str == "true" || str == "True" || str == "1") return true;
	if (str == "false" || str == "False" || str == "0") return false;
	throw std::invalid_argument("Invalid boolean string: " + str);
}

//設定ファイルを読み込んでパラメータをマップに格納
std::map<std::string, std::map<std::string, std::string>> readSettings(const std::string& filename) {
	std::map<std::string, std::map<std::string, std::string>> sections;
	std::ifstream file(filename);
	std::string line;
	std::string currentSection;

	while (std::getline(file, line)) {
		if (line[0] == '/' || line.empty()) continue;

		if (line[0] == '[') {
			currentSection = line.substr(1, line.find(']') - 1);
		}
		else {
			size_t delimiterPos = line.find("=");
			std::string key = line.substr(0, delimiterPos);
			std::string value = line.substr(delimiterPos + 1);
			key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
			value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
			sections[currentSection][key] = value;
		}

		
	}
	return sections;
}


int FileHandler::index = 0;

int main() {
	{
		//シミュレーションの設定パラメータをinput.iniファイルから取得する
		std::map<std::string, std::map<std::string, std::string>> settings  = readSettings("input.ini");
		

		
		
		//真偽値の設定を取得する
		try {
			//パラメータを取得する
			//定数パラメータ
			double time_step = stringToNumber<double>(settings["Constant"]["time_step"]);
			int duration = stringToNumber<int>(settings["Constant"]["duration"]);
			//変数パラメータ
			std::vector<double> particleRange = ParseRange(settings["Variable"]["particles"]);
			//設定フラグパラメータ
			bool enableLogging = stringToBool(settings["Settings"]["enable_loggings"]);
			bool useAdvancedFeatures = stringToBool(settings["Settings"]["use_advanced_features"]);
			bool debugMode = stringToBool(settings["Settings"]["debug_mode"]);

			if (particleRange.size() != 3) {
				std::cerr << "変数の入力形式が正しくありません。" << std::endl;
				return 1;
			}

			double particle_start = particleRange[0];
			double particle_end = particleRange[1];
			double particle_step = particleRange[2];

			for (int particles = particle_start; particles <= particle_end; particles += particle_step) {
				std::cout << "Running simulation with " << particles << " particles" << std::endl;

				// ここでシミュレーション処理を実行
				// 例: runSimulation(time_step, duration, particles);

				std::cout << "Simulation completed for " << particles << " particles." << std::endl;
			}

			std::cout << "[Paramater]" << std::endl;
			std::cout << "time_step: " << time_step << std::endl;
			std::cout << "duration: " << duration << std::endl;
			

			std::cout << "[Setting]" << std::endl;
			std::cout << "Enable Logging: " << enableLogging << std::endl;
			std::cout << "Use Advanced Features: " << useAdvancedFeatures << std::endl;
			std::cout << "Debug Mode: " << debugMode << std::endl;
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			std::exit(EXIT_FAILURE);
		}
		
		
		//パラメーターの設定
		Paramater paramater = {};
		paramater.voltexNum = 6;		//ボルテックスの数
		paramater.piningSiteNum = 6;	//ピニングサイトの数
		paramater.dt = 0.001;			//時間変化量
		paramater.maxTime = 15;			//計算時間
		paramater.a = 6;				//初期のボルテックスの格子間隔
		paramater.cutoff = 16;
		paramater.eta = 1.0;

		//分子動力学法の実行
		unique_ptr<MD> md = std::make_unique<MD>();
		//md->Run(paramater);

		
	}
	
	return 0;
}

