#include "Freeze.h"


WCHAR filePath[1024];// Path to current app
WCHAR AScanFileName[25];
char AScanHeader[] = "A_SCAN_HEADER:";
char AScanValArr [LCD_WIDTH*5] = {0};
HANDLE hFile = NULL;

void ReadDisplayAScan(void);
void AScanFileWrite(LPCVOID lpParam); 
void AScanFileInit (void);


void AScanFileInit (void)
{	
DBG_SHOW_FUNC;


	SYSTEMTIME time;	
	GetLocalTime(&time);
	// Form file name as "log_<dd-mm-yyyy>_<hh-mm>.txt"
	wsprintf(AScanFileName,
			 L"AScan_%02i-%02i-%04i_%02i-%02i.txt\0",
			 time.wDay,
			 time.wMonth,
			 time.wYear,
			 time.wHour,
			 time.wMinute);

// WinCE does not have GetCurrentDirectory
// Microsoft - I'm lovin` it!
// Para-pa-pa-pa
#ifdef _WIN32_WCE
	GetModuleFileName(NULL, filePath, 1023);// Get path to current app
	LPWSTR it = wcsrchr(filePath, L'\\');	// Exclude .exe name from path
	if (it) {*++it = L'\0';}// Character after last slash in path should be EOL
#else
	GetCurrentDirectory(1023, filePath);	// Get path to current app
#endif	// _WIN32_WCE

	wcscat(filePath, AScanFileName);// Add file name to current app`s path
	HANDLE hFile = CreateFile(filePath,			// Open log file
							  GENERIC_WRITE,
							  FILE_SHARE_WRITE,
							  NULL,
							  CREATE_ALWAYS,
							  FILE_ATTRIBUTE_NORMAL,
							  0);
	
	if (hFile == INVALID_HANDLE_VALUE) // Error handling
	{
		MessageBox(NULL, L"Unable to create file!", L"LOG Error",MB_OK);
		//ExitThread(-1);
	}	
	DWORD bytesWriten = 0;// Counter of writen bytes for WriteFile	

	//this is not need!
	//WCHAR UTF16Header = 0xFEFF;// Add unicode text file header
	
	WriteFile(hFile, AScanHeader, sizeof(AScanHeader), &bytesWriten, NULL);// Save header to file
	SetFilePointer(hFile, 0, NULL, FILE_END);// Append to the end of file
	CloseHandle(hFile);// Close log file
}


void AScanFileWrite(LPCVOID lpParam) // Write data to file
{
DBG_SHOW_FUNC;
int AScanRawArr[LCD_WIDTH] = {0};
int AScanSmoothedArr[LCD_WIDTH] = {0};
int arr_2[LCD_WIDTH] = {0};

	// Open log file
	 hFile = CreateFile(filePath,
							  GENERIC_WRITE,
							  FILE_SHARE_WRITE,
							  NULL,
							  OPEN_ALWAYS,
							  FILE_ATTRIBUTE_NORMAL,
							  0);

	// Check if file opened
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, L"Unable to open file!", L"LOG Error", MB_OK);
		ExitThread(-1);
	}
	
	//SetFilePointer(hFile, 0, NULL, FILE_END);// Append to the end of file
	DWORD bytesWriten = 0;// Counter of writen bytes for WriteFile
	
	int writeSize = LCD_WIDTH;// Count size of data to write

	//convert int AScanRawArr to char AScanRawArr
	char token[3];
	int value = 0;
	for(int i=0; i<LCD_WIDTH; i++)
	{ 
		 value = AScanSmoothedArr[i];
         _itoa(value, token, 10);
		strcat(token, ", ");
		strcat(AScanValArr, token);
	} 
	strcat(AScanValArr, "END"); //add tail of Ascan sequence
	WriteFile(hFile, AScanValArr , sizeof(AScanValArr), &bytesWriten, NULL); 	// Write data to file//AScanSmoothedArr AScanRawArr with AScan
	CloseHandle(hFile);	// Close file
}

//char AScanValArr [LCD_WIDTH*5] = {0}; 
void ReadDisplayAScan(void)
{
	DBG_SHOW_FUNC;

	int adc_val = 0;
//	char token[3];
	int start = 0;
	 DWORD byteReaded;

	WCHAR tmp_path[500] = {};

	wsprintf(tmp_path, L"\\Program Files\\DM-FPGA_GeneratorsTest\\AScan_11-11-2016_14-12.txt");

		// Open log file
	 hFile = CreateFile(filePath, //tmp_path, // \Program Files\DM-FPGA_GeneratorsTest\AScan_11-11-2016_11-56.txt
								//   \Program Files\DM-FPGA_GeneratorsTest\AScan_11-11-2016_11-56.txt	
							  GENERIC_READ,
							  0,
							  NULL,
							  OPEN_EXISTING,
							  FILE_ATTRIBUTE_NORMAL,
							  0);

	// Check if file opened
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, L"Unable to open file!", L"LOG Error", MB_OK);
		ExitThread(-1);
	}

	ReadFile(hFile, AScanValArr, sizeof(AScanValArr), &byteReaded, NULL) ; 

	//header search
	if(!strncmp(AScanValArr, AScanHeader, sizeof(AScanHeader))){start = sizeof(AScanHeader);}
	else {start = 0;}

	 char * pch = strtok (AScanValArr+sizeof(AScanHeader)," ,"); // во втором параметре указаны разделитель	 			 
	 
	FPGA.setAScanDrawMode(1); 
	FPGA.setAScanWrCS(1);

//	FPGA_Write(RamCntRdRst ,0x01);
	//FPGA_Write(AScanRamCntRdRst ,0x01);
//	FPGA_Write(AScanStartAddrWr ,0x00);
			
	//for(int i = start; i<272; i++)
	 while(strcmp(pch,"END"))
	{ 
      pch = strtok (NULL, " ,");

	  adc_val = atoi(pch);

	  FPGA.setAScanBuff(adc_val+20, VAL_MIRROR_X(adc_val+30));
		/* value = AScanSmoothedArr[i];
		 strncpy (token, AScanSmoothedArr+start, 3);
         _itoa(value, token, 10);

		strcat(AScanValArr, token);
		 strchr*/
	} 
CloseHandle(hFile);
}