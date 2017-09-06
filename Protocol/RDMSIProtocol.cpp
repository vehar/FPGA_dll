#include <iostream>

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

	UCHAR chk	=	headerSymbols[0];
	chk			^=	headerSymbols[1];
	chk			^=	headerSymbols[2];
	chk			^=	headerSymbols[3];

	chk			^=	gpsData.state;
	chk			^=	((*(int*)&gpsData.lat) & 0xFF000000) >> 24;
	chk			^=	((*(int*)&gpsData.lat) & 0x00FF0000) >> 16;
	chk			^=	((*(int*)&gpsData.lat) & 0x0000FF00) >> 8;
	chk			^=	((*(int*)&gpsData.lat) & 0x000000FF);
	chk			^=	((*(int*)&gpsData.lon) & 0xFF000000) >> 24;
	chk			^=	((*(int*)&gpsData.lon) & 0x00FF0000) >> 16;
	chk			^=	((*(int*)&gpsData.lon) & 0x0000FF00) >> 8;
	chk			^=	((*(int*)&gpsData.lon) & 0x000000FF);
	chk			^=	gpsData.speed;
	chk			^=	(gpsData.course & 0xFF00) >> 8;
	chk			^=	(gpsData.course & 0x00FF);
	chk			^=	gpsData.sec;
	chk			^=	gpsData.min;
	chk			^=	gpsData.hour;
	chk			^=	gpsData.year;
	chk			^=	gpsData.month;
	chk			^=	gpsData.day;

	chk			^=	(timeData & 0xFF00000000000000) >> 56;
	chk			^=	(timeData & 0x00FF000000000000) >> 48;
	chk			^=	(timeData & 0x0000FF0000000000) >> 40;
	chk			^=	(timeData & 0x000000FF00000000) >> 32;
	chk			^=	(timeData & 0x00000000FF000000) >> 24;
	chk			^=	(timeData & 0x0000000000FF0000) >> 16;
	chk			^=	(timeData & 0x000000000000FF00) >> 8;
	chk			^=	(timeData & 0x00000000000000FF);
	chk			^=	freeMemory;
	chk			^=	railType;
	chk			^=	(operatorNumber & 0xFF00) >> 8;
	chk			^=	(operatorNumber & 0x00FF);
	chk			^=	direction;
	chk			^=	controlType;

	return chk;

}

// Return packet size
USHORT RDMSHeader::size() {

	return 0x0025;

}

// Read packet data from buffer
void RDMSHeader::read(LPCSTR buffer) {

	memcpy(headerSymbols, buffer, sizeof(UCHAR) * 4);
	buffer += sizeof(UCHAR) * 4;

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
	buffer += sizeof(UCHAR);
	memcpy(&checkSum, buffer, sizeof(UCHAR));

}

// Write packet data to buffer
void RDMSHeader::write(LPSTR buffer) {

	memcpy(buffer, headerSymbols, sizeof(UCHAR) * 4);
	buffer += sizeof(UCHAR) * 4;

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
	buffer += sizeof(UCHAR);
	memcpy(buffer, &checkSum, sizeof(UCHAR));

}


// Calculate check sum of packet
UCHAR RDMSSpeedPacket::calcCheckSum() {

	UCHAR chk	=	timeDifference;
	chk			^=	speed.integer;
	chk			^=	speed.fractional;

	return chk;

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

	UCHAR chk	=	pathSensorData.km;
	chk			^=	pathSensorData.m;
	chk			^=	pathSensorData.sm;
	chk			^=	pathSensorData.mm;

	chk			^=	gpsData.state;
	chk			^=	((*(int*)&gpsData.lat) & 0xFF000000) >> 24;
	chk			^=	((*(int*)&gpsData.lat) & 0x00FF0000) >> 16;
	chk			^=	((*(int*)&gpsData.lat) & 0x0000FF00) >> 8;
	chk			^=	((*(int*)&gpsData.lat) & 0x000000FF);
	chk			^=	((*(int*)&gpsData.lon) & 0xFF000000) >> 24;
	chk			^=	((*(int*)&gpsData.lon) & 0x00FF0000) >> 16;
	chk			^=	((*(int*)&gpsData.lon) & 0x0000FF00) >> 8;
	chk			^=	((*(int*)&gpsData.lon) & 0x000000FF);
	chk			^=	gpsData.speed;
	chk			^=	(gpsData.course & 0xFF00) >> 8;
	chk			^=	(gpsData.course & 0x00FF);
	chk			^=	gpsData.sec;
	chk			^=	gpsData.min;
	chk			^=	gpsData.hour;
	chk			^=	gpsData.year;
	chk			^=	gpsData.month;
	chk			^=	gpsData.day;

	for (DWORD i = 0; i < 8; ++i) {

		chk		^=	defects[i].amplitude;
		chk		^=	(defects[i].depth & 0xFF00) >> 8;
		chk		^=	(defects[i].depth & 0x00FF);

	}

	return chk;

}

