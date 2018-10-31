connect arm hw
after 2000
source design_1_wrapper_hw_platform_0/ps7_init.tcl
after 2000
ps7_init
after 2000
fpga -f design_1_wrapper_hw_platform_0/design_1_wrapper.bit
after 2000
ps7_post_config
after 2000
dow fsbl.elf
after 2000
con
after 2000
stop
after 2000
dow normal.elf
after 2000
dow secure.elf
after 2000
run
after 20000
rst
