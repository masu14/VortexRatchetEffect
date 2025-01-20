#include "MD.h"

double PiningSite::potentialE = 0.0;
double PiningSite::kp = 0.0;
double PiningSite::lp = 0.0;

//�R���X�g���N�^
MD::MD()
{
	/* DO NOTHING */
}

//�f�X�g���N�^
MD::~MD() {
	/* DO NOTHING */
}

//main.cpp��paramater��ݒ肵�ARun���\�b�h��md�v�Z�����s����
void MD::Run(Paramater<double> param) {

	//�p�����[�^�[�����Ƃɕϐ���ݒ肷��
	EOM					= param.EOM;
	condition			= param.condition;
	vortexNum			= param.vortexNum;
	piningSiteNum		= param.piningSiteNum;
	dt					= param.dt;
	maxTime				= param.maxTime;
	a					= param.a;
	weight				= param.weight;
	height				= param.height;
	cutoff				= param.cutoff;
	eta					= param.eta;
	lorentzForce		= param.lorentzForce;
	siteDistance		= param.siteDistance;
	annealTime			= param.annealTime;
	lorentzFrequency	= param.lorentzFrequency;
	f0					= param.f0;
	kp					= param.kp;
	lp					= param.lp;
	outPosition			= param.outPosition;
	outVelocity			= param.outVelocity;
	outForce			= param.outForce;
	outPinPotential		= param.outPinPotential;
	var1name			= param.var1name;
	var2name			= param.var2name;


	
	


	//�����������������Ƃ�MD�@�����s����
	if (InitApp())
	{
		MainLoop();
	}

}

bool MD::InitApp() {

	// �s�j���O�T�C�g�̏�����
	if(!InitPiningSite())
	{
		std::cout << "!initpinpos" << std::endl;
		return false;
	}

	// �{���e�b�N�X�̏�����
	if (!InitVortex())
	{
		std::cout << "!initvortexpos" << std::endl;
		return false;
	}

	//�{���e�b�N�X�ւ̊O�͂�������
	InitForce();
	//std::cout << "mainroop�ɓ���܂�" << std::endl;
	return true;
}

//-------------------------------------------------------------------------------------------------
//		�s�j���O�T�C�g�������ʒu�ɔz�u����
//-------------------------------------------------------------------------------------------------
bool MD::InitPiningSite() {
	if (piningSiteNum < 0) {
		std::cout << "piningSiteNum�ɕs���Ȓl�����͂���܂���" << std::endl;
		return false;
	}
	else if (piningSiteNum == 0) {
		std::cout << "�s�j���O�T�C�g��z�u���Ă��܂���" << std::endl;
		noPiningSite = true;
		return true;
	}

	PiningSite::Setkp(kp);
	PiningSite::Setlp(lp);

	//�s�j���O�T�C�g�̎�ނ�ݒ肷��
	piningType = SetPinType();

	//�s�j���O�T�C�g�̎�ނɉ����Ĕz�u���s��
	SetPin();

	//�z�u�����s�j���O�T�C�g�ɂ��s�j���O�|�e���V�������쐬����
	
	if (outPinPotential) SetPotential();

	CalcPinForce = CreatePinForce(linePinSites);

	return true;
}

//-----------------------------------------------------------------------------------------------
//    �s�j���O�T�C�g�̌`��A�g�ݍ��킹�ɉ������z�u���s��
//-----------------------------------------------------------------------------------------------
void MD::SetPin()
{
	//�咆��3�̉~�`�s�j���O�T�C�g
	if (piningType == PiningType::tripleCircle) {
		circlePinSites = std::make_unique<PiningSiteCircle[]>(piningSiteNum);
		PlaceCirclePinTriple();
		ShiftCirclePinTriple();
	}

	//�召2�̉~�`�s�j���O�T�C�g
	if (piningType == PiningType::doubleCircle) {
		circlePinSites = std::make_unique<PiningSiteCircle[]>(piningSiteNum);
		PlaceCirclePinDouble();
		ShiftCirclePinDouble();
	}

	//�召2�̐��s�j���O�T�C�g
	if (piningType == PiningType::doubleLine) {
		linePinSites = std::make_unique<PiningSiteLine[]>(piningSiteNum);
		PlaceLinePinDouble();
		ShiftLinePinDouble();
	}
	
}

