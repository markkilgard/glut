
COM This batch file assumes that you have Visual C++ installed
COM on your system and that you have properly set up your
COM environment for command-line operation of the Visual C++
COM compile.  This is normally done by running the VCVARS32.BAT
COM batch file.

COM For Visual C++ 4.2, VCVARS32.BAT is usually installed in
COM c:\msdev\bin

COM For Visual C++ 5.0, VCVARS32.BAT is usually installed in
COM c:\Program Files\DevStudio\VC\bin

COM For Visual C++ 6.0, VCVARS32.BAT is usually installed in
COM c:\Program Files\Microsoft Visual Studio\VC98\bin

COM You can also set the appropriate environment variables
COM set n VCVARS32.BAT in your AUTOEXEC.BAT file to avoid 
COM running VCVARS32.BAT.

COM Windows 95 and 98 users may find it necessary to expand
COM the default environment memory space.  This can be done
COM by adding or updating the following lines to your CONFIG.SYS
COM file:
COM
COM [shell]
COM SHELL=c:\command.com /e:8192

nmake nodebug=1 -f Makefile.win %1 %2 %3 %4 %5 %6 %7 %8 %9
