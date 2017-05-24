#pragma once

#include "RDMSIProtocol.hpp"

// Default ring buffer size
#define		RDMS_DEFAULT_RING_BUFFER_SIZE		2048

// Protocol interface forward declaration
class RDMSIProtocol;

// Protocol class
class RDMSProtocolWriter : public RDMSIProtocol {

	private:

		LPSTR			currentBuffer;		// 

		LPSTR			firstBuffer;		// First buffer for data to write
		LPSTR			secondBuffer;		// Second buffer for data to write

		LPSTR			firstBufferWrite;	// 
		LPSTR			secondBufferWrite;	// 

		DWORD			bufferSize;			// Buffers size

		// Default constructor
		RDMSProtocolWriter() {}


	public:

		// Constructor
		RDMSProtocolWriter(LPCWSTR, const RDMSHeader*);

		// Write header to protocol
		void			writeHeader(RDMSHeader*);

		// Read protocol
		void			readProtocol() {};
		// Write packet to protocol
		void			writePacket(RDMSIPacket&, const DWORD&);

		// Destructor
		~RDMSProtocolWriter();

};
