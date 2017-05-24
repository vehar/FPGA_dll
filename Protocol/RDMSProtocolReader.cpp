#include "RDMSProtocolReader.hpp"


// Default constructor
RDMSProtocolReader::RDMSProtocolReader(LPCSTR fileName) : RDMSIProtocol() {

	// Pointer to file
	std::fstream* protocolFile = getFile();
	// Open file for reading
	protocolFile->open(fileName, std::ios::binary |
								 std::ios::in);

	// If file not opened
	if (protocolFile->is_open() == false) {

		// Stop constructing instantly
		return;

	}

	// Move file pointer to the end of file
	protocolFile->seekg(0, std::ios::end);

	// File size in bytes
	DWORD fileLength = protocolFile->tellg();
	// Move file pointer at the beginning of file
	protocolFile->seekg(0, std::ios::beg);
	// Save size
	fileSize = fileLength;

	// Allocate memory for file data
	fileData = new CHAR[fileLength];

	// If memory not allocated
	if (fileData == NULL) {
	
		protocolFile->close();
	
	}

}


// Read protocol function
void RDMSProtocolReader::readProtocol() {

	// Pointer to file
	std::fstream* protocolFile = getFile();
	// Read data from file to array
	protocolFile->read(fileData, fileSize);

}


// Destructor
RDMSProtocolReader::~RDMSProtocolReader() {

	// Pointer to file
	std::fstream* protocolFile = getFile();
	// Close file if opened
	if (protocolFile->is_open() == true) {

		protocolFile->close();

	}

	// Cleanup
	if (fileData != NULL) {
	
		delete [] fileData;
		fileData = NULL;
		fileSize = 0;
	
	}

}
