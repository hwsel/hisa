for NUM in 10
do
cd trustzone_isolation/non-secure
make clean
make USR_DEFINE="-DSIZE=$NUM"
cd ..
cp non-secure/normal.elf ./
cd secure
make clean
make
cd ..
cp secure/secure.elf ./
cd ..
cp trustzone_isolation/normal.elf ./
cp trustzone_isolation/secure.elf ./
xmd -tcl XMD_Commands.tcl
done
