del "*.bak" /f /s /a: H

::visual studio
cd "C:\Users\user\Dropbox\Work\FPGA_dll_out"
del "*.exp" /f /s /a: H
del "*.lib" /f /s /a: H
del "*.pdb" /f /s /a: H


::set path="C:\Program Files\WinRAR\";%path%
::rar a  -r %cd% *.*
::del "*.bak" 


::sleep 90
