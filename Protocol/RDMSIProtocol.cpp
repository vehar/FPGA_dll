#include "RDMSIProtocol.hpp"


// Default constructor
RDMSIProtocol::RDMSIProtocol() : header(),
								 tail(),
								 file() {}

// Get file
std::fstream* RDMSIProtocol::getFile() {

	return &file;

}

// Get file header
RDMSHeader& RDMSIProtocol::getHeader() {

	return header;

}

// Get file tail
RDMSTail& RDMSIProtocol::getTail() {

	return tail;

}

// Set file header
void RDMSIProtocol::setHeader(const RDMSHeader &newHeader) {

	header = newHeader;

}

// Set file tail
void RDMSIProtocol::setTail(const RDMSTail &newTail) {

	tail = newTail;

}

// Destructor
RDMSIProtocol::~RDMSIProtocol() {}


// Calculate check sum of packet
UCHAR RDMSHeader::calcCheckSum() {

	return 0x00;

}

// Return packet size
USHORT RDMSHeader::size() {

	return 0x001A;

}

// Read packet data from buffer
void RDMSHeader::read(LPCSTR buffer) {

	memcpy(headerSymbols, buffer, sizeof(UCHAR) * 4);
	buffer += sizeof(UCHAR) * 4;

	/*
	memcpy(&gpsData.latLongDirection, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.course, buffer, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(&gpsData.sec, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.min, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.hour, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.year, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.month, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.day, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);

	*/
	memcpy(&gpsData.state, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.lat, buffer, sizeof(float));
	buffer += sizeof(float);
	memcpy(&gpsData.lon, buffer, sizeof(float));
	buffer += sizeof(float);
	memcpy(&gpsData.speed, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.course, buffer, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(&gpsData.sec, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.min, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.hour, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.year, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.month, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.day, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);

	memcpy(&timeData, buffer, sizeof(ULONGLONG));
	buffer += sizeof(ULONGLONG);
	memcpy(&freeMemory, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&railType, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&operatorNumber, buffer, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(&direction, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&controlType, buffer, sizeof(RDMSCtrlType));

}

// Write packet data to buffer
void RDMSHeader::write(LPSTR buffer) {

	memcpy(buffer, headerSymbols, sizeof(UCHAR) * 4);
	buffer += sizeof(UCHAR) * 4;

	/*
	memcpy(buffer, &gpsData.latLongDirection, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.lat, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &gpsData.lon, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &gpsData.speed, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.course, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(buffer, &gpsData.sec, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.min, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.hour, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.year, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.month, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.day, sizeof(UCHAR));
	buffer += sizeof(UCHAR);

	*/
	memcpy(buffer, &gpsData.state, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.lat, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &gpsData.lon, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &gpsData.speed, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.course, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(buffer, &gpsData.sec, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.min, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.hour, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.year, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.month, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.day, sizeof(UCHAR));
	buffer += sizeof(UCHAR);

  memcpy(buffer, &timeData, sizeof(ULONGLONG));
	buffer += sizeof(ULONGLONG);
	memcpy(buffer, &freeMemory, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &railType, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &operatorNumber, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(buffer, &direction, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &controlType, sizeof(RDMSCtrlType));

}


// Calculate check sum of packet
UCHAR RDMSSpeedPacket::calcCheckSum() {

	return 0x00;

}

// Return packet size
USHORT RDMSSpeedPacket::size() {

	return 0x0006;

}

// Read Speed packet data from buffer
void RDMSSpeedPacket::read(LPCSTR buffer) {

	memcpy(&startByte, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&timeDifference, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&speed.integer, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&speed.fractional, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&checkSum, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&endByte, buffer, sizeof(UCHAR));

}

// Write Speed packet data to buffer
void RDMSSpeedPacket::write(LPSTR buffer) {

	memcpy(buffer, &startByte, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &timeDifference, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &speed.integer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &speed.fractional, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &checkSum, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &endByte, sizeof(UCHAR));

}


// Calculate check sum of packet
UCHAR RDMSDefectPacket::calcCheckSum() {

	UCHAR summ;

	summ =	pathSensorData.km;
	summ ^=	pathSensorData.m;
	summ ^=	pathSensorData.sm;
	summ ^=	pathSensorData.mm;
	/*
	summ ^= gpsData.latLongDirection;
	summ ^= gpsData.latDegrees;
	summ ^= gpsData.latMinutes;
	summ ^= gpsData.latSeconds;
	summ ^= gpsData.longDegrees;
	summ ^= gpsData.longMinutes;
	summ ^= gpsData.longSeconds;
	*/
	summ ^= gpsData.state;
	summ ^= ((int)&gpsData.lat & 0xFF000000) >> 24;
	summ ^= ((int)&gpsData.lat & 0x00FF0000) >> 16;
	summ ^= ((int)&gpsData.lat & 0x0000FF00) >> 8;
	summ ^= ((int)&gpsData.lat & 0x000000FF);
	summ ^= ((int)&gpsData.lon & 0xFF000000) >> 24;
	summ ^= ((int)&gpsData.lon & 0x00FF0000) >> 16;
	summ ^= ((int)&gpsData.lon & 0x0000FF00) >> 8;
	summ ^= ((int)&gpsData.lon & 0x000000FF);
	summ ^= gpsData.speed;
	summ ^= (gpsData.course & 0xFF00) >> 8;
	summ ^= (gpsData.course & 0x00FF);
	summ ^= gpsData.sec;
	summ ^= gpsData.min;
	summ ^= gpsData.hour;
	summ ^= gpsData.year;
	summ ^= gpsData.month;
	summ ^= gpsData.day;

	for (DWORD i = 0; i < 8; ++i) {

		summ ^= defects[i].amplitude;
		summ ^= defects[i].depth;

	}

	return summ;

}

// Return packet size
USHORT RDMSDefectPacket::size() {


	return 0x0027;

}

// Read Defect packet data from buffer
void RDMSDefectPacket::read(LPCSTR buffer) {

	memcpy(&startByte, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&pathSensorData.km, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&pathSensorData.m, buffer, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(&pathSensorData.sm, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&pathSensorData.mm, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	/*
	memcpy(&gpsData.latLongDirection, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.latDegrees, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.latMinutes, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.latSeconds, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.longDegrees, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.longMinutes, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.longSeconds, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	*/
	memcpy(&gpsData.state, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.lat, buffer, sizeof(float));
	buffer += sizeof(float);
	memcpy(&gpsData.lon, buffer, sizeof(float));
	buffer += sizeof(float);
	memcpy(&gpsData.speed, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.course, buffer, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(&gpsData.sec, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.min, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.hour, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.year, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.month, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.day, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);

	memcpy(&gpsData.state, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.lat, buffer, sizeof(float));
	buffer += sizeof(float);
	memcpy(&gpsData.lon, buffer, sizeof(float));
	buffer += sizeof(float);
	memcpy(&gpsData.speed, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.course, buffer, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(&gpsData.sec, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.min, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.hour, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.year, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.month, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.day, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);

	for (DWORD i = 0; i < 8; ++i) {
	
		memcpy(&defects[i].amplitude, buffer, sizeof(UCHAR));
		buffer += sizeof(UCHAR);
		memcpy(&defects[i].depth, buffer, sizeof(USHORT));
		buffer += sizeof(USHORT);
	
	}

	memcpy(&checkSum, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&endByte, buffer, sizeof(UCHAR));

}

// Write Defect packet data to buffer
void RDMSDefectPacket::write(LPSTR buffer) {

	memcpy(buffer, &startByte, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &pathSensorData.km, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &pathSensorData.m, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(buffer, &pathSensorData.sm, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &pathSensorData.mm, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	/*
	memcpy(buffer, &gpsData.latLongDirection, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.latDegrees, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.latMinutes, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.latSeconds, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.longDegrees, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.longMinutes, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.longSeconds, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	*/
	memcpy(buffer, &gpsData.state, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.lat, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &gpsData.lon, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &gpsData.speed, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.course, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(buffer, &gpsData.sec, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.min, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.hour, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.year, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.month, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.day, sizeof(UCHAR));
	buffer += sizeof(UCHAR);

	memcpy(buffer, &gpsData.state, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.lat, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &gpsData.lon, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &gpsData.speed, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.course, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(buffer, &gpsData.sec, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.min, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.hour, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.year, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.month, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.day, sizeof(UCHAR));
	buffer += sizeof(UCHAR);

	for (DWORD i = 0; i < 8; ++i) {
	
		memcpy(buffer, &defects[i].amplitude, sizeof(UCHAR));
		buffer += sizeof(UCHAR);
		memcpy(buffer, &defects[i].depth, sizeof(USHORT));
		buffer += sizeof(USHORT);
	
	}

	memcpy(buffer, &checkSum, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &endByte, sizeof(UCHAR));

}


// Calculate check sum of packet
UCHAR RDMSTrackCoordPacket::calcCheckSum() {

	return 0x00;

}

// Return packet size
USHORT RDMSTrackCoordPacket::size() {

	return 0x0013 + stageName.nameLength * sizeof(WCHAR);

}

// Read Track coordinates packet data from buffer
void RDMSTrackCoordPacket::read(LPCSTR buffer) {

	memcpy(&startByte, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
  
	/*
	memcpy(&gpsData.latLongDirection, buffer, sizeof(UCHAR));

	buffer += sizeof(UCHAR);
	memcpy(&gpsData.course, buffer, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(&gpsData.sec, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.min, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.hour, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.year, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.month, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.day, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);

	*/
	memcpy(&gpsData.state, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.lat, buffer, sizeof(float));
	buffer += sizeof(float);
	memcpy(&gpsData.lon, buffer, sizeof(float));
	buffer += sizeof(float);
	memcpy(&gpsData.speed, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.course, buffer, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(&gpsData.sec, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.min, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.hour, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.year, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.month, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gpsData.day, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);

	memcpy(&pathSensorData.km, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&pathSensorData.m, buffer, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(&pathSensorData.sm, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&pathSensorData.mm, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&railNumber, buffer, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(&thread, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&stageName.nameLength, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&stageName.name, buffer, sizeof(WCHAR) * stageName.nameLength);
	buffer += sizeof(WCHAR) * stageName.nameLength;
	memcpy(&checkSum, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&endByte, buffer, sizeof(UCHAR));

}

// Write Track coordinates packet data to buffer
void RDMSTrackCoordPacket::write(LPSTR buffer) {

	memcpy(buffer, &startByte, sizeof(UCHAR));
	buffer += sizeof(UCHAR);

	/*
	memcpy(buffer, &gpsData.latLongDirection, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.lat, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &gpsData.lon, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &gpsData.speed, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.course, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(buffer, &gpsData.sec, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.min, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.hour, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.year, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gpsData.month, sizeof(UCHAR));
	buffer += sizeof(UCHAR);

	memcpy(buffer, &gpsData.day, sizeof(UCHAR));
	buffer += sizeof(UCHAR);

	memcpy(buffer, &pathSensorData.km, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &pathSensorData.m, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(buffer, &pathSensorData.sm, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &pathSensorData.mm, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &railNumber, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(buffer, &thread, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &stageName.nameLength, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &stageName.name, sizeof(WCHAR) * stageName.nameLength);
	buffer += sizeof(WCHAR) * stageName.nameLength;
	memcpy(buffer, &checkSum, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &endByte, sizeof(UCHAR));

}


// Calculate check sum of packet
UCHAR RDMSKmStockPacket::calcCheckSum() {

	return 0x00;

}

// Return packet size
USHORT RDMSKmStockPacket::size() {

	return 0x0009 + stageName.nameLength * sizeof(WCHAR);

}

// Read Km stock packet data from buffer
void RDMSKmStockPacket::read(LPCSTR buffer) {

	memcpy(&startByte, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&position.km, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&position.m, buffer, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(&position.trackNumber, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&railNum, buffer, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(&stageName.nameLength, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&stageName.name, buffer, sizeof(WCHAR) * stageName.nameLength);
	buffer += sizeof(WCHAR) * stageName.nameLength;
	memcpy(&checkSum, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&endByte, buffer, sizeof(UCHAR));

}

// Write Km stock packet data to buffer
void RDMSKmStockPacket::write(LPSTR buffer) {

	memcpy(buffer, &startByte, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &position.km, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &position.m, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(buffer, &position.trackNumber, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &railNum, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(buffer, &stageName.nameLength, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &stageName.name, sizeof(WCHAR) * stageName.nameLength);
	buffer += sizeof(WCHAR) * stageName.nameLength;
	memcpy(buffer, &checkSum, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &endByte, sizeof(UCHAR));

}


// Calculate check sum of packet
UCHAR RDMSRspPacket::calcCheckSum() {

	return 0x00;

}

// Return packet size
USHORT RDMSRspPacket::size() {

	return 0x0008 + nameData.nameLength * sizeof(WCHAR);

}

// Read RSP packet data from buffer
void RDMSRspPacket::read(LPCSTR buffer) {

	memcpy(&startByte, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&rackNum, buffer, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(&railNum, buffer, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(&nameData.nameLength, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&nameData.name, buffer, sizeof(WCHAR) * nameData.nameLength);
	buffer += sizeof(WCHAR) * nameData.nameLength;
	memcpy(&checkSum, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&endByte, buffer, sizeof(UCHAR));

}

// Write RSP packet data to buffer
void RDMSRspPacket::write(LPSTR buffer) {

	memcpy(buffer, &startByte, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &rackNum, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(buffer, &railNum, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(buffer, &nameData.nameLength, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &nameData.name, sizeof(WCHAR) * nameData.nameLength);
	buffer += sizeof(WCHAR) * nameData.nameLength;
	memcpy(buffer, &checkSum, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &endByte, sizeof(UCHAR));

}


// Calculate check sum of packet
UCHAR RDMSTurnoutPacket::calcCheckSum() {

	return 0x00;

}

// Return packet size
USHORT RDMSTurnoutPacket::size() {

	return 0x0009 + stageName.nameLength * sizeof(WCHAR);

}

// Read Turnout packet data from buffer
void RDMSTurnoutPacket::read(LPCSTR buffer) {

	memcpy(&startByte, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&position.km, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&position.m, buffer, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(&position.trackNumber, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&type, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&stageName.nameLength, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&stageName.name, buffer, sizeof(WCHAR) * stageName.nameLength);
	buffer += sizeof(WCHAR) * stageName.nameLength;
	memcpy(&checkSum, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&endByte, buffer, sizeof(UCHAR));

}

// Write Turnout packet data to buffer
void RDMSTurnoutPacket::write(LPSTR buffer) {

	memcpy(buffer, &startByte, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &position.km, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &position.m, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(buffer, &position.trackNumber, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &type, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &stageName.nameLength, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &stageName.name, sizeof(WCHAR) * stageName.nameLength);
	buffer += sizeof(WCHAR) * stageName.nameLength;
	memcpy(buffer, &checkSum, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &endByte, sizeof(UCHAR));

}


// Calculate check sum of packet
UCHAR RDMSAcousticPacket::calcCheckSum() {

	return 0x00;

}

// Return packet size
USHORT RDMSAcousticPacket::size() {

	return 0x0004;

}

// Read Acoustic contact packet data from buffer
void RDMSAcousticPacket::read(LPCSTR buffer) {

	memcpy(&startByte, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&contact, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&checkSum, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&endByte, buffer, sizeof(UCHAR));

}

// Write Acoustic contact packet data to buffer
void RDMSAcousticPacket::write(LPSTR buffer) {

	memcpy(buffer, &startByte, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &contact, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &checkSum, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &endByte, sizeof(UCHAR));

}


// Calculate check sum of packet
UCHAR RDMSGainPacket::calcCheckSum() {

	checkSum =	channelNum;
	checkSum ^=	gain;

	return checkSum;

}

// Return packet size
USHORT RDMSGainPacket::size() {

	return 0x0005;

}

// Read Gain packet data from buffer
void RDMSGainPacket::read(LPCSTR buffer) {

	memcpy(&startByte, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&channelNum, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&gain, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&checkSum, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&endByte, buffer, sizeof(UCHAR));

}

// Write Gain packet data to buffer
void RDMSGainPacket::write(LPSTR buffer) {

	memcpy(buffer, &startByte, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &channelNum, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &gain, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &checkSum, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &endByte, sizeof(UCHAR));

}


// Calculate check sum of packet
UCHAR RDMSSyncPacket::calcCheckSum() {

	return 0x00;

}

// Return packet size
USHORT RDMSSyncPacket::size() {

	return 0x0004;

}

// Read Synchronization packet data from buffer
void RDMSSyncPacket::read(LPCSTR buffer) {

	memcpy(&startByte, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&syncronization, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&checkSum, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&endByte, buffer, sizeof(UCHAR));

}

// Write Synchronization packet data to buffer
void RDMSSyncPacket::write(LPSTR buffer) {

	memcpy(buffer, &startByte, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &syncronization, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &checkSum, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &endByte, sizeof(UCHAR));

}


// Calculate check sum of packet
UCHAR RDMSVoltageTempPacket::calcCheckSum() {

	checkSum =	((int)stmTemperature & 0xFF000000) >> 24;
	checkSum ^=	((int)stmTemperature & 0x00FF0000) >> 16;
	checkSum ^=	((int)stmTemperature & 0x0000FF00) >> 8;
	checkSum ^=	((int)stmTemperature & 0x000000FF);
	checkSum ^=	((int)axelTemperature & 0xFF000000) >> 24;
	checkSum ^=	((int)axelTemperature & 0x00FF0000) >> 16;
	checkSum ^=	((int)axelTemperature & 0x0000FF00) >> 8;
	checkSum ^=	((int)axelTemperature & 0x000000FF);
	checkSum ^=	((int)accumCurrent & 0xFF000000) >> 24;
	checkSum ^=	((int)accumCurrent & 0x00FF0000) >> 16;
	checkSum ^=	((int)accumCurrent & 0x0000FF00) >> 8;
	checkSum ^=	((int)accumCurrent & 0x000000FF);
	checkSum ^=	((int)accumVoltage & 0xFF000000) >> 24;
	checkSum ^=	((int)accumVoltage & 0x00FF0000) >> 16;
	checkSum ^=	((int)accumVoltage & 0x0000FF00) >> 8;
	checkSum ^=	((int)accumVoltage & 0x000000FF);
	checkSum ^=	((int)board_1_8V & 0xFF000000) >> 24;
	checkSum ^=	((int)board_1_8V & 0x00FF0000) >> 16;
	checkSum ^=	((int)board_1_8V & 0x0000FF00) >> 8;
	checkSum ^=	((int)board_1_8V & 0x000000FF);
	checkSum ^=	((int)board_3_3V & 0xFF000000) >> 24;
	checkSum ^=	((int)board_3_3V & 0x00FF0000) >> 16;
	checkSum ^=	((int)board_3_3V & 0x0000FF00) >> 8;
	checkSum ^=	((int)board_3_3V & 0x000000FF);
	checkSum ^=	((int)board_5V & 0xFF000000) >> 24;
	checkSum ^=	((int)board_5V & 0x00FF0000) >> 16;
	checkSum ^=	((int)board_5V & 0x0000FF00) >> 8;
	checkSum ^=	((int)board_5V & 0x000000FF);
	checkSum ^=	((int)board_140V & 0xFF000000) >> 24;
	checkSum ^=	((int)board_140V & 0x00FF0000) >> 16;
	checkSum ^=	((int)board_140V & 0x0000FF00) >> 8;
	checkSum ^=	((int)board_140V & 0x000000FF);

	return checkSum;

}

// Return packet size
USHORT RDMSVoltageTempPacket::size() {

	return 0x23;

}

// Read packet data from buffer
void RDMSVoltageTempPacket::read(LPCSTR buffer) {

	memcpy(&startByte, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&stmTemperature, buffer, sizeof(float));
	buffer += sizeof(float);
	memcpy(&axelTemperature, buffer, sizeof(float));
	buffer += sizeof(float);
	memcpy(&accumCurrent, buffer, sizeof(float));
	buffer += sizeof(float);
	memcpy(&accumVoltage, buffer, sizeof(float));
	buffer += sizeof(float);
	memcpy(&board_1_8V, buffer, sizeof(float));
	buffer += sizeof(float);
	memcpy(&board_3_3V, buffer, sizeof(float));
	buffer += sizeof(float);
	memcpy(&board_5V, buffer, sizeof(float));
	buffer += sizeof(float);
	memcpy(&board_140V, buffer, sizeof(float));
	buffer += sizeof(float);
	memcpy(&checkSum, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&endByte, buffer, sizeof(UCHAR));

}

// Write packet data to buffer
void RDMSVoltageTempPacket::write(LPSTR buffer) {

	memcpy(buffer, &startByte, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &stmTemperature, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &axelTemperature, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &accumCurrent, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &accumVoltage, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &board_1_8V, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &board_3_3V, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &board_5V, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &board_140V, sizeof(float));
	buffer += sizeof(float);
	memcpy(buffer, &checkSum, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &endByte, sizeof(UCHAR));

}


// Calculate check sum of packet
UCHAR RDMSTail::calcCheckSum() {

	return 0x00;

}

// Return packet size
USHORT RDMSTail::size() {

	return 0x0005;

}

// Read Tail data from buffer
void RDMSTail::read(LPCSTR buffer) {

	memcpy(headerSymbols, buffer, sizeof(UCHAR) * 4);
	buffer += sizeof(UCHAR) * 4;
	memcpy(&checkSum, buffer, sizeof(UCHAR));

}

// Write Tail data to buffer
void RDMSTail::write(LPSTR buffer) {

	memcpy(buffer, headerSymbols, sizeof(UCHAR) * 4);
	buffer += sizeof(UCHAR) * 4;
	memcpy(buffer, &checkSum, sizeof(UCHAR));

}
