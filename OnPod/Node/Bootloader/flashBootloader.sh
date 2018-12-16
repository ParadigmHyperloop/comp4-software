"C:\Program Files (x86)\GNU Tools Arm Embedded\7 2018-q2-update\bin\arm-none-eabi-gdb.exe" \
 -ex "target extended-remote COM5" \
 -ex "monitor swdp_scan" \
 -ex "att 1" \
 -ex "file samd21_sam_ba.hex"\
 -ex "load"\
