//debug.h
#define DBG_SHOW_FUNC DEBUGMSG(TRUE,( TEXT("F_DLL: in file %s funk %s at line %d \r\n"), TEXT(__FILE__) , TEXT(__FUNCTION__) , __LINE__ ))
#define DBG_SHOW_DATE DEBUGMSG(TRUE,( TEXT("F_DLL: builded: %s at: %s \r\n"), TEXT(__DATE__), TEXT(__TIME__) ))
