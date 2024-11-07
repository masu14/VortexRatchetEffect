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
		range.push_back(stringToNumber<T>(token));
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