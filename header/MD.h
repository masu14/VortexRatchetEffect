#pragma once
#include <memory>
#include <math.h>			//��{�I�Ȋ֐����������߂̕W�����C�u����
#include <random>
#include <functional>
#include "Vortex.h"
#include "PiningSite.h"
#include "FileHandler.h"
#include "InputReader.h"	//Paramater�\���̂��Ƃ�

template<typename T> using unique_ptr = std::unique_ptr<T>;		//�����ď璷�Ȃ̂Łustd::�v���ȗ����Ă邾��


//�N���X�̑O���錾
class Vortex;		
class PiningSite;
class PiningSiteCircle;

enum class PiningType
{
	tripleCircle,
	doubleCircle,
	doubleLine,
};

/// <summary>
/// ���q���͊w�@�̃}�l�[�W���[�N���X�A�e�{���e�b�N�X�A�s�j���O�T�C�g�̊Ǘ�������
/// �{���e�b�N�X�A�s�j���O�T�C�g�̏������������߁A�R���X�g���N�^�ŃC���X�^���X�𐶐����邵�A
/// �f�X�g���N�^�ŃC���X�^���X���폜����
/// </summary>
class MD{
public:
	//=======================================================================================
	// public variables.
	//=======================================================================================
	/* NOTHING */

	//=======================================================================================
	// public methods.
	//=======================================================================================
	MD();			//�{���e�b�N�X�A�s�j���O�T�C�g�̏�����
	~MD();			//�C���X�^���X���폜����
	void Run(Paramater<double> param);		//���q���͊w�@�̎��s

private:
	//=======================================================================================
	// private variables.
	//=======================================================================================
	std::string EOM;			//�^���������̎�ށAordinary or overdamp
	std::string VVICalc;		//VVI�̎��, bessel_K1 or exp
	std::string condition;		//��������
	int vortexNum;				//�{���e�b�N�X�̐�
	int piningSiteNum;			//�s�j���O�T�C�g�̐�
	double dt = 0.001;			//���ԕω���
	double maxTime = 10.0;		//�v�Z����
	double a = 0.25;			//�����z�u���̃{���e�b�N�X�ԋ���(�O�p�i�q)
	double height = 5;			//�V�~�����[�V�����{�b�N�X�̍���
	double weight = 5;			//�V�~�����[�V�����{�b�N�X�̕�
	int cutoff = 4;				//�{���e�b�N�X�֑��ݍ�p���y�ڂ��Ώۂ̗L���͈�
	double eta = 1.0;			//�S����R��
	double lorentzForce;		//���[�����c�͂̑傫��
	double siteDistance;		//�Ώۂ̃s�j���O�T�C�g�̂��炷�l
	double annealTime;			//���߂Ƀ��[�����c�͂��������ɂ��鎞��
	double lorentzFrequency;	//���[�����c�͂̌�����؂�ւ��鎞��
	double f0;					//VVI�̌W���̑傫��
	double kp;					//�s�j���O�͂̌W���̑傫��
	double lp;					//
	bool outPosition = false;
	bool outVelocity = false;
	bool outForce = false;
	bool outPinPotential = false;

	std::string var1name;
	std::string var2name;

	PiningType piningType;

	unique_ptr<Vortex[]> vortexs;					//�{���e�b�N�X�̃C���X�^���X�A�@vortexNum�̔z��Ƃ��Ĉ���
	unique_ptr<PiningSiteCircle[]> circlePinSites;	//�~�s�j���O�T�C�g�̃C���X�^���X�ApiningSiteNum�̔z��Ƃ��Ĉ���
	unique_ptr<PiningSiteLine[]> linePinSites;		//���s�j���O�T�C�g
	bool noPiningSite = false;						//�s�j���O�T�C�g�����̏ꍇ�̃t���O
	double lambda = 1.0;

	//=======================================================================================
	// private methods.
	//=======================================================================================
	bool InitApp();
	void MainLoop();

	bool InitVortex();			//�{���e�b�N�X�̏����z�u���s��
	bool InitPiningSite();			//�s�j���O�T�C�g�̏����z�u���s��
	
	std::function<double(Vector2d vpos)> CreateLinePinPotential(const unique_ptr<PiningSiteLine[]>& pinSite);
	std::function<double(Vector2d vpos)> CreateCirclePinPotential(const unique_ptr<PiningSiteCircle[]>& pinSite);
	std::function<Vector2d(Vector2d vpos)>CreatePinForce(const unique_ptr<PiningSiteLine[]>& pinSite);

	std::function<Vector2d(Vector2d vpos)>CalcPinForce;
		
	void InitForce();						//�O�͂�0�ɏ���������
	void CalcVVI();							//�{���e�b�N�X�E�{���e�b�N�X���ݍ�p(VVI)���v�Z����
	void CalcCirclePiningForce();			//�s�j���O�͂��v�Z����(�~�`)
	void CalcLinePiningForce();				//�s�j���O�͂��v�Z����(��)
	void CalcLorentzForce(double time);		//���[�����c�͂��v�Z����	
	void CalcResistForce();					//�S����R�ɂ��͂��v�Z����
	void CalcThermalForce();				//�T�[�}���͂��v�Z����
	void CalcEOM(double time);				//�^���������������Ĉʒu�A���x�A�����x���X�V����
	void CalcEOMOverDamp(double time);

	void PlaceVorTriangle();		//�{���e�b�N�X�̏����z�u���O�p�i�q�ɂ���
	void PlaceVorSquare();
	void PlaceVorRandom();			//�{���e�b�N�X�̏����z�u�������_���ɂ���
	void PlaceVorManual();			//�{���e�b�N�X�̏����z�u������w�肷��

	PiningType SetPinType() const;
	void SetPin();				//�s�j���O�T�C�g�̎�ނɉ����Ĕz�u���s��
	void SetPotential();		//�s�j���O�T�C�g�ɂ�萶����|�e���V������ݒ肷��
	
	void PlaceCirclePinTriple();
	void PlaceCirclePinDouble();
	void PlaceLinePinDouble();
	void ShiftCirclePinTriple();
	void ShiftCirclePinDouble();
	void ShiftLinePinDouble();

	std::string SetVariableName(std::string varname);

};