//-------------------------------------------------------------------------------------------------
//		�{���e�b�N�X�������ʒu�ɔz�u����
//-------------------------------------------------------------------------------------------------
bool MD::InitVortex() {
	if (vortexNum <= 0) {
		std::cout << "vortexNum�ɕs���Ȓl�����͂���܂���" << std::endl;
		return false;
	}
	vortexs = std::make_unique<Vortex[]>(vortexNum);
	
	PlaceVorManual();		//�{���e�b�N�X��z�u
	
	return true;
}

//-------------------------------------------------------------------------------------------------
//		
//-------------------------------------------------------------------------------------------------
void MD::SetPotential()
{
	//�s�j���O�|�e���V�����̌v�Z�����쐬����
	auto CalcPinPotential = CreatePinPotential(linePinSites);
	

	std::string dirName = "../output/" + condition;
	std::string dirMD = dirName + "/MD" + FileHandler::GetIndex();

	FileHandler filePotential;
	filePotential.CreateFile(dirMD, "PinningPotential.csv");

	//���x���̏�������
	filePotential.WritePotentialLabel();

	//�e�ʒu�ł̃|�e���V�������v�Z����
	Vector2d pos = { 0.0,0.0 };
	while (pos.x() < weight) {
		pos(1) = 0.0;
		while (pos.y() < height) {
			double energy = CalcPinPotential(pos);
			filePotential.WritePotential(pos, energy);
			pos(1) += 0.01;
		}
		pos(0) += 0.01;
	}
	outPinPotential = false;


}

//-------------------------------------------------------------------------------------------------
//		
//-------------------------------------------------------------------------------------------------
std::function<double(Vector2d vpos)> MD::CreatePinPotential(const unique_ptr<PiningSiteLine[]>& pinSite)
{
	return [this](Vector2d vpos) {
		const double eps = 1e-10;
		double potentialE = 0.0;

		bool inLine = false;
		
		for (int i = 0; i < piningSiteNum; i++) {
			Vector2d difPos = vpos - linePinSites[i].GetPos();
			if (abs(difPos.x()) <= (linePinSites[i].GetLength() / 2.0)&& abs(difPos.y()) <= eps) {
				inLine = true;
				break;
			}
		}
		if (inLine) return potentialE;
		for (int i = 0; i < piningSiteNum; i++) {
			
			Vector2d difPos = vpos - linePinSites[i].GetPos();

			if (difPos.x() < -weight / 2) difPos(0) += weight;
			if (difPos.x() > weight / 2) difPos(0) -= weight;
			if (difPos.y() < -height / 2) difPos(1) += height;
			if (difPos.y() > height / 2) difPos(1) -= height;

			if (difPos.norm() > cutoff) continue;
			
			//x���W��������
			if (abs(difPos.x()) <= (linePinSites[i].GetLength() / 2.0)) {
				potentialE += kp * tanh(abs(difPos.y()) / lp);
			}
			//x���W�������E���ɂ���
			else if (difPos.x() < -(linePinSites[i].GetLength() / 2.0)) {
				Vector2d l = { linePinSites[i].GetLength()/2.0,0.0 };
				double d = (difPos + l).norm();
				potentialE += kp * tanh(d / lp);
			}
			//x���W������荶���ɂ���
			else if (difPos.x() > (linePinSites[i].GetLength() / 2.0)) {
				Vector2d l = { linePinSites[i].GetLength()/2.0,0.0 };
				double d = (difPos - l).norm();
				potentialE += kp * tanh(d / lp);
			}
		}
		return potentialE;
		};
}

