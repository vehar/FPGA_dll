//в данном хедере будут собиратся все необходимые структуры

#ifndef STRUCTUREDATA_H
#define STRUCTUREDATA_H

//структура - обекс контроля "Рельсы в пути"
struct railOnTheRoad
{
	int			km;
	int			m;
	int			road;
	wchar_t		peregon[256];
	int			distanceСount;
	int			thread;
};

//структура - обекс контроля "Покилометровый запас"
struct pokilometrovy
{
	int			km;
	int			m;
	int			road;
	wchar_t		peregon[256];
	int			distanceСount;
	int			railNum;
};

//структура - обекс контроля "Рельсы на РСП"
struct railOnRSP
{
	wchar_t		rsp[256];
	int			stelaj;
	int			railNum;
	int			distanceСount;
};

//структура - обекс контроля "Стрелочный перевод"
struct strelPerevod
{
	int			km;
	int			m;
	int			road;
	wchar_t		peregon[256];
	int			type;
};

//структура подготовки к контролю
//основная структура
struct protocolData
{
	int protocolN;
	int operatorN;
	int memory;
	int sync;

	//в зависимости от objectControl в union выбираем определенный обект контроля
	int objectControl;
	union
	{
		railOnTheRoad	railRoad;
		pokilometrovy	pokm;
		railOnRSP		railRSP;
		strelPerevod	strel;
	};

	//частота, пакет, буфер
	void (*dataFunc)(const int&, const int &, const int&);
	//размер
	void (*updFunc)(const int&);
};

#endif STRUCTUREDATA_H