del "*.bak" /f /s /a: H

::visual studio
del "*.user" /f /s /a: H
::del "*.htm" /f /s /a: H
del "*.obj" /f /s /a: H
del "*.pch" /f /s /a: H
del "*.pdb" /f /s /a: H
del "*.idb" /f /s /a: H
del "*.ncb" /f /s /a: H
del "*.user" /f /s /a: H
rmdir "FPGA_app\Pocket PC 2003 (ARMV4)" /q /s
rmdir "FPGA_dll\Pocket PC 2003 (ARMV4)" /q /s

::set path="C:\Program Files\WinRAR\";%path%
::rar a  -r %cd% *.*
::del "*.bak" 


::sleep 90