//-------------------------------------------------------------------------------------------------
//		
//-------------------------------------------------------------------------------------------------
std::function<Vector2d(Vector2d vpos)> MD::CreatePinForce(const unique_ptr<PiningSiteLine[]>& pinSite)
{
	return [this](Vector2d vpos) {
		const double eps = 1e-10;
		Vector2d force = { 0.0,0.0 };

		//�{���e�b�N�X���s�j���O�T�C�g�����ɂ���ꍇ�A�s�j���O�͍͂�p���Ȃ�
		bool inLine = false;
		for (int i = 0; i < piningSiteNum; i++) {
			Vector2d difPos = vpos - linePinSites[i].GetPos();
			if (abs(difPos.x()) <= (linePinSites[i].GetLength() / 2.0) && abs(difPos.y()) <= eps) {
				inLine = true;
				break;
			}
		}

		if (inLine) return force;

		for (int i = 0; i < piningSiteNum; i++) {
			Vector2d difPos = vpos - linePinSites[i].GetPos();

			if (difPos.x() < -weight / 2) difPos(0) += weight;
			if (difPos.x() > weight / 2) difPos(0) -= weight;
			if (difPos.y() < -height / 2) difPos(1) += height;
			if (difPos.y() > height / 2) difPos(1) -= height;

			if (difPos.norm() > cutoff) continue;
			
			force += linePinSites[i].CalcPiningForce(difPos);

		}
		return force;
		};
}

std::string MD::SetVariableName(std::string varname)
{
	try {
		if (varname == "lorentzForce") return FileHandler::FixedValueStr(2, lorentzForce);
		if (varname == "siteDistance") return FileHandler::FixedValueStr(2, siteDistance);
		else throw "�ϐ����ɊY�����镶���񂪐���������܂���";
	}
	catch (const char* e) {
		std::cout << "Error: " << e << std::endl;
	}
	
}



//-------------------------------------------------------------------------------------------------
//     ���Ԕ��W�����郁�C�����[�v
//-------------------------------------------------------------------------------------------------
void MD::MainLoop() {

	//dirMD�ɋL�ڂ���ϐ��p�����[�^�����擾����
	//var1name�͕ϐ����Avar1str�͕ϐ��̒l�̕�����
	std::string var1str, var2str;
	var1str = SetVariableName(var1name);
	var2str = SetVariableName(var2name);

	//�o�̓t�@�C��������f�B���N�g��dirMD���쐬����
	std::string dirName = "../output/" + condition;
	std::string dirMD;

	dirMD = dirName + "/MD" + FileHandler::GetIndex();
	dirMD += "/MD_" + var1name + "=" + var1str + "_" + var2name + "=" + var2str;
	FileHandler::CreateDir(dirMD);

	//�o�̓t�@�C���̍쐬
	FileHandler filePos;
	FileHandler fileVelocity;
	FileHandler fileForce;
	
	if (outPosition) filePos.CreateFile(dirMD, "position.csv");
	if (outVelocity) fileVelocity.CreateFile(dirMD, "velocity.csv");
	if (outForce) fileForce.CreateFile(dirMD, "force.csv");
	
	//���x���̏�������
	if (piningType == PiningType::tripleCircle || piningType == PiningType::doubleCircle) {
		if (outPosition) filePos.WritePinPos(circlePinSites, piningSiteNum);
	}
	if (piningType == PiningType::doubleLine) {
		if (outPosition) filePos.WritePinPos(linePinSites, piningSiteNum);
	}
	if (outPosition) filePos.     WriteLabel(vortexNum);
	if (outVelocity) fileVelocity.WriteLabel(vortexNum);
	if (outForce) fileForce.WriteLabel(vortexNum);
	
	//���C�����[�v
	double time = 0;
	
	while (time <= maxTime) {
		//�^��������������
		if (EOM == "ordinary") CalcEOM(time);
		if (EOM == "overdamp") CalcEOMOverDamp(time);
		

		//�v�Z���ʂ��t�@�C���ɏ�������
		if (outPosition) filePos.     WritePos(time, vortexs, vortexNum);
		if (outVelocity) fileVelocity.WriteVelocity(time, vortexs, vortexNum);
		if (outForce)fileForce.WriteForce(time, vortexs, vortexNum);

		time += dt;
	}
}

//-------------------------------------------------------------------------------------------------
//		�O�͂�0�ɏ���������
//-------------------------------------------------------------------------------------------------
void MD::InitForce() {
	for (int i = 0; i < vortexNum; i++) {
		vortexs[i].SetForce(0.0, 0.0);
	}
}

