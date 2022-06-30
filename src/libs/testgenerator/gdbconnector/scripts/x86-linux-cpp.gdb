#target extended-remote host:1234
file /home/taste/SpaceCreator/TestProj3/generated/work/binaries/hostpartition
break notifyTestFinished
#commands
#end

run
dump srec memory /dev/stdout testData testData+testDataSize
#monitor exit