// Return packet size
USHORT RDMSDefectPacket::size() {

	return 0x0032;

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

	UCHAR chk	=	gpsData.state;
	chk			^=	((*(int*)&gpsData.lat) & 0xFF000000) >> 24;
	chk			^=	((*(int*)&gpsData.lat) & 0x00FF0000) >> 16;
	chk			^=	((*(int*)&gpsData.lat) & 0x0000FF00) >> 8;
	chk			^=	((*(int*)&gpsData.lat) & 0x000000FF);
	chk			^=	((*(int*)&gpsData.lon) & 0xFF000000) >> 24;
	chk			^=	((*(int*)&gpsData.lon) & 0x00FF0000) >> 16;
	chk			^=	((*(int*)&gpsData.lon) & 0x0000FF00) >> 8;
	chk			^=	((*(int*)&gpsData.lon) & 0x000000FF);
	chk			^=	gpsData.speed;
	chk			^=	(gpsData.course & 0xFF00) >> 8;
	chk			^=	(gpsData.course & 0x00FF);
	chk			^=	gpsData.sec;
	chk			^=	gpsData.min;
	chk			^=	gpsData.hour;
	chk			^=	gpsData.year;
	chk			^=	gpsData.month;
	chk			^=	gpsData.day;

	chk			^=	pathSensorData.km;
	chk			^=	(pathSensorData.m & 0xFF00) >> 8;
	chk			^=	(pathSensorData.m & 0x00FF);
	chk			^=	pathSensorData.sm;
	chk			^=	pathSensorData.mm;

	return chk;

}

// Return packet size
USHORT RDMSTrackCoordPacket::size() {

	return 0x001A;//0x0013 + stageName.nameLength * sizeof(WCHAR);

}

