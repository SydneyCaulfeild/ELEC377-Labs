#!/bin/sh
# Make sure there's only one argument.
if [ "$#" -ne "1" ]
then
    echo "You must provide one argument. Example usage: $0 directoryname."
    exit
fi

if [[ -d "$1" ]]
then
    echo "Mail Files:"	
    mainFound=0
    moduleFound=0
    # Search the directory for files ending in .c
    for file in `find "$1" -name '*.c'`; 
    do	    
    	# Find and count all occurrences of the main function in the file
	grep 'int main' $file -q
	if [ "$?" -eq "0" ]
	then
	    mainFound=1
	    # Find and count all occurences of printf and fprintf in the file. Print these
	    numPrintf=`grep -c "printf" $file`
	    numfprintf=`grep -c "fprintf" $file`
	    echo $file $numPrintf $numfprintf | awk '{ printf "%s: %d, %d", $1, $2, $3}'
	    echo 
	fi
    done
    # Indicate that no files were found with a main function if mainFound is zero
    if [ $mainFound = 0 ] 
    then
	echo "No mail files..."
    fi

    echo "Module files:"
    # Loop again  foor all files ending in .c
    for file in `find "$1" -name '*.c'`; 
    do	    
    	# Find all files in the directory with init_module
	grep 'init_module' $file -q
	if [ "$?" -eq "0" ]
	then
	    # Find all occurrences of  printk in the file. Print the line numbers. 
	    moduleFound=1
	    numPrintk=`grep -n "printk" $file | cut -d: -f1 | tr '\n' ','` 
	    numPrintk=${numPrintk%?}
	    echo $file $numPrintk | awk '{printf "%s: %s", $1, $2}'
	    echo 
	fi
    done
    # Indiciate that no files were found with init_module if moduleFound is zero
    if [ $moduleFound = 0 ] 
    then
	echo "No module files..."
    fi
else
    # Print that this directory name doesn't exist.
    echo "That is not a valid directory name."
fi
