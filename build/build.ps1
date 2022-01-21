gcc ../src/main.c -o main -O2

#Clear-Host
Write-Output "START:"
Write-Output "-----------------------------------------------------"
./main.exe ../tests/multi_string_input.bf -bytecodepass -time
Write-Output "-----------------------------------------------------"