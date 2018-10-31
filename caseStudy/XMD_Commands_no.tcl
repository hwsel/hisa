connect arm hw -debugdevice cpunr 2
after 2000
connect arm hw
after 2000
source hw_platform_0/ps7_init.tcl
after 2000
ps7_init
after 2000
fpga -f hw_platform_0/Zynq_Design_wrapper.bit
after 2000
ps7_post_config
after 2000
dow hls_motion.elf
after 2000
run
after 10000
stop
after 2000
dow secure.elf
after 2000
set check [xrmem 65 0x2f000]
after 2000
run
while {$check == {X 0 #$}} {set check [xrmem 65 0x2f000]}
rst
