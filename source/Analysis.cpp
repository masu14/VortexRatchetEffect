#include "Analysis.h"

//コンストラクタ
Analysis::Analysis()
{

}

//デストラクタ
Analysis::~Analysis()
{

}

void Analysis::MakeVelFile(const std::string& filename)
{

}

// velocity.csvを読み込み、平均速度を計算する
double Analysis::CalcVelAve(const std::string& filePath)
{
	std::ifstream file(filePath);
	std::string line;
	double xSum = 0.0, ySum = 0.0;
	size_t count = 0;

	// ヘッダー行をスキップ
	std::getline(file, line);

	// データ行を処理
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string time, xStr, yStr;

		std::getline(ss, time, ',');
		std::getline(ss, xStr, ',');
		std::getline(ss, yStr, ',');

		xSum += std::stod(xStr);
		ySum += std::stod(yStr);
		++count;
	}

	double xAve = xSum / count;
	double yAve = ySum / count;

	return sqrt(xAve * xAve + yAve * yAve);
}

std::pair<std::string, std::string> Analysis::GetVarValues(const std::string& path)
{
	std::regex varRegex(R"(.*lorentzForce=([^_]+)_siteDistance=([^/]+).*)");
	std::smatch match;

	if (std::regex_match(path, match, varRegex) && match.size() == 3) {
		return { match[1].str(), match[2].str() };
	}

	return { "unknown", "unknown" };
}

void Analysis::WriteResult(const std::string& filename)
{
	std::ofstream file(filename);
	file << "lorentzForce,siteDistance,vAve\n";

}