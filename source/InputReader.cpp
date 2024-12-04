#include "InputReader.h"

InputReader::InputReader()
{

}

InputReader::~InputReader()
{

}

Paramater<double> InputReader::GetParam()
{
	return param;
}

//入力データを読み込みParamater構造体に書き込む
void InputReader::ReadParam(const string& filename)
{
	sections = ReadInputFile(filename);
	try {
		//定数パラメータ
		param.EOM = sections["Constant"]["EOM"];
		param.condition = sections["Constant"]["condition"];
		param.vortexNum = StringToNumber<int>(sections["Constant"]["vortexNum"]);
		param.piningSiteNum = StringToNumber<int>(sections["Constant"]["piningsiteNum"]);
		param.dt = StringToNumber<double>(sections["Constant"]["dt"]);
		param.maxTime = StringToNumber<double>(sections["Constant"]["maxTime"]);
		param.a = StringToNumber<double>(sections["Constant"]["a"]);
		param.weight = StringToNumber<double>(sections["Constant"]["weight"]);
		param.height = StringToNumber<double>(sections["Constant"]["height"]);
		param.cutoff = StringToNumber<int>(sections["Constant"]["cutoff"]);
		param.eta = StringToNumber<double>(sections["Constant"]["eta"]);
		param.annealTime = StringToNumber<double>(sections["Constant"]["annealTime"]);
		param.lorentzFrequency = StringToNumber<double>(sections["Constant"]["lorentzFrequency"]);
		param.f0 = StringToNumber<double>(sections["Constant"]["f0"]);
		param.kp = StringToNumber<double>(sections["Constant"]["kp"]);
		param.lp = StringToNumber<double>(sections["Constant"]["lp"]);

		//変数パラメータ
		//[Variable]タグのついたmapから変数名を取り出す
		const auto& innerMap = sections["Variable"];
		std::cout  << std::endl;
		std::array<string, 2> varStrs;
		int i = 0;
		for (const auto& innerPair : innerMap) {
			if (i < 2) {
				varStrs[i] = innerPair.first;
				i++;
			}
			else {
				break;
			}
			
		}
		//[Variable]タグの変数名
		param.var1name = varStrs[0];	//var1に対応
		param.var2name = varStrs[1];	//var2に対応

		//[Variable]タグの変数(配列)の値(start, end, step)
		param.var1 = ReadRange<double>(sections["Variable"][varStrs[0]]);
		param.var2 = ReadRange<double>(sections["Variable"][varStrs[1]]);
		
		//設定フラグパラメータ
		bool enableLoggings = stringToBool(sections["Settings"]["enable_loggings"]);
		bool debugMode = stringToBool(sections["Settings"]["debug_mode"]);


		std::cout << "[Setting]" << std::endl;
		std::cout << "Enable Logging: " << enableLoggings << std::endl;
		std::cout << "Debug Mode: " << debugMode << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

//文字列を数値に変換する関数
template <typename T>
T InputReader::StringToNumber(const string& str)
{
	
	std::istringstream iss(str);
	T num;
	iss >> num;
	return num;
}

//文字列をカンマ区切りで分割して数値のベクトル([start,end,step])に変換
template <typename T>
std::array<T,3> InputReader::ReadRange(const string& str)
{
	std::array<T,3> range;
	std::istringstream iss(str);
	string token;
	size_t index = 0;
	while (std::getline(iss, token, ',')) {
		if (index >= 3) {
			throw std::invalid_argument("[Variable]パラメータが多すぎます" + str);
		}
		range[index] = StringToNumber<T>(token);
		index++;
	}
	
	if (index != 3) {
		throw std::invalid_argument("[Variable]パラメータの形式が正しくありません: " + str);
	}
	return range;
}

//文字列を真偽値に変換する関数
bool InputReader::stringToBool(const string& str)
{
	//文字列から真偽値を返す
	if (str == "true" || str == "True" || str == "1") return true;
	if (str == "false" || str == "False" || str == "0") return false;
	throw std::invalid_argument("Invalid boolean string: " + str);
}


std::map<string, std::map<string, string>> InputReader::ReadInputFile(const string& filename)
{
	std::map<string, std::map<string, string>> sections;
	std::ifstream file(filename);
	string line;
	string currentSection;

	while (std::getline(file, line)) {
		//「/」から始まる場合や空行の場合は無視する
		if (line.empty() || line[0] == '/') continue;

		//セクション(定数や変数タグ)を判別する
		if (line[0] == '[') {
			currentSection = line.substr(1, line.find(']') - 1);
		}
		//セクションとパラメータ名と値を記録する
		else {
			size_t delimiterPos = line.find("=");
			string key = line.substr(0, delimiterPos);
			string value = line.substr(delimiterPos + 1);
			key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
			value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
			sections[currentSection][key] = value;
		}
	}
	return sections;
}