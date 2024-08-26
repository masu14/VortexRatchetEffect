#include "FileHandler.h"


//コンストラクタ
FileHandler::FileHandler()
{
	fileName = "";
}

//デストラクタ
FileHandler::~FileHandler()
{
	/* DO_NOTHING */
}

//----------------------------------------------------------------------------------------------
//    ファイル名のナンバリングを設定する
//    他のファイルのナンバリングを確認し、それより1だけ大きくした値を設定する
//----------------------------------------------------------------------------------------------
void FileHandler::SetIndex(std::string dirName)
{
	int maxNum = 0;
	std::regex re(R"(\d{3}[a-zA-Z]+\.[a-zA-Z]+)");	//"(数字3桁)英字列.csvにマッチする正規表現

	for (const auto& entry : std::filesystem::directory_iterator(dirName)) {
		std::string fileName = entry.path().filename().string();
		std::smatch match;

		//正規表現で"(数字3桁)英字列.csvにマッチするファイルを探す
		if (std::regex_match(fileName, match, re)) {
			int num = std::stoi(match[1].str());

			//一番大きいナンバリングの数字を保持しておく
			if (num > maxNum) {
				maxNum = num;
			}
		}
	}
	index = maxNum + 1;	//一番大きいナンバリングに1を足してindexに入れておく
}

//----------------------------------------------------------------------------------------------
//    ファイル名を設定する
//----------------------------------------------------------------------------------------------
void FileHandler::SetName(std::string name)
{
	fileName = name;
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
std::string FileHandler::CreateFilePos(std::string dirName)
{
	std::ostringstream oss;
	oss << std::setw(3) << std::setfill('0') << index << "position.csv";

	std::string fileName = dirName + "/" + oss.str();
	return fileName;
}

//----------------------------------------------------------------------------------------------
//     csvファイル書き込み用、ラベルを記載する
//----------------------------------------------------------------------------------------------
void FileHandler::WriteLabel(std::ofstream& file, int voltexNum)
{
	file << "time";
	for (int i = 0; i < voltexNum; i++) {
		file << ",x,y";
	}
	file << "\n";
}

//----------------------------------------------------------------------------------------------
//    
//----------------------------------------------------------------------------------------------
void FileHandler::WriteAll(std::ofstream& file, double time)
{

}

//----------------------------------------------------------------------------------------------
//    csvファイル書き込み用、各ボルテックスの位置をファイルに書き込む
//----------------------------------------------------------------------------------------------
void FileHandler::WritePos(std::ofstream& file, double time, const unique_ptr<Voltex[]>& voltexs, int voltexNum)
{
	file << time;
	for (int i = 0; i < voltexNum; i++) {
		file << "," << voltexs[i].GetPos().x() << "," << voltexs[i].GetPos().y();
	}
	file << "\n";
}