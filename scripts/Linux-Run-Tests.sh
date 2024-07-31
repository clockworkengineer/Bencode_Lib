 #!/bin/
 cd ./Release/tests/
 ./Bencode_Lib_Unit_Tests
if [ $? -eq 0 ]; then
  cd ../../
else
  exit $?
fi
 cd ./Debug/tests/
 ./Bencode_Lib_Unit_Tests
if [ $? -eq 0 ]; then
  cd ../../
else
  exit $?
fi
