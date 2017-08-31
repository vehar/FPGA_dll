#pragma once

#include <windows.h>
#include <fstream>


// Header symbol
#define		RDMS_HEADER_SYMBOLS		"RDMS"


/////////////////////////////
//
//	Packet type enumeration
//
enum RDMSPacketType : UCHAR {

	RDMS_UNKNOWN_PACKET			= 0x00,		// Unknown/error packet
	RDMS_SPEED_PACKET			= 0x01,		// Speed packet
	RDMS_DEFECT_PACKET			= 0x02,		// Defect packet
	RDMS_START_COORD_PACKET		= 0x03,		// Track start coordinate packet
	RDMS_END_COORD_PACKET		= 0x04,		// Track end coordinate packet
	RDMS_KM_STOCK_PACKET		= 0x05,		// Km stock packet
	RDMS_RSP_PACKET				= 0x06,		// RSP packet
	RDMS_TURNOUT_PACKET			= 0x07,		// Turnout packet
	RDMS_ACOUSTIC_PACKET		= 0x08,		// Acoustic contact packet
	RDMS_GAIN_PACKET			= 0x09,		// Gain packet
	RDMS_SYNC_PACKET			= 0x0A,		// Syncronization packet
	RDMS_VOLTAGE_TEMP_PACKET	= 0x0B		// Voltage and temperature packet

};

////////////////////////////////////////
//
//	Lattitude/Longitude flag enumeration
//
enum RDMSGpsLatLongFlag : UCHAR {

	RDMS_GPS_NE		= 0x00,					// North-East position
	RDMS_GPS_NW		= 0x0F,					// North-West position
	RDMS_GPS_SE		= 0xF0,					// South-East position
	RDMS_GPS_SW		= 0xFF					// South-West position

};

/*
////////////////////
//
//	GPS data
//
struct RDMSGpsInfo {

	UCHAR		state;						// GPS state flags
	float		lat;						// Lattitude
	float		lon;						// Longitude
	UCHAR		speed;						// Speed (m/s)
	USHORT		course;						// Course angle (0 - 359)
	UCHAR		sec;						// Seconds (0 - 59)
	UCHAR		min;						// Minutes (0 - 59)
	UCHAR		hour;						// Hours (0 - 23)
	UCHAR		year;						// Year (XXXX)
	UCHAR		month;						// Month (1 - 12)
	UCHAR		day;						// Day (1 - 31)

};
*/

////////////////////
//
//	GPS data
//
struct RDMSGpsInfo {

	UCHAR		state;						// GPS state flags
	float		lat;						// Lattitude
	float		lon;						// Longitude
	UCHAR		speed;						// Speed (m/s)
	USHORT		course;						// Course angle (0 - 359)
	UCHAR		sec;						// Seconds (0 - 59)
	UCHAR		min;						// Minutes (0 - 59)
	UCHAR		hour;						// Hours (0 - 23)
	UCHAR		year;						// Year (XXXX)
	UCHAR		month;						// Month (1 - 12)
	UCHAR		day;						// Day (1 - 31)

};

///////////////////////
//
//	Path sensor data
//
struct RDMSPathSensor {

	UCHAR		km;							// Distance in kilometers
	USHORT		m;							// Distance in meters
	UCHAR		sm;							// Distance in santimeters
	UCHAR		mm;							// Distance in millimeters

};

//////////////////
//
//	Speed data
//
struct RDMSSpeed {

	UCHAR		integer;					// Integer part of speded value
	UCHAR		fractional;					// Fractional part of speed value

};

///////////////////////////
//
//	Rail control type
//
enum RDMSCtrlType : UCHAR {

	RDMS_CONTROL_TYPE_TRACK		= 0x00,		// Track control
	RDMS_CONTROL_TYPE_KM_STOCK	= 0x01,		// Km stock control
	RDMS_CONTROL_TYPE_RSP		= 0x02,		// RSP control
	RDMS_CONTROL_TYPE_TURNOUT	= 0x03		// Turnout control

};

//////////////////////////////
//
//	Packet interface structure
//
struct RDMSIPacket {

	// Calculate check sum of packet
	virtual UCHAR	calcCheckSum()	= 0;

	// Return packet size
	virtual USHORT	size()			= 0;

	// Read packet data from buffer
	virtual void	read(LPCSTR)	= 0;
	// Write packet data to buffer
	virtual void	write(LPSTR)	= 0;

};

////////////////////
//
//	RDMS file header
//
struct RDMSHeader : RDMSIPacket {

