for i in $(seq 1 1 1)
do
cd HWTrojan/secure
make clean
make
cd ..
cp secure/secure.elf ./
cd ..
cp HWTrojan/secure.elf ./
xmd -tcl XMD_Commands_with_Trojan.tcl
done
