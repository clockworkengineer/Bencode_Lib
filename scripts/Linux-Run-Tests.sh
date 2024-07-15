 #!/bin/
 cd build/tests/
 ./Bencode_Lib_Unit_Tests
if [ $? -eq 0 ]; then
  cd ../../
else
  exit $?
fi
