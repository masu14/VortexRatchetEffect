#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>		// ��������ϒ��̔z��ŏ�������
#include <map>			// ����(key, value)���g��
#include <algorithm>	// std::remove_if ���̓f�[�^�̕����񏈗�
#include <cctype>		// std::isspace   ���̓f�[�^�̕����񏈗�

//�^�G�C���A�X
using string = std::string;

/// <summary>
/// Paramater�\����
/// �����������܂Ƃ߂��\���́A�{�V�~�����[�V�����̃p�����[�^�͂��ׂĂ����ŊǗ�����B
/// <summary>
template <typename T>
struct Paramater {
	int vortexNum;				//�{���e�b�N�X�̐�
	int piningSiteNum;			//�s�j���O�T�C�g�̐�
	double dt = 0.001;			//���ԕω���
	double maxTime = 10.0;		//�v�Z����
	double a = 0.25;			//�����z�u���̃{���e�b�N�X�ԋ���(�O�p�i�q)
	double weight;				//�����I���E�����̉���
	double height;				//�����I���E�����̏c��
	int cutoff = 4;				//�{���e�b�N�X�֑��ݍ�p���y�ڂ��Ώۂ̗L���͈�
	double eta = 1.0;			//�S����R��
	double lorentzForce;		//���[�����c�͂̑傫��
	double siteDistance;		//�s�j���O�T�C�g�Ԃ̋���
	double annealTime;			//�A�j�[���ɂ����鎞��
	double lorentzFrequency;	//���[�����c�͂̎��g��
	double f0;					//VVI�̌W��
	double kp;					//�s�j���O�͂̑傫�������߂�W��
	double lp;					//�s�j���O�T�C�g�ɂ������`�����璴�`���ւ̉񕜒�

	string EOM;
	string VVICalc;
	string condition;

	string var1name;
	string var2name;
	std::array<T,3> var1;				//���[�v�p�A�z��ɂ�[start, end, step]������
	std::array<T,3> var2;				//���[�v�p�A�z��ɂ�[start, end, step]������

	bool outPosition;
	bool outVelocity;
	bool outForce;
	bool outPinPotential;
};


/// <summary>
/// InputReader�N���X
/// </summary>
class InputReader
{
public:
	//=========================================================================================
	// public variables
	//=========================================================================================
	

	//=========================================================================================
	// public methods
	//=========================================================================================
	InputReader();
	~InputReader();

	Paramater<double> GetParam();
	void ReadParam(const string& filename);
	
private:
	//=========================================================================================
	// private variables
	//=========================================================================================
	std::map<string, std::map<string, string>> sections;
	Paramater<double> param;

	//=========================================================================================
	// private methods
	//=========================================================================================
	template <typename T>
	T					StringToNumber(const string& str);		// ������𐔒l�ɕϊ�����֐�
	template <typename T>
	std::array<T,3>		ReadRange(const string& str);			// �����񂩂�ϐ��̒�`��ƍ��ݕ����擾����֐�
	bool				stringToBool(const string& str);		// �������^�U�l�ɕϊ�����֐�
	std::map <string, std::map<string, string>> ReadInputFile(const string& filename);	//���̓t�@�C����ǂݍ��ފ֐�

};