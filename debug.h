//debug.h

//#define APPLICATION

#define DBG_SHOW_FUNC_T(Target) DEBUGMSG(TRUE,( TEXT("%s: %s \r\n"),TEXT(Target), TEXT(__FUNCTION__)))
#define DBG_SHOW_DATE_T(Target) DEBUGMSG(TRUE,( TEXT("%s: builded: %s at: %s \r\n"), TEXT(Target), TEXT(__DATE__), TEXT(__TIME__) ))

#define DBG_SHOW_FUNC DEBUGMSG(TRUE,( TEXT("F_DLL: %s \r\n"), TEXT(__FUNCTION__)))
#define DBG_SHOW_DATE DEBUGMSG(TRUE,( TEXT("F_DLL: builded: %s at: %s \r\n"), TEXT(__DATE__), TEXT(__TIME__) ))


//#define DBG_SHOW_FUNC_T(Target) DEBUGMSG(TRUE,( TEXT("%s: in file %s funk %s at line %d \r\n"),TEXT(Target), TEXT(__FILE__) , TEXT(__FUNCTION__) , __LINE__ ))
//#define DBG_SHOW_DATE_T(Target) DEBUGMSG(TRUE,( TEXT("%s: builded: %s at: %s \r\n"), TEXT(Target), TEXT(__DATE__), TEXT(__TIME__) ))

//#define DBG_SHOW_FUNC DEBUGMSG(TRUE,( TEXT("F_DLL: in file %s funk %s at line %d \r\n"), TEXT(__FILE__) , TEXT(__FUNCTION__) , __LINE__ ))
//#define DBG_SHOW_DATE DEBUGMSG(TRUE,( TEXT("F_DLL: builded: %s at: %s \r\n"), TEXT(__DATE__), TEXT(__TIME__) ))