	UCHAR			headerSymbols[4];		// Header symbols ("RDMS")
	RDMSGpsInfo		gpsData;				// GPS coordinates
	ULONGLONG		timeData;				// Current UNIX timestamp
	UCHAR			freeMemory;				// Free memory (%)
	UCHAR			railType;				// Rail type
	USHORT			operatorNumber;			// Operator`s number
	UCHAR			direction;				// Countdown direction (+, -)
	RDMSCtrlType	controlType;			// Control type
	UCHAR			checkSum;				// Check sum

	// Calculate check sum of packet
	UCHAR			calcCheckSum();

	// Return packet size
	USHORT			size();

	// Read packet data from buffer
	void			read(LPCSTR);
	// Write packet data to buffer
	void			write(LPSTR);

};


//////////////////////////////////////
//
//	Speed data packet
//
struct RDMSSpeedPacket : RDMSIPacket {

	UCHAR		startByte;					// Start byte
	UCHAR		timeDifference;				// Difference with previous timestamp
	RDMSSpeed	speed;						// Movement speed
	UCHAR		checkSum;					// Check sum
	UCHAR		endByte;					// End byte

	// Calculate check sum of packet
	UCHAR		calcCheckSum();

	// Return packet size
	USHORT		size();

	// Read packet data from buffer
	void		read(LPCSTR);
	// Write packet data to buffer
	void		write(LPSTR);

};

///////////////////////
//
//	Defect data
//
struct RDMSDefectData {

	UCHAR		amplitude;					// Signal amplitude
	USHORT		depth;						// Defect depth (mm)

};

///////////////////////////////////////
//
//	Defect data packet
//
struct RDMSDefectPacket : RDMSIPacket {

	UCHAR			startByte;				// Start byte
	RDMSPathSensor	pathSensorData;			// Path sensor coordinate
	RDMSGpsInfo		gpsData;				// GPS coordinates
	RDMSDefectData	defects[8];				// Defect data structures fro 8 channels
	UCHAR			checkSum;				// Check sum
	UCHAR			endByte;				// End byte

	// Calculate check sum of packet
	UCHAR			calcCheckSum();

	// Return packet size
	USHORT			size();

	// Read packet data from buffer
	void			read(LPCSTR);
	// Write packet data to buffer
	void			write(LPSTR);

};

///////////////////////////////
//
//	Unicode text data structure
//
struct RDMSTextData {

	UCHAR		nameLength;					// Name length (characters)
	WCHAR		name[256];					// Name data (UNICODE)

};

///////////////////////////////////////////
//
//	Track coordinate packet
//
struct RDMSTrackCoordPacket : RDMSIPacket {

	UCHAR			startByte;				// Start byte
	RDMSGpsInfo		gpsData;				// GPS coordinate
	RDMSPathSensor	pathSensorData;			// Path sensor coordinate
	USHORT			railNumber;				// Rail number
	UCHAR			thread;					// Left or right thread
	RDMSTextData	stageName;				// Track coordinate stage data
	UCHAR			checkSum;				// Check sum
	UCHAR			endByte;				// End byte

	// Calculate check sum of packet
	UCHAR			calcCheckSum();

	// Return packet size
	USHORT			size();

	// Read packet data from buffer
	void			read(LPCSTR);
	// Write packet data to buffer
	void			write(LPSTR);

};

///////////////////////////
//
//	Km stock position data
//
struct RDMSRailPos {

	UCHAR		km;							// Kilometers
	USHORT		m;							// Meters
	UCHAR		trackNumber;				// Track number

};

////////////////////////////////////////
//
//	Km stock packet
//
struct RDMSKmStockPacket : RDMSIPacket {

	UCHAR			startByte;				// Start byte
	RDMSRailPos		position;				// Km stock position data
	//USHORT			railwayRunNum;			// Railway number
	USHORT			railNum;				// Rail number
	RDMSTextData	stageName;				// Km stock stage name data
	UCHAR			checkSum;				// Check sum
	UCHAR			endByte;				// End byte

	// Calculate check sum of packet
	UCHAR			calcCheckSum();

	// Return packet size
	USHORT			size();

	// Read packet data from buffer
	void			read(LPCSTR);
	// Write packet data to buffer
	void			write(LPSTR);

};

////////////////////////////////////
//
//	RSP packet
//
struct RDMSRspPacket : RDMSIPacket {

	UCHAR			startByte;				// Start byte
	USHORT			rackNum;				// Rack number
	USHORT			railNum;				// Rail number
	RDMSTextData	nameData;				// RSP name data
	UCHAR			checkSum;				// Check sum
	UCHAR			endByte;				// End byte

	// Calculate check sum of packet
	UCHAR			calcCheckSum();

	// Return packet size
	USHORT			size();

	// Read packet data from buffer
	void			read(LPCSTR);
	// Write packet data to buffer
	void			write(LPSTR);

};

