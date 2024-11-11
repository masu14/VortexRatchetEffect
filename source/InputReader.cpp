#include "InputReader.h"

InputReader::InputReader()
{

}

InputReader::~InputReader()
{

}

Paramater InputReader::GetParam()
{
	Paramater param = {};
	//[Constant]
	param.voltexNum;
	param.piningSiteNum;
	param.dt;
	param.maxTime;
	param.a;
	param.cutoff;
	param.eta;

	return param;
}

void InputReader::SetParam(const string& filename)
{
	std::map<string, std::map<string, string>> sections = ReadInputFile(filename);
	try {
		//定数パラメータ
		int voltexNum = StringToNumber<int>(sections["Constant"]["voltexNum"]);
		int piningSiteNum = StringToNumber<int>(sections["Constant"]["piningsiteNum"]);
		double dt = StringToNumber<double>(sections["Constant"]["dt"]);
		double maxTime = StringToNumber<double>(sections["Constant"]["maxTime"]);
		double a = StringToNumber<double>(sections["Constant"]["a"]);
		int cutoff = StringToNumber<int>(sections["Constant"]["cutoff"]);
		double eta = StringToNumber<double>(sections["Constant"]["eta"]);

		//変数パラメータ
		std::vector<double> lorentzForce = ReadRange<double>(sections["Variable"]["lorentzForce"]);

		//設定フラグパラメータ
		bool enableLoggings = stringToBool(sections["Settings"]["enable_loggings"]);
		bool debugMode = stringToBool(sections["Settings"]["dobug_mode"]);

		std::cout << "[Paramater]" << std::endl;
		std::cout << "voltexNum: " << voltexNum << std::endl;
		std::cout << "piningSiteNum: " << piningSiteNum << std::endl;
		std::cout << "dt: " << dt << std::endl;
		std::cout << "maxTime: " << maxTime << std::endl;
		std::cout << "a: " << a << std::endl;
		std::cout << "cutoff: " << cutoff << std::endl;
		std::cout << "eta: " << eta << std::endl;

		std::cout << "[Variable]" << std::endl;
		std::cout << "lorentzForce: " << lorentzForce[0] << "," << lorentzForce[1] << "," << lorentzForce[2] << std::endl;

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
std::vector<T> InputReader::ReadRange(const string& str)
{
	std::vector<T> range;
	std::istringstream iss(str);
	string token;
	while (std::getline(iss, token, ',')) {
		range.push_back(StringToNumber<T>(token));
	}
	
	
	std::cout << range.size() << std::endl;
	if (range.size() != 3) {
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
		//「/」や空白から始まる場合は無視する
		if (line[0] == '/' || line.empty()) continue;

		//セクション(定数や変数タグ)を判別する
		if (line[0] == '[') {
			currentSection = line.substr(1, line.find(']') - 1);
		}
		//セクションとパラメータ名と値を記録する
		else {
			size_t delimiterPos = line.find("=");
			string key = line.substr(0, delimiterPos);
			string value = line.substr(0, delimiterPos + 1);
			key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
			value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
			sections[currentSection][key] = value;
		}
	}
	return sections;
}