//-------------------------------------------------------------------------------------------------
//		�{���e�b�N�X�E�{���e�b�N�X���ݍ�p(VVI)���v�Z����
//-------------------------------------------------------------------------------------------------
void MD::CalcVVI() {
	const double eps = 1e-10;
	for (int i = 0; i < vortexNum -1 ; i++) {
		for (int j = i+1; j < vortexNum; j++) {
			
			Vector2d difPos = vortexs[i].GetPos() - vortexs[j].GetPos();		//�x�N�g���̍�
			
			//�����I�ɌJ��Ԃ��{���e�b�N�X�̂����A�߂��{���e�b�N�X���v�Z����
			if (difPos.x() < -weight / 2) difPos(0) += weight;
			if (difPos.x() >  weight / 2) difPos(0) -= weight;
			if (difPos.y() < -height / 2) difPos(1) += height;
			if (difPos.y() >  height / 2) difPos(1) -= height;
			
			//�ȉ��A�{���e�b�N�X���m�̋������J�b�g�I�t������蒷����Όv�Z���Ȃ�
			if (difPos.norm() > cutoff) continue;						
			
			Vector2d force = f0 * exp(- difPos.norm() / lambda) * difPos/(difPos.norm() + eps);	//VVI�̎���p����
			
			double xForce = force.x();				//VVI��x����
			double yForce = force.y();				//VVI��y����
			
			vortexs[i].AddForce(xForce, yForce);	//��p
			vortexs[j].AddForce(-xForce, -yForce);	//����p
		}
	}
	
}

//-------------------------------------------------------------------------------------------------
//		�s�j���O�͂��v�Z����(�~�`)
//-------------------------------------------------------------------------------------------------
void MD::CalcCirclePiningForce() {
	
	for (int i = 0; i < vortexNum; i++) {

		bool inCircle = false;
		//�{���e�b�N�X���s�j���O�T�C�g�̓����ɂ���ꍇ�͂ǂ̃s�j���O�T�C�g������O�͂��󂯂Ȃ�
		for (int j = 0; j < piningSiteNum; j++) {
			Vector2d difPos = vortexs[i].GetPos() - circlePinSites[j].GetPos();
			if (difPos.norm() <= circlePinSites[j].GetR()) {
				inCircle = true;
				break;
			}
		}

		if (inCircle == true) continue;

		for (int j = 0; j < piningSiteNum; j++) {
			Vector2d difPos = vortexs[i].GetPos() - circlePinSites[j].GetPos();

			if (difPos.x() < -weight / 2) difPos(0) += weight;
			if (difPos.x() > weight / 2) difPos(0) -= weight;
			//if (difPos.y() < -height / 2) difPos(1) += height;
			//if (difPos.y() > height / 2) difPos(1) -= height;

			
			if (difPos.norm() > cutoff) continue;

			Vector2d force = circlePinSites[j].CalcPiningForce(difPos);

			double xForce = force.x();
			double yForce = force.y();
			vortexs[i].AddForce(xForce, yForce);
		}
	}
}

//-------------------------------------------------------------------------------------------------
//		�s�j���O�͂��v�Z����(��)
//-------------------------------------------------------------------------------------------------
void MD::CalcLinePiningForce()
{
	for (int i = 0; i < vortexNum; i++) {
		
		Vector2d force = CalcPinForce(vortexs[i].GetPos());
		double xForce = force.x();
		double yForce = force.y();
		vortexs[i].AddForce(xForce, yForce);
	}
}

//-------------------------------------------------------------------------------------------------
//		���[�����c�͂��v�Z����	
//-------------------------------------------------------------------------------------------------
void MD::CalcLorentzForce(double time) {
	const double PI = 3.141592653589;
	double force;
	if (sin( PI / lorentzFrequency * (time-annealTime)) > 0) force = -lorentzForce;
	else force = lorentzForce;
	
	for (int i = 0; i < vortexNum; i++) {
		vortexs[i].AddForce(force, 0.0);
	}
}

