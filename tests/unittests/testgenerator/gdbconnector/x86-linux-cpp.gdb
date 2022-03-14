target extended-remote :1234

break notifyTestFinished
commands
print *testData@kTestDataSize
end

run
monitor exit

