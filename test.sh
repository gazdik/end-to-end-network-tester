#!/bin/bash
# Name: test.sh
# Autor: Peter Gazdik, xgazdi03@stud.fit.vutbr.cz
# Date: 03.05.2015

# Colors
red='\033[1;31m'
green='\033[1;32m'
NC='\033[0m'

# Remove all output files
rm -f ipkperf-*

# Run server on port 4242
./ipkperfserver -p 4242 &

###################################################################
echo -e "\n${green}TEST 1: Spustenie s vychodzimi hodnotami a ukoncenie signalom SIGINT po 5 sekundach${NC}\n"
echo -e "./ipkperfclient -p 4242 localhost &\n"

./ipkperfclient -p 4242 localhost &
sleep 5
pkill -2 ipkperfclient 
sleep 2
cat ipkperf-127.0.0.1-100-10

###################################################################
echo -e "\n${green}TEST 2: Minimalna mozna velkost payload paketu	${NC}\n"
echo -e "./ipkperfclient -p 4242 -s 10 -t 2 -i 1 localhost\n"

./ipkperfclient -p 4242 -s 10 -t 2 -i 1 localhost
cat ipkperf-127.0.0.1-10-10

###################################################################

echo -e "\n${green}TEST 3: Maximalna mozna velkost payload paketu ${NC}\n"
echo -e "./ipkperfclient -p 4242 -s 65507 -t 5 -i 2 localhost\n"

./ipkperfclient -p 4242 -s 65507 -t 5 -i 2 localhost
cat ipkperf-127.0.0.1-65507-10

###################################################################

echo -e "\n${green}TEST 4: Bezna cinnost - Interval 1s, Timeout 10s, Size 100 bytov, Rate 100 packet/s${NC}\n"
echo -e "./ipkperfclient -p 4242 -s 100 -t 10 -i 1 -r 100 localhost\n"

./ipkperfclient -p 4242 -s 100 -t 10 -i 1 -r 100 localhost
cat ipkperf-127.0.0.1-100-100

###################################################################

echo -e "\n${green}TEST 5: Pridavanie do suboru na koniec - Interval 2s, Timeout 3s, Size 100 bytov, Rate 100 packet/s${NC}\n"
echo -e "./ipkperfclient -p 4242 -s 100 -t 3 -i 2 -r 100 localhost\n"

./ipkperfclient -p 4242 -s 100 -t 3 -i 2 -r 100 localhost
cat ipkperf-127.0.0.1-100-100


##################################################################
echo -e "\n${green}TEST 6: Ukoncenie serveru signalom SIGINT${NC}\n"

pkill -2 ipkperfserver
