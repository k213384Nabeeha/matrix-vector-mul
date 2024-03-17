@echo off
setlocal enabledelayedexpansion

REM Array of input sizes
set "sizes=64 128 256 512 1024 2048 4096 8192 16384 32768"
REM Number of iterations for each test
set "iterations=10"

REM Create or clear the CSV file
echo Iteration No,Size,Time Taken (seconds) > benchmark_results.csv

echo Benchmarking started...

REM Loop through each input size
for %%s in (%sizes%) do (
    REM Run the program for each input size
    echo Benchmarking for input size %%s
    set total_time=0
    for /l %%i in (1,1,%iterations%) do (
        REM Record start time for the iteration
        set start=!time!

        echo Testing input size %%s, iteration %%i...

        REM Execute the program
        .\Project1.exe %%s %%s

        REM Record end time for the iteration
        set end=!time!

        REM Extract hours, minutes, and seconds
        set "start=!start: =0!"
        set "end=!end: =0!"
        set "start_hour=!start:~0,2!"
        set "start_min=!start:~3,2!"
        set "start_sec=!start:~6,2!"
        set "end_hour=!end:~0,2!"
        set "end_min=!end:~3,2!"
        set "end_sec=!end:~6,2!"

        REM Calculate time in seconds
        set /a "time_seconds=(1!end_hour! * 3600 + 1!end_min! * 60 + 1!end_sec!) - (1!start_hour! * 3600 + 1!start_min! * 60 + 1!start_sec!)"

        REM Add the time taken to the total time
        set /a "total_time+=time_seconds"

        REM Output the time difference for the iteration
        echo   Iteration %%i: !time_seconds! seconds

        REM Append data to CSV file
        echo %%i,%%s,!time_seconds! >> benchmark_results.csv
    )

    REM Calculate the average time for 10 iterations with floating point arithmetic
    set /a "average_time=total_time / iterations"
    set /a "average_time_decimal=total_time %% iterations * 100 / iterations"

    REM Output the average time
    echo Average time for input size %%s: !average_time!.!average_time_decimal! seconds
    echo.

    REM Append the average time to CSV file
    echo Average,%%s,!average_time!.!average_time_decimal! >> benchmark_results.csv
)

echo Benchmarking completed!
