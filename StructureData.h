//� ������ ������ ����� ��������� ��� ����������� ���������

#ifndef STRUCTUREDATA_H
#define STRUCTUREDATA_H

//��������� - ����� �������� "������ � ����"
struct railOnTheRoad
{
	int			km;
	int			m;
	int			road;
	wchar_t		peregon[256];
	int			distance�ount;
	int			thread;
};

//��������� - ����� �������� "�������������� �����"
struct pokilometrovy
{
	int			km;
	int			m;
	int			road;
	wchar_t		peregon[256];
	int			distance�ount;
	int			railNum;
};

//��������� - ����� �������� "������ �� ���"
struct railOnRSP
{
	wchar_t		rsp[256];
	int			stelaj;
	int			railNum;
	int			distance�ount;
};

//��������� - ����� �������� "���������� �������"
struct strelPerevod
{
	int			km;
	int			m;
	int			road;
	wchar_t		peregon[256];
	int			type;
};

//��������� ���������� � ��������
//�������� ���������
struct protocolData
{
	int protocolN;
	int operatorN;
	int memory;
	int sync;

	//� ����������� �� objectControl � union �������� ������������ ����� ��������
	int objectControl;
	union
	{
		railOnTheRoad	railRoad;
		pokilometrovy	pokm;
		railOnRSP		railRSP;
		strelPerevod	strel;
	};

	//�������, �����, �����
	void (*dataFunc)(const int&, const int &, const int&);
	//������
	void (*updFunc)(const int&);
};

#endif STRUCTUREDATA_H