//-------------------------------------------------------------------------------------------------
//		�S����R�ɂ��͂��v�Z����
//-------------------------------------------------------------------------------------------------
void MD::CalcResistForce() {
	for (int i = 0; i < vortexNum; i++) {
		Vector2d velocity = vortexs[i].GetVelocity();	//�{���e�b�N�X�̑��x���擾����
		Vector2d force = -eta * velocity;				//�S����R�ɂ��͂��v�Z����
		vortexs[i].AddForce(force.x(), force.y());		//�{���e�b�N�X�ւ̊O�͂ɉ�����
	}
}

//-------------------------------------------------------------------------------------------------
//		�T�[�}���͂��v�Z����
//-------------------------------------------------------------------------------------------------
void MD::CalcThermalForce() {
	
}

//-------------------------------------------------------------------------------------------------
//		�^���������������ă{���e�b�N�X�̈ʒu�A���x�A�O�͂��X�V����
//-------------------------------------------------------------------------------------------------
void MD::CalcEOM(double time) 
{
	//���x�x�����@��p�������Ԕ��W�Ń{���e�b�N�X�̈ʒu�A���x���X�V����
	//r(t+dt) = r(t) + v(t)*dt + (1/2)*(F(t)/m)*dt^2
	//v(t+dt) = v(t) + (1/2)*((F(t)+F(t+dt))/m)*dt
	{
		if (time == 0) {
			return;
		}

		unique_ptr<Vector2d[]> v1 = std::make_unique<Vector2d[]>(vortexNum);	//���xv(t)�̓��I�z��Av(t+dt)�̌v�Z�Ɏg��
		unique_ptr<Vector2d[]> f1 = std::make_unique<Vector2d[]>(vortexNum);	//�O��F(t)�Av(t+dt)�̌v�Z�Ɏg��
		for (int i = 0; i < vortexNum; i++) {

			Vector2d r1 = vortexs[i].GetPos();		//�ʒur(t)
			v1[i] = vortexs[i].GetVelocity();		//���xv(t)
			f1[i] = vortexs[i].GetForce();			//�O��F(t)

			//�ʒur(t+dt)���v�Z���A�X�V����
			Vector2d r2 = r1 + v1[i] * dt + (f1[i] / eta) / 2 * dt * dt;	//�ʒur(t+dt)�̌v�Z
			
			//�����I���E����
			if (r2.x() < 0)      r2(0) += weight;
			if (r2.x() > weight) r2(0) -= weight;
			if (r2.y() < 0)      r2(1) += height;
			if (r2.y() > height) r2(1) -= height;
			
			vortexs[i].SetPos(r2.x(), r2.y());								//�ʒur(t+dt)�̍X�V
		}

		//�O�͂̍Čv�Z���s���AF(t+dt)���v�Z����
		InitForce();	//�{���e�b�N�X�ւ̊O�͂�������

		//F(t+dt)�̌v�Z
		CalcVVI();
		if(piningType == PiningType::tripleCircle) CalcCirclePiningForce();
		if (piningType == PiningType::doubleCircle) CalcCirclePiningForce();
		if (piningType == PiningType::doubleLine) CalcLinePiningForce();
		if (time > annealTime)CalcLorentzForce(time);
		CalcResistForce();

		//v(t),F(t),F(t+dt)��p���đ��xv(t+dt)���v�Z���A�X�V����
		for (int i = 0; i < vortexNum; i++) {
			
			Vector2d f2 = vortexs[i].GetForce();
			Vector2d v2 = v1[i] + (f1[i] + f2) / (2 * eta) * dt;	//���xv(t+dt)�̌v�Z
			
			vortexs[i].SetForce(f2.x(), f2.y());					//�O��F(t+dt)�̍X�V�A���̎��Ԕ��W�̈ʒur(t)�v�Z�Ŏg��
			vortexs[i].SetVelocity(v2.x(), v2.y());					//���xv(t+dt)�̍X�V
		}
	}
}

