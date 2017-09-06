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

////////////////////
//
//	GPS data
//
struct RDMSGpsInfo {

	UCHAR		state;						//	0	1		GPS state flags
	float		lat;						//	1	4		Lattitude
	float		lon;						//	5	4		Longitude
	UCHAR		speed;						//	9	1		Speed (m/s)
	USHORT		course;						//	10	2		Course angle (0 - 359)
	UCHAR		sec;						//	12	1		Seconds (0 - 59)
	UCHAR		min;						//	13	1		Minutes (0 - 59)
	UCHAR		hour;						//	14	1		Hours (0 - 23)
	UCHAR		year;						//	15	1		Year (XXXX)
	UCHAR		month;						//	16	1		Month (1 - 12)
	UCHAR		day;						//	17	1		Day (1 - 31)

};											//	18

///////////////////////
//
//	Path sensor data
//
struct RDMSPathSensor {

	UCHAR		km;							//	0	1		Distance in kilometers
	USHORT		m;							//	1	2		Distance in meters
	UCHAR		sm;							//	3	1		Distance in santimeters
	UCHAR		mm;							//	4	1		Distance in millimeters

};											//	5

//////////////////
//
//	Speed data
//
struct RDMSSpeed {

	UCHAR		integer;					//	0	1		Integer part of speded value
	UCHAR		fractional;					//	1	1		Fractional part of speed value

};											//	2

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

	UCHAR			headerSymbols[4];		//	0	4		Header symbols ("RDMS")
	RDMSGpsInfo		gpsData;				//	5	18		5GPS coordinates
	ULONGLONG		timeData;				//	22	8		Current UNIX timestamp
	UCHAR			freeMemory;				//	30	1		Free memory (%)
	UCHAR			railType;				//	32	1		Rail type
	USHORT			operatorNumber;			//	33	2		Operator`s number
	UCHAR			direction;				//	34	1		Countdown direction (+, -)
	RDMSCtrlType	controlType;			//	35	1		Control type
	UCHAR			checkSum;				//	36	1		Check sum

	// Calculate check sum of packet
	UCHAR			calcCheckSum();

	// Return packet size
	USHORT			size();

	// Read packet data from buffer
	void			read(LPCSTR);
	// Write packet data to buffer
	void			write(LPSTR);

};											//	37 bytes


//////////////////////////////////////
//
//	Speed data packet
//
struct RDMSSpeedPacket : RDMSIPacket {

	UCHAR		startByte;					//	0	1		Start byte
	UCHAR		timeDifference;				//	1	1		Difference with previous timestamp
	RDMSSpeed	speed;						//	2	2		Movement speed
	UCHAR		checkSum;					//	4	1		Check sum
	UCHAR		endByte;					//	5	1		End byte

	// Calculate check sum of packet
	UCHAR		calcCheckSum();

	// Return packet size
	USHORT		size();

	// Read packet data from buffer
	void		read(LPCSTR);
	// Write packet data to buffer
	void		write(LPSTR);

};											// 6

///////////////////////
//
//	Defect data
//
struct RDMSDefectData {

	UCHAR		amplitude;					//	0	1		Signal amplitude
	USHORT		depth;						//	1	2		Defect depth (mm)

};											//	3

///////////////////////////////////////
//
//	Defect data packet
//
struct RDMSDefectPacket : RDMSIPacket {

	UCHAR			startByte;				//	0	1		Start byte
	RDMSPathSensor	pathSensorData;			//	1	5		Path sensor coordinate
	RDMSGpsInfo		gpsData;				//	6	18		GPS coordinates
	RDMSDefectData	defects[8];				//	25	3 * 8	Defect data structures fro 8 channels
	UCHAR			checkSum;				//	50	1		Check sum
	UCHAR			endByte;				//	51	1		End byte

	// Calculate check sum of packet
	UCHAR			calcCheckSum();

	// Return packet size
	USHORT			size();

	// Read packet data from buffer
	void			read(LPCSTR);
	// Write packet data to buffer
	void			write(LPSTR);

};											//	52

///////////////////////////////
//
//	Unicode text data structure
//
struct RDMSTextData {

	UCHAR		nameLength;					//	0	1		Name length (characters)
	WCHAR		name[256];					//	1	0..255	Name data (UNICODE)

};											//	1..510

///////////////////////////////////////////
//
//	Track coordinate packet
//
struct RDMSTrackCoordPacket : RDMSIPacket {

	UCHAR			startByte;				//	0	1		Start byte
	RDMSGpsInfo		gpsData;				//	1	18		GPS coordinate
	RDMSPathSensor	pathSensorData;			//	20	5		Path sensor coordinate
	//USHORT			railNumber;				//	25	2		Rail number
	//UCHAR			thread;					//	27	1		Left or right thread
	//RDMSTextData	stageName;				//	28	1..510	Track coordinate stage data
	UCHAR			checkSum;				//	538	1		Check sum
	UCHAR			endByte;				//	539	1		End byte

	// Calculate check sum of packet
	UCHAR			calcCheckSum();

	// Return packet size
	USHORT			size();

	// Read packet data from buffer
	void			read(LPCSTR);
	// Write packet data to buffer
	void			write(LPSTR);

};											//	540

///////////////////////////
//
//	Km stock position data
//
struct RDMSRailPos {

	UCHAR		km;							//	0	1		Kilometers
	USHORT		m;							//	1	2		Meters
	UCHAR		trackNumber;				//	3	1		Track number

};											//	4

////////////////////////////////////////
//
//	Km stock packet
//
struct RDMSKmStockPacket : RDMSIPacket {

