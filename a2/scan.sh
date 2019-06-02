#*********************************************
#* Last Name:   Iremadze
#* First Name:  Luke
#* Student ID:  10163614
#* Course:      CPSC 457
#* Tutorial:    T02
#* Assignment:  2
#* Question:    Q1
#*
#* File name: scan.sh
#*********************************************
find -type f -name "*$1" -printf '%p %s\n' | sort -k 2 -n -r | head -$2 | awk '{ x += $2 ; print $1, $2} END {print "Total size: " x }'
