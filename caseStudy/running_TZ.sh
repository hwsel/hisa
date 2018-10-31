for i in $(seq 1 1 1)
do
cd TZ/non-secure
make clean
make
cd ..
cp non-secure/normal.elf ./
cd secure
make clean
make
cd ..
cp secure/secure.elf ./
cd ..
cp TZ/normal.elf ./
cp TZ/secure.elf ./
xmd -tcl XMD_Commands.tcl
done
