#pragma once

#include "RDMSIProtocol.hpp"

// Protocol interface forward declaration
class RDMSIProtocol;

// Protocol class
class RDMSProtocolReader : private RDMSIProtocol {

	private:

		LPSTR			fileData;		// Readed from file data
		DWORD			fileSize;		// Readed data size;

		// Default constructor
		RDMSProtocolReader();


	public:

		// Constructor
		explicit		RDMSProtocolReader(LPCSTR);

		// Read protocol function
		void			readProtocol();

		// Destructor
		~RDMSProtocolReader();

};
