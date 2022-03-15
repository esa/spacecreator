target extended-remote :1234

break notifyTestFinished
commands
dump srec memory /dev/stdout testData testData+kTestDataSize
end

run
monitor exit