//-------------------------------------------------------------------------------------------------
//		�I�[���x�ɒB�����ۂ̉ߌ����^���������������ă{���e�b�N�X�̈ʒu�A���x�A�O�͂��X�V����
//-------------------------------------------------------------------------------------------------
void MD::CalcEOMOverDamp(double time) 
{
	//�I�[���x�ɒB�����ۂ̑��x����{���e�b�N�X�̈ʒu�A���x���X�V����
	//v(t+dt) = F(t+dt) / m
	//r(t+dt) = r(t) + v(t+dt)*dt + (1/2)*((F(t+dt)-F(t))/m)
	if (time == 0) {
		return;
	}

	unique_ptr<Vector2d[]> f1 = std::make_unique<Vector2d[]>(vortexNum);
	for (int i = 0; i < vortexNum; i++) {

		f1[i] = vortexs[i].GetForce();	//f(t)�̎擾
	}

	InitForce();	//�{���e�b�N�X�ւ̊O�͂�������

	//F(t+dt)�̌v�Z
	CalcVVI();
	if (piningType == PiningType::tripleCircle) CalcCirclePiningForce();
	if (piningType == PiningType::doubleCircle) CalcCirclePiningForce();
	if (piningType == PiningType::doubleLine) CalcLinePiningForce();
	if (time > annealTime)CalcLorentzForce(time);
	
	//�I�[���x�����߁A��������ʒu�����߂�
	for (int i = 0; i < vortexNum; i++) {
		Vector2d r1 = vortexs[i].GetPos();							//r(t)�̎擾
		Vector2d f2 = vortexs[i].GetForce();						//f(t+dt)�̎擾
		Vector2d v2 = f2 / eta;										//v(t+dt)�̌v�Z
		Vector2d r2 = r1 + v2 * dt + (f2 - f1[i]) / (2 * eta) * dt;	//r(t+dt)�̌v�Z

		//�����I���E����
		if (r2.x() < 0)      r2(0) += weight;
		if (r2.x() > weight) r2(0) -= weight;
		if (r2.y() < 0)      r2(1) += height;
		if (r2.y() > height) r2(1) -= height;

		vortexs[i].SetForce(f2.x(), f2.y());		//�O�͂̍X�V
		vortexs[i].SetVelocity(v2.x(), v2.y());		//���x�̍X�V
		vortexs[i].SetPos(r2.x(), r2.y());			//�ʒu�̍X�V
	}
}

//------------------------------------------------------------------------------------------------
//    �{���e�b�N�X�̏����z�u���O�p�i�q�ɂ���
//	  �{���e�b�N�X�̐���6�̔{���̂Ƃ��Ɏg���悤�ɂ���
//------------------------------------------------------------------------------------------------
void MD::PlaceVorTriangle() {
	double y = a * sqrt(3.0) / 4.0;
	for (int i = 0; i < vortexNum / 3.0; i++) { 
		double x = a / 4.0;
		if (i % 2 == 1) x += a / 2.0;
		
		for (int j = 0; j < 3; j++) {
			vortexs[3 * i + j].SetPos(x + a * (double)j, y + sqrt(3) / 2.0 * a * (double)i);
		}
	}
}

//------------------------------------------------------------------------------------------------
//    �{���e�b�N�X�̏����z�u�𒷕��`�z�u�ɂ���
//	  �{���e�b�N�X�̐���6�̔{���̂Ƃ��Ɏg���悤�ɂ���
//------------------------------------------------------------------------------------------------
void MD::PlaceVorSquare() {
	double y = a * sqrt(3.0) / 4.0;
	for (int i = 0; i < vortexNum / 3.0; i++) {
		double x = a / 4.0;
		for (int j = 0; j < 3; j++) {
			vortexs[3 * i + j].SetPos(x + a * (double)j, y + sqrt(3) / 2.0 * a * (double)i);
		}
	}
}

//-----------------------------------------------------------------------------------------------
//    �{���e�b�N�X�̏����z�u�������_���ɂ���
//-----------------------------------------------------------------------------------------------
void MD::PlaceVorRandom() {

	std::random_device rd;
	std::mt19937_64 gen(rd());
	double xmin = 0.0;
	double xmax = weight;
	double ymin = 0.0;
	double ymax = height;
	std::uniform_real_distribution<> xdis(xmin, xmax);
	std::uniform_real_distribution<> ydis(ymin, ymax);
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			double x = xdis(gen);
			double y = ydis(gen);
			vortexs[3 * i + j].SetPos(x, y);
		}
		
	}
}

