@echo off
IF NOT EXIST A:\AppData\Local\Temp (
mkdir  A:\AppData\Local\Temp)

if errorlevel 1 goto VCReportError
goto VCEnd
:VCReportError
echo Project : error PRJ0019: A tool returned an error code from "Performing Pre-Build Event..."
exit 1
:VCEnd