// Read Track coordinates packet data from buffer
void RDMSTrackCoordPacket::read(LPCSTR buffer) {

	memcpy(&startByte, buffer, sizeof(UCHAR));
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

	memcpy(&pathSensorData.km, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&pathSensorData.m, buffer, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(&pathSensorData.sm, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&pathSensorData.mm, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	//memcpy(&railNumber, buffer, sizeof(USHORT));
	//buffer += sizeof(USHORT);
	//memcpy(&thread, buffer, sizeof(UCHAR));
	//buffer += sizeof(UCHAR);
	//memcpy(&stageName.nameLength, buffer, sizeof(UCHAR));
	//buffer += sizeof(UCHAR);
	//memcpy(&stageName.name, buffer, sizeof(WCHAR) * stageName.nameLength);
	//stageName.name[stageName.nameLength] = L'\0';
	//buffer += sizeof(WCHAR) * stageName.nameLength;
	memcpy(&checkSum, buffer, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(&endByte, buffer, sizeof(UCHAR));

}

// Write Track coordinates packet data to buffer
void RDMSTrackCoordPacket::write(LPSTR buffer) {

	memcpy(buffer, &startByte, sizeof(UCHAR));
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

	memcpy(buffer, &pathSensorData.km, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &pathSensorData.m, sizeof(USHORT));
	buffer += sizeof(USHORT);
	memcpy(buffer, &pathSensorData.sm, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &pathSensorData.mm, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	//memcpy(buffer, &railNumber, sizeof(USHORT));
	//buffer += sizeof(USHORT);
	//memcpy(buffer, &thread, sizeof(UCHAR));
	//buffer += sizeof(UCHAR);
	//memcpy(buffer, &stageName.nameLength, sizeof(UCHAR));
	//buffer += sizeof(UCHAR);
	//memcpy(buffer, &stageName.name, sizeof(WCHAR) * stageName.nameLength);
	//buffer += sizeof(WCHAR) * stageName.nameLength;
	memcpy(buffer, &checkSum, sizeof(UCHAR));
	buffer += sizeof(UCHAR);
	memcpy(buffer, &endByte, sizeof(UCHAR));

}


// Calculate check sum of packet
UCHAR RDMSKmStockPacket::calcCheckSum() {

	UCHAR chk	=	position.km;
	chk			^=	(position.m & 0xFF00) >> 8;
	chk			^=	(position.m & 0x00FF);
	chk			^=	position.trackNumber;
	chk			^=	railNum;
	chk			^=	stageName.nameLength;

	for (int i = 0; i < stageName.nameLength; ++i) {

		chk		^=	stageName.name[i];

	}

	return chk;

}

// Return packet size
USHORT RDMSKmStockPacket::size() {

	return 0x000A + stageName.nameLength * sizeof(WCHAR);

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

	UCHAR chk	=	(rackNum & 0xFF00) >> 8;
	chk			^=	(rackNum & 0x00FF);
	chk			^=	(railNum & 0xFF00) >> 8;
	chk			^=	(railNum & 0x00FF);
	chk			^=	nameData.nameLength;

	for (int i = 0; i < nameData.nameLength; ++i) {

		chk		^=	nameData.name[i];

	}

	return chk;

}

// Return packet size
USHORT RDMSRspPacket::size() {

	return 0x0205;

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

	UCHAR chk	=	position.km;
	chk			^=	(position.m & 0xFF00) >> 8;
	chk			^=	(position.m & 0x00FF);
	chk			^=	position.trackNumber;
	chk			^=	type;
	chk			^=	stageName.nameLength;

	for (int i = 0; i < stageName.nameLength; ++i) {

		chk		^=	stageName.name[i];

	}

	return chk;

}

// Return packet size
USHORT RDMSTurnoutPacket::size() {

	return 0x0206;

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

	return contact;

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

	UCHAR chk	=	channelNum;
	chk			^=	gain;

	return chk;

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

	return syncronization;

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

	UCHAR chk	=	((*(int*)&stmTemperature) & 0xFF000000) >> 24;
	chk			^=	((*(int*)&stmTemperature) & 0x00FF0000) >> 16;
	chk			^=	((*(int*)&stmTemperature) & 0x0000FF00) >> 8;
	chk 		^=	((*(int*)&stmTemperature) & 0x000000FF);
	chk 		^=	((*(int*)&axelTemperature) & 0xFF000000) >> 24;
	chk 		^=	((*(int*)&axelTemperature) & 0x00FF0000) >> 16;
	chk 		^=	((*(int*)&axelTemperature) & 0x0000FF00) >> 8;
	chk 		^=	((*(int*)&axelTemperature) & 0x000000FF);
	chk 		^=	((*(int*)&accumCurrent) & 0xFF000000) >> 24;
	chk 		^=	((*(int*)&accumCurrent) & 0x00FF0000) >> 16;
	chk 		^=	((*(int*)&accumCurrent) & 0x0000FF00) >> 8;
	chk 		^=	((*(int*)&accumCurrent) & 0x000000FF);
	chk 		^=	((*(int*)&accumVoltage) & 0xFF000000) >> 24;
	chk 		^=	((*(int*)&accumVoltage) & 0x00FF0000) >> 16;
	chk 		^=	((*(int*)&accumVoltage) & 0x0000FF00) >> 8;
	chk 		^=	((*(int*)&accumVoltage) & 0x000000FF);
	chk 		^=	((*(int*)&board_1_8V) & 0xFF000000) >> 24;
	chk 		^=	((*(int*)&board_1_8V) & 0x00FF0000) >> 16;
	chk 		^=	((*(int*)&board_1_8V) & 0x0000FF00) >> 8;
	chk 		^=	((*(int*)&board_1_8V) & 0x000000FF);
	chk 		^=	((*(int*)&board_3_3V) & 0xFF000000) >> 24;
	chk 		^=	((*(int*)&board_3_3V) & 0x00FF0000) >> 16;
	chk 		^=	((*(int*)&board_3_3V) & 0x0000FF00) >> 8;
	chk 		^=	((*(int*)&board_3_3V) & 0x000000FF);
	chk 		^=	((*(int*)&board_5V) & 0xFF000000) >> 24;
	chk 		^=	((*(int*)&board_5V) & 0x00FF0000) >> 16;
	chk 		^=	((*(int*)&board_5V) & 0x0000FF00) >> 8;
	chk 		^=	((*(int*)&board_5V) & 0x000000FF);
	chk 		^=	((*(int*)&board_140V) & 0xFF000000) >> 24;
	chk 		^=	((*(int*)&board_140V) & 0x00FF0000) >> 16;
	chk 		^=	((*(int*)&board_140V) & 0x0000FF00) >> 8;
	chk 		^=	((*(int*)&board_140V) & 0x000000FF);

	return chk;

}

// Return packet size
USHORT RDMSVoltageTempPacket::size() {

	return 0x0023;

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
