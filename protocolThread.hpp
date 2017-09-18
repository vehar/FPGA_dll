#ifndef H_PROTOCOL_THREAD
#define H_PROTOCOL_THREAD


#include "StructureData.h"


struct execInfo {

	// Thread execution flag
	// True - thread runs now, false - thread will stop
	BOOL			threadExecuting;
	// Protocol header data
	protocolData*	pHeader;

};

static execInfo* execStruct = NULL;

// Thread handle for control
static HANDLE protThread = NULL;

// Start protocol thread
DWORD startProtocolThread(protocolData*);
// Stop protocol thread
void stopProtocolThread();

// Protocol thread
DWORD WINAPI protocolThreadFunc(LPVOID);

extern	DWORD wordNum;

#endif	// H_PROTOCOL_THREAD
