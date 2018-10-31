for i in $(seq 1 1 1)
do
cd no_TZ/secure
make clean
make
cd ..
cp secure/secure.elf ./
cd ..
cp no_TZ/secure.elf ./
xmd -tcl XMD_Commands_no.tcl
done
