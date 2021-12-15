set OSNAME=LuftOS
set BUILDDIR=%0/../bin
set OVMFDIR=%0/../../OVMFbin

qemu-system-x86_64 -drive file=%BUILDDIR%/%OSNAME%.img -m 2G -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="%OVMFDIR%/OVMF_CODE.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="%OVMFDIR%/OVMF_VARS.fd" -net none
pause
