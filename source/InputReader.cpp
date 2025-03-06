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

//���̓f�[�^��ǂݍ���Paramater�\���̂ɏ�������
void InputReader::ReadParam(const string& filename)
{
	sections = ReadInputFile(filename);
	try {
		//�萔�p�����[�^
		param.EOM = sections["Constant"]["EOM"];
		param.VVICalc = sections["Constant"]["VVICalc"];
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

		//�ϐ��p�����[�^
		//[Variable]�^�O�̂���map����ϐ��������o��
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
		//[Variable]�^�O�̕ϐ���
		param.var1name = varStrs[0];	//var1�ɑΉ�
		param.var2name = varStrs[1];	//var2�ɑΉ�

		//[Variable]�^�O�̕ϐ�(�z��)�̒l(start, end, step)
		param.var1 = ReadRange<double>(sections["Variable"][varStrs[0]]);
		param.var2 = ReadRange<double>(sections["Variable"][varStrs[1]]);
		
		//�ݒ�t���O�p�����[�^
		bool enableLoggings = stringToBool(sections["Settings"]["enable_loggings"]);
		bool debugMode = stringToBool(sections["Settings"]["debug_mode"]);
		param.outPosition = stringToBool(sections["Settings"]["outPosition"]);
		param.outVelocity = stringToBool(sections["Settings"]["outVelocity"]);
		param.outForce = stringToBool(sections["Settings"]["outForce"]);
		param.outPinPotential = stringToBool(sections["Settings"]["outPinPotential"]);

	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

//������𐔒l�ɕϊ�����֐�
template <typename T>
T InputReader::StringToNumber(const string& str)
{
	
	std::istringstream iss(str);
	T num;
	iss >> num;
	return num;
}

//��������J���}��؂�ŕ������Đ��l�̃x�N�g��([start,end,step])�ɕϊ�
template <typename T>
std::array<T,3> InputReader::ReadRange(const string& str)
{
	std::array<T,3> range;
	std::istringstream iss(str);
	string token;
	size_t index = 0;
	while (std::getline(iss, token, ',')) {
		if (index >= 3) {
			throw std::invalid_argument("[Variable]�p�����[�^���������܂�" + str);
		}
		range[index] = StringToNumber<T>(token);
		index++;
	}
	
	if (index != 3) {
		throw std::invalid_argument("[Variable]�p�����[�^�̌`��������������܂���: " + str);
	}
	return range;
}

//�������^�U�l�ɕϊ�����֐�
bool InputReader::stringToBool(const string& str)
{
	//�����񂩂�^�U�l��Ԃ�
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
		//�u/�v����n�܂�ꍇ���s�̏ꍇ�͖�������
		if (line.empty() || line[0] == '/') continue;

		//�Z�N�V����(�萔��ϐ��^�O)�𔻕ʂ���
		if (line[0] == '[') {
			currentSection = line.substr(1, line.find(']') - 1);
		}
		//�Z�N�V�����ƃp�����[�^���ƒl���L�^����
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