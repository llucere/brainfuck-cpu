gcc ../src/main.c -o bfcpu -O2

#Clear-Host
Write-Output "START:"
Write-Output "-----------------------------------------------------"
bfcpu ../examples/badword.bfc -bytecodepass -time
Write-Output "-----------------------------------------------------"