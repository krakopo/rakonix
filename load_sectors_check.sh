#!/bin/sh

ceil()
{
  awk -v x=$1 'BEGIN { print int(x) + (x > int(x)) }'
}

docheck()
{
  marker=$1
  srcfile=$2
  binary=$3

  sectors_loading=$(grep $marker $srcfile | awk '{ print $3 }')
  size_bytes=$(ls -l $binary | awk '{ print $5 }')
  sectors_used=$(awk -v bytes=$size_bytes 'BEGIN { printf "%f", bytes / 512 }')
  sectors_used=$(ceil $sectors_used)
  if [ $sectors_used -ne $sectors_loading ]
  then
    echo "*******************************************************************"
    echo "Sector size check failed"
    echo
    echo " Source file: $srcfile"
    echo " Binary: $binary"
    echo " Marker: $marker"
    echo
    echo " Sectors to load: $sectors_loading"
    echo " Binary size (sectors): $sectors_used"
    echo
    echo "Change the number of sectors being loaded to match the kernel size."
    echo "*******************************************************************"

    failures=$(expr $failures + 1 )
  fi
}

failures=0
docheck BOOT1_SIZE_CHECK_MARKER boot/boot0.asm boot/boot1.bin
docheck KERNEL_SIZE_CHECK_MARKER boot/boot1.asm kernel/kernel.bin
test $failures -eq 0
