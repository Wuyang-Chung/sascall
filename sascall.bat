#!/bin/tcsh

sntp -S tw.pool.ntp.org

setenv SYSDIR /nfs/freebsd.wyc/sys
#cd /nfs/freebsd.wyc/share/examples/kld/sascall
make
kldunload ./module/sascall.ko
kldload   ./module/sascall.ko

