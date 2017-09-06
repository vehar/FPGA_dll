//в данном хедере будут собиратся все необходимые структуры

#ifndef STRUCTUREDATA_H
#define STRUCTUREDATA_H
#include <Windows.h>

struct sSensitivity
{
	int channel0;
	int channel1;
	int channel2;
	int channel3;
	int channel4;
	int channel5;
	int channel6;
	int channel7;
	char check;
};

struct sSettings	 
{
	bool acousticСontrol;
	int railType;
	int splice;
	char check;
};

struct sMarks
{
	int km;
	int m;
	wchar_t operators[256];
	int number;
	int DefectCode;
	wchar_t	Other[256];
	char check;
};



//структура - обекс контроля "Рельсы в пути"
struct railOnTheRoad
{
	int		km;
	int		m;
	int		road;
	wchar_t	peregon[256];
	int		distanceСount;
	int		thread;
	char	check;
};

//структура - обекс контроля "Покилометровый запас"
struct pokilometrovy
{
	int		km;
	int		m;
	int		road;
	wchar_t	peregon[256];
	int		distanceСount;
	int		railNam;
	char	check;
};

//структура - обекс контроля "Рельсы на РСП"
struct railOnRSP
{
	wchar_t	rsp[256];
	int		stelaj;
	int		railNam;
	int		distanceСount;
	char	check;
};



//структура - обекс контроля "Стрелочный перевод"
struct strelPerevod
{

	int		km;
	int		m;
	int		road;
	wchar_t peregon[256];
	int		type;
	
	char	check;
};


union uControl
{

		railOnTheRoad	railRoad;
		pokilometrovy	pokm;
		railOnRSP		railRSP;
		strelPerevod	strel;

};

struct sControlObject
{
	unsigned int	protocolN;
	unsigned int	operatorN;
	unsigned int	freeMemoryPersent;
	unsigned int	objectControl;
	uControl		Control;
	int		synchronization;
};
//структура подготовки к контролю
//основная структура
struct protocolData {
public:
	protocolData(void);
	static protocolData& init(void);
	sSensitivity	sensitivity;
	sSettings		settings;
	sMarks			marks;
	sControlObject	controlObject;
	unsigned int	memLimit;
	char	check;
};

#endif STRUCTUREDATA_H