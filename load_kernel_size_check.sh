#!/bin/sh

ceil()
{
  awk -v x=$1 'BEGIN { print int(x) + (x > int(x)) }'
}

sectors_loading=$(grep KERNEL_SIZE_CHECK boot/boot_kernel.asm  | awk '{ print $3 }')
kernel_size_bytes=$(ls -l kernel/kernel.bin | awk '{ print $5 }')
sectors_used=$(awk -v bytes=$kernel_size_bytes 'BEGIN { printf "%f", bytes / 512 }')
sectors_used=$(ceil $sectors_used)
if [ $sectors_used -ne $sectors_loading ]
then
  echo "*******************************************************************"
  echo "Kernel size check failed!"
  echo
  echo " Sectors loading: $sectors_loading"
  echo " Kernel size (sectors): $sectors_used"
  echo
  echo "Change the number of sectors being loaded to match the kernel size."
  echo "*******************************************************************"
  exit 1
fi
