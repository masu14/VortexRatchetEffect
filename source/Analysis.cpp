#include "Analysis.h"

//コンストラクタ
Analysis::Analysis()
{

}

//デストラクタ
Analysis::~Analysis()
{

}

void Analysis::MakeVelFile(const std::string& dirName)
{
	std::string fileName = dirName + "/velocity_averages.csv";

	std::vector<std::tuple<std::string, std::string, double>> results;

	for (const auto& entry : std::filesystem::recursive_directory_iterator(dirName)) {
		if (entry.is_regular_file() && entry.path().filename() == "velocity.csv") {
			//var1,var2を抽出
			auto [var1, var2] = GetVarValues(entry.path().parent_path().string());

			//velocity.csvの平均を計算
			double vAve = CalcVelAve(entry.path().string());

			results.emplace_back(var1, var2, vAve);

		}
	}

	//結果をcsvに書き出し
	WriteResult(fileName, results);
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

	return xAve;
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

void Analysis::WriteResult(const std::string& filename,
	const std::vector<std::tuple<std::string, std::string, double>>& results)
{
	std::ofstream file(filename);
	file << "lorentzForce,siteDistance,vAve\n";

	for (const auto& [var1, var2, vAve] : results) {
		file << var1 << "," << var2 << "," << vAve << "\n";
	}

}