//-----------------------------------------------------------------------------------------------
//    �f�o�b�N�p�A�z��̒����ɒ���
//�@�@����̔z�u�ŊO�͍���ς��Ď����������Ƃ��Ɏg��
//-----------------------------------------------------------------------------------------------
void MD::PlaceVorManual()
{
	for (int i = 0; i < vortexNum; i++) {
		if (piningType == PiningType::tripleCircle || piningType == PiningType::doubleCircle) {
			vortexs[i].SetPos(circlePinSites[i].GetPos().x(), circlePinSites[i].GetPos().y());
		}
		
		if (piningType == PiningType::doubleLine) {
			vortexs[i].SetPos(linePinSites[i].GetPos().x(), linePinSites[i].GetPos().y());
			
		}
	}
	
}

//-----------------------------------------------------------------------------------------------
//    
//-----------------------------------------------------------------------------------------------
PiningType MD::SetPinType() const
{
	//�咆��3��ނ̉~�`�s�j���O�T�C�g�̎���
	if (condition == "Circle-S2M2L2-M_is_Variable" ||
		condition == "Circle-S2M2L2-L_is_Variable" ||
		condition == "Circle-S2M2L2-S_is_Variable") {
		return PiningType::tripleCircle;
	}

	//�召2��ނ̉~�`�s�j���O�T�C�g�̎���
	if (condition == "Circle-S2L2-S_is_Variable" ||
		condition == "Circle-S2L2-L_is_Variable") {
		return PiningType::doubleCircle;
	}

	//���s�j���O�T�C�g�̎���
	if (condition == "Line-S2L2-S_is_Variable" ||
		condition == "Line-S2L2-L_is_Variable") {
		return PiningType::doubleLine;
	}
}



//-----------------------------------------------------------------------------------------------
//    �召2�̉~�`�s�j���O�T�C�g�̍��W�Ɣ��a���Z�b�g����
//-----------------------------------------------------------------------------------------------
void MD::PlaceCirclePinDouble()
{
	circlePinSites[0].SetPos(1.5, 2.6);
	circlePinSites[1].SetPos(5.5, 2.6);
	circlePinSites[2].SetPos(9.5, 2.6);
	circlePinSites[3].SetPos(13.5, 2.6);
	circlePinSites[4].SetPos(1.5, 7.8);
	circlePinSites[5].SetPos(5.5, 7.8);
	circlePinSites[6].SetPos(9.5, 7.8);
	circlePinSites[7].SetPos(13.5, 7.8);

	const double L = 1.5, S = 0.5;
	double r1 = 0.0, r2 = 0.0;
	if (condition == "Circle-S2L2-S_is_Variable") {
		r1 = L, r2 = S;
	}
	else if (condition == "Circle-S2L2-L_is_Variable") {
		r1 = S, r2 = L;
	}
	else {
		std::cout << "�Y������condition�����݂��܂���" << std::endl;
	}

	circlePinSites[0].SetR(r1);
	circlePinSites[1].SetR(r2);
	circlePinSites[2].SetR(r1);
	circlePinSites[3].SetR(r2);
	circlePinSites[4].SetR(r1);
	circlePinSites[5].SetR(r2);
	circlePinSites[6].SetR(r1);
	circlePinSites[7].SetR(r2);
}

//-----------------------------------------------------------------------------------------------
//    �咆��3�̉~�`�s�j���O�T�C�g�̍��W�Ɣ��a���Z�b�g����
//-----------------------------------------------------------------------------------------------

void MD::PlaceCirclePinTriple()
{
	circlePinSites[0].SetPos(1.5, 2.6);
	circlePinSites[1].SetPos(5.5, 2.6);
	circlePinSites[2].SetPos(9.5, 2.6);
	circlePinSites[3].SetPos(1.5, 7.8);
	circlePinSites[4].SetPos(5.5, 7.8);
	circlePinSites[5].SetPos(9.5, 7.8);

	const double L = 1.5, M = 1.0, S = 0.5;

	double r1 = 0.0, r2 = 0.0, r3 = 0.0;
	if (condition == "Circle-S2M2L2-M_is_Variable") {
		r1 = L, r2 = M, r3 = S;
	}
	else if (condition == "Circle-S2M2L2-S_is_Variable") {
		r1 = M, r2 = S, r3 = L;
	}
	else if (condition == "Circle-S2M2L2-L_is_Variable") {
		r1 = S, r2 = L, r3 = M;
	}
	else {
		std::cout << "�Y������condition�����݂��܂���" << std::endl;
	}

	circlePinSites[0].SetR(r1);
	circlePinSites[1].SetR(r2);
	circlePinSites[2].SetR(r3);
	circlePinSites[3].SetR(r1);
	circlePinSites[4].SetR(r2);
	circlePinSites[5].SetR(r3);
}

