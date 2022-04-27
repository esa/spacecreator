target extended-remote :1234

break notifyTestFinished
commands
dump srec memory /dev/stdout testData testData+testDataSize
end

run
monitor exit
