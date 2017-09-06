#include "StructureData.h"


protocolData::protocolData()
{
	sensitivity.check = 0;
	settings.check = 0;
	
}

protocolData& protocolData::init(void)
{
	static protocolData pData;
    return pData;
}