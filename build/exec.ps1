gcc ../src/main.c -o main -O2

#Clear-Host
Write-Output "START:"
Write-Output "-----------------------------------------------------"
./main.exe ../tests/wait1sec.bf -bcp -t
Write-Output "-----------------------------------------------------"