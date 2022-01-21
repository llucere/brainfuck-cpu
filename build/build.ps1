gcc ../src/main.c -o bfcpu -O2

#Clear-Host
Write-Output "START:"
Write-Output "-----------------------------------------------------"
./bfcpu.exe ../tests/multi_string_input.bf -bytecodepass -time
Write-Output "-----------------------------------------------------"