////////////////////////////////////////
//
//	Turnout packet
//
struct RDMSTurnoutPacket : RDMSIPacket {

	UCHAR			startByte;				// Start byte
	RDMSRailPos		position;				// Turnout position data
	UCHAR			type;					// Turnout type
	RDMSTextData	stageName;				// Turnout stage name data
	UCHAR			checkSum;				// Check sum
	UCHAR			endByte;				// End byte

	// Calculate check sum of packet
	UCHAR			calcCheckSum();

	// Return packet size
	USHORT			size();

	// Read packet data from buffer
	void			read(LPCSTR);
	// Write packet data to buffer
	void			write(LPSTR);

};

/////////////////////////////////////////
//
//	Acoustic contact changed packet
//
struct RDMSAcousticPacket : RDMSIPacket {

	UCHAR		startByte;					// Start byte
	UCHAR		contact;					// Acoustic contact (Yes / No)
	UCHAR		checkSum;					// Check sum
	UCHAR		endByte;					// End byte

	// Calculate check sum of packet
	UCHAR		calcCheckSum();

	// Return packet size
	USHORT		size();

	// Read packet data from buffer
	void		read(LPCSTR);
	// Write packet data to buffer
	void		write(LPSTR);

};

/////////////////////////////////////
//
//	Gain changed packet
//
struct RDMSGainPacket : RDMSIPacket {

	UCHAR		startByte;					// Start byte
	UCHAR		channelNum;					// Channel number
	UCHAR		gain;						// Gain
	UCHAR		checkSum;					// Check sum
	UCHAR		endByte;					// End byte

	// Calculate check sum of packet
	UCHAR		calcCheckSum();

	// Return packet size
	USHORT		size();

	// Read packet data from buffer
	void		read(LPCSTR);
	// Write packet data to buffer
	void		write(LPSTR);

};

/////////////////////////////////////
//
//	Synchronization changed packet
//
struct RDMSSyncPacket : RDMSIPacket {

	UCHAR		startByte;					// Start byte
	UCHAR		syncronization;				// Syncronization
	UCHAR		checkSum;					// Check sum
	UCHAR		endByte;					// End byte

	// Calculate check sum of packet
	UCHAR		calcCheckSum();

	// Return packet size
	USHORT		size();

	// Read packet data from buffer
	void		read(LPCSTR);
	// Write packet data to buffer
	void		write(LPSTR);

};

/////////////////////////////////////
//
//	Voltage/Temperature packet
//
struct RDMSVoltageTempPacket : RDMSIPacket {

	UCHAR		startByte;					// Start byte
	float		stmTemperature;				// STM temperature
	float		axelTemperature;			// Axelerometer temperature
	float		accumCurrent;				// Accumulator current
	float		accumVoltage;				// Accummulator voltage
	float		board_1_8V;					// Board 1.8 V
	float		board_3_3V;					// Board 3.3 V
	float		board_5V;					// Board 5 V
	float		board_140V;					// Board 140 V
	UCHAR		checkSum;					// Check sum
	UCHAR		endByte;					// End byte

	// Calculate check sum of packet
	UCHAR		calcCheckSum();

	// Return packet size
	USHORT		size();

	// Read packet data from buffer
	void		read(LPCSTR);
	// Write packet data to buffer
	void		write(LPSTR);

};

//////////////////
//
//	RDMS file tail
//
struct RDMSTail : RDMSIPacket {

	UCHAR		headerSymbols[4];			// Header symbols ("RDMS")
	UCHAR		checkSum;					// File check sum

	// Calculate check sum of packet
	UCHAR		calcCheckSum();

	// Return packet size
	USHORT		size();

	// Read packet data from buffer
	void		read(LPCSTR);
	// Write packet data to buffer
	void		write(LPSTR);

};


//////////////////////
//
//	Protocol interface
//
class RDMSIProtocol {

	private:
	
		RDMSHeader		header;				// Protocol header
		RDMSTail		tail;				// Protocol tail

		std::fstream	file;				// File stream for protocol file


	public:

		// Default constructor
		RDMSIProtocol();

		// Pure virtual read protocol function
		virtual void	readProtocol()							= 0;
		// Pure virtual write packet function
		virtual void	writePacket(RDMSIPacket&, const DWORD&)	= 0;

		// Get file
		std::fstream*	getFile();
		// Get file header
		RDMSHeader&		getHeader();
		// Get file tail
		RDMSTail&		getTail();

		// Set file header
		void			setHeader(const RDMSHeader&);
		// Set file tail
		void			setTail(const RDMSTail&);

		// Destructor
		virtual			~RDMSIProtocol();

};