//-----------------------------------------------------------------------------------------------
//    �召2�̐��s�j���O�T�C�g�̍��W�ƒ������Z�b�g����
//-----------------------------------------------------------------------------------------------
void MD::PlaceLinePinDouble()
{
	linePinSites[0].SetPos(1.6, 0.5);
	linePinSites[1].SetPos(5.6, 0.5);
	linePinSites[2].SetPos(9.6, 0.5);
	linePinSites[3].SetPos(13.6, 0.5);
	linePinSites[4].SetPos(1.6, 2.5);
	linePinSites[5].SetPos(5.6, 2.5);
	linePinSites[6].SetPos(9.6, 2.5);
	linePinSites[7].SetPos(13.6, 2.5);
	linePinSites[8].SetPos(1.6, 4.5);
	linePinSites[9].SetPos(5.6, 4.5);
	linePinSites[10].SetPos(9.6, 4.5);
	linePinSites[11].SetPos(13.6, 4.5);
	linePinSites[12].SetPos(1.6, 6.5);
	linePinSites[13].SetPos(5.6, 6.5);
	linePinSites[14].SetPos(9.6, 6.5);
	linePinSites[15].SetPos(13.6, 6.5);

	const double L = 3.0, S = 0.1;

	double l1 = 0.0, l2 = 0.0;
	if (condition == "Line-S2L2-S_is_Variable") {
		l1 = L, l2 = S;
	}
	else {
		std::cout << "�Y������condition�����݂��܂���" << std::endl;
	}

	linePinSites[0].SetLength(l1);
	linePinSites[1].SetLength(l2);
	linePinSites[2].SetLength(l1);
	linePinSites[3].SetLength(l2);
	linePinSites[4].SetLength(l1);
	linePinSites[5].SetLength(l2);
	linePinSites[6].SetLength(l1);
	linePinSites[7].SetLength(l2);
	linePinSites[8].SetLength(l1);
	linePinSites[9].SetLength(l2);
	linePinSites[10].SetLength(l1);
	linePinSites[11].SetLength(l2);
	linePinSites[12].SetLength(l1);
	linePinSites[13].SetLength(l2);
	linePinSites[14].SetLength(l1);
	linePinSites[15].SetLength(l2);
}

//-----------------------------------------------------------------------------------------------
//    �召2�̃s�j���O�T�C�g�����炷
//-----------------------------------------------------------------------------------------------

void MD::ShiftCirclePinDouble()
{
	circlePinSites[1].AddPos(siteDistance, 0);
	circlePinSites[3].AddPos(siteDistance, 0);
	circlePinSites[5].AddPos(siteDistance, 0);
	circlePinSites[7].AddPos(siteDistance, 0);
}

//-----------------------------------------------------------------------------------------------
//    �咆��3�̃s�j���O�T�C�g�����炷
//-----------------------------------------------------------------------------------------------

void MD::ShiftCirclePinTriple()
{
	circlePinSites[1].AddPos(siteDistance, 0);
	circlePinSites[4].AddPos(siteDistance, 0);
}

void MD::ShiftLinePinDouble()
{
	linePinSites[1].AddPos(siteDistance, 0);
	linePinSites[3].AddPos(siteDistance, 0);
	linePinSites[5].AddPos(siteDistance, 0);
	linePinSites[7].AddPos(siteDistance, 0);
	linePinSites[9].AddPos(siteDistance, 0);
	linePinSites[11].AddPos(siteDistance, 0);
	linePinSites[13].AddPos(siteDistance, 0);
	linePinSites[15].AddPos(siteDistance, 0);
}