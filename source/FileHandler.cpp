#include "FileHandler.h"


//コンストラクタ
FileHandler::FileHandler()
	:fileName("")
{ /* DO_NOTHING */ }

//デストラクタ
FileHandler::~FileHandler()
{
	if (file.is_open()) {
		file.close();
	}
}

//----------------------------------------------------------------------------------------------
//    ディレクトリの名前を受け取り、ファイルを作成する
//    OutputType列挙体で出力するデータを指定し、それに応じた名前を点ける
//----------------------------------------------------------------------------------------------
void FileHandler::CreateFile(std::string dirName, OutputType type)
{
	if (type == OutputType::position) fileName = CreatePosFile(dirName);	//位置データファイル作成
	if (type == OutputType::velocity) fileName = CreateVelFile(dirName);	//速度データファイル作成
	if (type == OutputType::force)    fileName = CreateForceFile(dirName);	//外力データファイル作成
	file.open(fileName);
}

//----------------------------------------------------------------------------------------------
//    ファイル名のナンバリングを設定する
//    他のファイルのナンバリングを確認し、それより1だけ大きくした値を設定する
//    std::stoiで文字列をint型の値に変換できなかった時、例外を投げる
//----------------------------------------------------------------------------------------------
void FileHandler::SetIndex(const std::string& dirName)
{
	int maxNum = 0;
	std::regex re(R"(\d{3})");	//"(数字3桁)の正規表現

	try {
		//dirName内のファイルで3桁の数字で始まるものを探し、最も大きい数字を記録する
		for (const auto& entry : std::filesystem::directory_iterator(dirName)) {
			std::string fileName = entry.path().filename().string();
			std::smatch match;

			//正規表現で"(数字3桁)英字列.csvにマッチするファイルを探す
			if (std::regex_search(fileName, match, re)) {
				int num = std::stoi(match[0].str());

				//一番大きいナンバリングの数字を保持しておく
				if (num > maxNum) {
					maxNum = num;
				}
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "An error occurred: " << e.what() << std::endl;
		std::abort();
	}
	
	index = maxNum + 1;	//一番大きいナンバリングに1を足してindexに入れておく
	
}

//------------------------------------------------------------------------------------------------
//    csvファイル生成用、現在時刻を取得する
//------------------------------------------------------------------------------------------------
std::string FileHandler::GetCurrentTimeStr()
{
	auto now = std::chrono::system_clock::now();
	auto inTimeT = std::chrono::system_clock::to_time_t(now);

	std::tm buf;
	localtime_s(&buf, &inTimeT);
	std::stringstream ss;
	ss << std::put_time(&buf, "%Y%m%d");	//年月日まで取得する

	return ss.str();	//取得した時間を文字列に変換して返す
}

//------------------------------------------------------------------------------------------------
//    outputディレクトリに今日の日付のディレクトリがなかった場合、ディレクトリを作成する
//------------------------------------------------------------------------------------------------
void FileHandler::CreateDir(std::string dirName)
{
	if (!std::filesystem::exists(dirName)) {
		std::filesystem::create_directories(dirName);
	}
}

//----------------------------------------------------------------------------------------------
//    ファイル名のナンバリングを返す
//----------------------------------------------------------------------------------------------
int FileHandler::GetIndex() const
{
	return index;
}

//----------------------------------------------------------------------------------------------
//    ファイル名を返す
//----------------------------------------------------------------------------------------------
std::string FileHandler::GetName() const
{
	return fileName;
}

//----------------------------------------------------------------------------------------------
//    ボルテックスの位置を出力するファイルを作成する
//----------------------------------------------------------------------------------------------
std::string FileHandler::CreatePosFile(std::string dirName)
{
	std::ostringstream oss;
	oss << std::setw(3) << std::setfill('0') << index << "position.csv";

	std::string fileName = dirName + "/" + oss.str();
	return fileName;
}

//----------------------------------------------------------------------------------------------
//    ボルテックスの速度を出力するファイルを作成する
//----------------------------------------------------------------------------------------------
std::string FileHandler::CreateVelFile(std::string dirName)
{
	std::ostringstream oss;
	oss << std::setw(3) << std::setfill('0') << index << "velocity.csv";

	std::string fileName = dirName + "/" + oss.str();
	return fileName;
}

//----------------------------------------------------------------------------------------------
//    ボルテックスの外力を出力するファイルを作成する
//----------------------------------------------------------------------------------------------
std::string FileHandler::CreateForceFile(std::string dirName)
{
	std::ostringstream oss;
	oss << std::setw(3) << std::setfill('0') << index << "force.csv";

	std::string fileName = dirName + "/" + oss.str();
	return fileName;
}

//----------------------------------------------------------------------------------------------
//     csvファイル書き込み用、ラベルを記載する
//----------------------------------------------------------------------------------------------
void FileHandler::WriteLabel(int voltexNum)
{
	file << "time";
	for (int i = 0; i < voltexNum; i++) {
		file << ",x,y";
	}
	file << "\n";
}

//----------------------------------------------------------------------------------------------
//    csvファイル書き込み用、各ボルテックスの位置をファイルに書き込む(時間発展用)
//----------------------------------------------------------------------------------------------
void FileHandler::WritePos(double time, const unique_ptr<Voltex[]>& voltexs, int voltexNum)
{
	file << time;
	for (int i = 0; i < voltexNum; i++) {
		file << "," << voltexs[i].GetPos().x() << "," << voltexs[i].GetPos().y();
	}
	file << "\n";
}

//----------------------------------------------------------------------------------------------
//    csvファイル書き込み用、各ボルテックスの速度をファイルに書き込む(時間発展用)
//----------------------------------------------------------------------------------------------
void FileHandler::WriteVelocity(double time, const unique_ptr<Voltex[]>& voltexs, int voltexNum)
{
	file << time;
	for (int i = 0; i < voltexNum; i++) {
		file << "," << voltexs[i].GetVelocity().x() << "," << voltexs[i].GetVelocity().y();
	}
	file << "\n";
}

//----------------------------------------------------------------------------------------------
//    csvファイル書き込み用、各ボルテックスの外力をファイルに書き込む(時間発展用)
//----------------------------------------------------------------------------------------------
void FileHandler::WriteForce(double time, const unique_ptr<Voltex[]>& voltexs, int voltexNum)
{
	file << time;
	for (int i = 0; i < voltexNum; i++) {
		file << "," << voltexs[i].GetForce().x() << "," << voltexs[i].GetForce().y();
	}
	file << "\n";
}

//----------------------------------------------------------------------------------------------
//    csvファイル書き込み用、各ボルテックスの位置をファイルに書き込む(分布用)
//----------------------------------------------------------------------------------------------
void FileHandler::WritePos(const unique_ptr<Voltex[]>& voltexs, int voltexNum)
{
	file << "x,y\n";
	for (int i = 0; i < voltexNum; i++) {
		file << voltexs[i].GetPos().x() << "," << voltexs[i].GetPos().y() << "\n";
	}
}

//----------------------------------------------------------------------------------------------
//    csvファイル書き込み用、各ボルテックスの速度をファイルに書き込む(分布用)
//----------------------------------------------------------------------------------------------
void FileHandler::WriteVelocity(const unique_ptr<Voltex[]>& voltexs, int voltexNum)
{
	file << "x,y\n";
	for (int i = 0; i < voltexNum; i++) {
		file << voltexs[i].GetVelocity().x() << "," << voltexs[i].GetVelocity().y() << "\n";
	}
}

//----------------------------------------------------------------------------------------------
//    csvファイル書き込み用、各ボルテックスの外力をファイルに書き込む(分布用)
//----------------------------------------------------------------------------------------------
void FileHandler::WriteForce(const unique_ptr<Voltex[]>& voltexs, int voltexNum)
{
	file << "x,y\n";
	for (int i = 0; i < voltexNum; i++) {
		file << voltexs[i].GetForce().x() << "," << voltexs[i].GetForce().y() << "\n";
	}
}