	UCHAR			startByte;				//	0	1		Start byte
	RDMSRailPos		position;				//	1	4		Km stock position data
	USHORT			railNum;				//	5	2		Rail number
	RDMSTextData	stageName;				//	7	1..510	Km stock stage name data
	UCHAR			checkSum;				//	517	1		Check sum
	UCHAR			endByte;				//	518	1		End byte

	// Calculate check sum of packet
	UCHAR			calcCheckSum();

	// Return packet size
	USHORT			size();

	// Read packet data from buffer
	void			read(LPCSTR);
	// Write packet data to buffer
	void			write(LPSTR);

};											//	519

////////////////////////////////////
//
//	RSP packet
//
struct RDMSRspPacket : RDMSIPacket {

	UCHAR			startByte;				//	0	1		Start byte
	USHORT			rackNum;				//	1	2		Rack number
	USHORT			railNum;				//	3	2		Rail number
	RDMSTextData	nameData;				//	5	1..510	RSP name data
	UCHAR			checkSum;				//	515	1		Check sum
	UCHAR			endByte;				//	516	1		End byte

	// Calculate check sum of packet
	UCHAR			calcCheckSum();

	// Return packet size
	USHORT			size();

	// Read packet data from buffer
	void			read(LPCSTR);
	// Write packet data to buffer
	void			write(LPSTR);

};											//	517

////////////////////////////////////////
//
//	Turnout packet
//
struct RDMSTurnoutPacket : RDMSIPacket {

	UCHAR			startByte;				//	0	1		Start byte
	RDMSRailPos		position;				//	1	4		Turnout position data
	UCHAR			type;					//	5	1		Turnout type
	RDMSTextData	stageName;				//	6	1..510	Turnout stage name data
	UCHAR			checkSum;				//	516	1		Check sum
	UCHAR			endByte;				//	517	1		End byte

	// Calculate check sum of packet
	UCHAR			calcCheckSum();

	// Return packet size
	USHORT			size();

	// Read packet data from buffer
	void			read(LPCSTR);
	// Write packet data to buffer
	void			write(LPSTR);

};											//	518

/////////////////////////////////////////
//
//	Acoustic contact changed packet
//
struct RDMSAcousticPacket : RDMSIPacket {

	UCHAR		startByte;					//	0	1		Start byte
	UCHAR		contact;					//	1	1		Acoustic contact (Yes / No)
	UCHAR		checkSum;					//	2	1		Check sum
	UCHAR		endByte;					//	3	1		End byte

	// Calculate check sum of packet
	UCHAR		calcCheckSum();

	// Return packet size
	USHORT		size();

	// Read packet data from buffer
	void		read(LPCSTR);
	// Write packet data to buffer
	void		write(LPSTR);

};											// 4

/////////////////////////////////////
//
//	Gain changed packet
//
struct RDMSGainPacket : RDMSIPacket {

	UCHAR		startByte;					//	0	1		Start byte
	UCHAR		channelNum;					//	1	1		Channel number
	UCHAR		gain;						//	2	1		Gain
	UCHAR		checkSum;					//	3	1		Check sum
	UCHAR		endByte;					//	4	1		End byte

	// Calculate check sum of packet
	UCHAR		calcCheckSum();

	// Return packet size
	USHORT		size();

	// Read packet data from buffer
	void		read(LPCSTR);
	// Write packet data to buffer
	void		write(LPSTR);

};											//	5

/////////////////////////////////////
//
//	Synchronization changed packet
//
struct RDMSSyncPacket : RDMSIPacket {

	UCHAR		startByte;					//	0	1		Start byte
	UCHAR		syncronization;				//	1	1		Syncronization
	UCHAR		checkSum;					//	2	1		Check sum
	UCHAR		endByte;					//	3	1		End byte

	// Calculate check sum of packet
	UCHAR		calcCheckSum();

	// Return packet size
	USHORT		size();

	// Read packet data from buffer
	void		read(LPCSTR);
	// Write packet data to buffer
	void		write(LPSTR);

};											//	4

/////////////////////////////////////
//
//	Voltage/Temperature packet
//
struct RDMSVoltageTempPacket : RDMSIPacket {

	UCHAR		startByte;					//	0	1		Start byte
	float		stmTemperature;				//	1	4		STM temperature
	float		axelTemperature;			//	5	4		Axelerometer temperature
	float		accumCurrent;				//	9	4		Accumulator current
	float		accumVoltage;				//	13	4		Accummulator voltage
	float		board_1_8V;					//	17	4		Board 1.8 V
	float		board_3_3V;					//	21	4		Board 3.3 V
	float		board_5V;					//	25	4		Board 5 V
	float		board_140V;					//	29	4		Board 140 V
	UCHAR		checkSum;					//	33	1		Check sum
	UCHAR		endByte;					//	34	1		End byte

	// Calculate check sum of packet
	UCHAR		calcCheckSum();

	// Return packet size
	USHORT		size();

	// Read packet data from buffer
	void		read(LPCSTR);
	// Write packet data to buffer
	void		write(LPSTR);

};											//	35

//////////////////
//
//	RDMS file tail
//
struct RDMSTail : RDMSIPacket {

	UCHAR		headerSymbols[4];			//	0	1		Header symbols ("RDMS")
	UCHAR		checkSum;					//	1	4		File check sum

	// Calculate check sum of packet
	UCHAR		calcCheckSum();

	// Return packet size
	USHORT		size();

	// Read packet data from buffer
	void		read(LPCSTR);
	// Write packet data to buffer
	void		write(LPSTR);

};											//	5


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
