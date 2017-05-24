#include "RDMSProtocolWriter.hpp"


// Constructor #2
RDMSProtocolWriter::RDMSProtocolWriter(LPCWSTR fileName,
									   const RDMSHeader* fileHeader) {

	// Pointer to file
	std::fstream* protocolFile = getFile();
	// Open file for reading
	protocolFile->open(fileName, std::ios::binary | std::ios::out);

	// Check if file opened
	if (protocolFile->is_open() == false) {
	
		return;
	
	}

	// Set header from outside
	RDMSIProtocol::setHeader(*fileHeader);
	memcpy(RDMSIProtocol::getHeader().headerSymbols, RDMS_HEADER_SYMBOLS, 4);

	// Allocate memory for
	LPSTR data = new CHAR[RDMSIProtocol::getHeader().size()];
	// Copy header data to array
	RDMSIProtocol::getHeader().write(data);

	// Write data to file
	protocolFile->write(data, RDMSIProtocol::getHeader().size());

	// Cleanup
	delete [] data;

}

// Write header to protocol
void RDMSProtocolWriter::writeHeader(RDMSHeader* fileHeader) {

	// Pointer to file
	std::fstream* protocolFile = getFile();

	// Copy header data
	RDMSIProtocol::setHeader(*fileHeader);

	// Allocate memory for
	LPSTR data = new CHAR[RDMSIProtocol::getHeader().size()];
	// Copy header data to array
	RDMSIProtocol::getHeader().write(data);

	// Move file pointer at the beginning of file
	protocolFile->seekg(0, std::ios::beg);
	// Write data to file
	protocolFile->write(data, RDMSIProtocol::getHeader().size());
	// Move file pointer to the end of file
	protocolFile->seekg(0, std::ios::end);

	// Cleanup
	delete [] data;

}

// Write packet to protocol
void RDMSProtocolWriter::writePacket(RDMSIPacket &packet, const DWORD &size) {

	// Pointer to file
	std::fstream* protocolFile = getFile();

	// Allocate memory for packet
	CHAR* data = new CHAR[size];
	// Write packet to allocated memory
	packet.write(data);

	// Write packet data to file
	protocolFile->write(data, size);

	// Cleanup
	delete [] data;

}

// Destructor
RDMSProtocolWriter::~RDMSProtocolWriter() {

	// Set tail data
	memcpy(RDMSIProtocol::getTail().headerSymbols, RDMS_HEADER_SYMBOLS, 4);
	RDMSIProtocol::getTail().checkSum = 0x05;

	// Pointer to file
	std::fstream* protocolFile = getFile();

	// Allocate memory for packet
	LPSTR data = new CHAR[RDMSIProtocol::getTail().size()];
	// Write packet to allocated memory
	RDMSIProtocol::getTail().write(data);
	// Write packet data to file
	protocolFile->write(data, RDMSIProtocol::getTail().size());

	// Cleanup
	delete [] data;

	// Close file if opened
	if (protocolFile->is_open() == true) {

		protocolFile->close();

	}

}