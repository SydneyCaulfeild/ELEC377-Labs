Sydney Caulfeild / 20102648  
Emma Paczkowski / 20095427

**File name:** selfcomp.c   
**Objective:**  
This file is used to execute the exploit internally and is used to determine the lenth of the buffer.  
**Code description:**  
The main data structure in the code is a struct that defines a buffer that is followed by a function pointer. There are two array variables used in the file, exploit and exploit2.  exploit is used to find the length of the buffer so that we know how many nop instructin to add to exploit2 to have the same lenght. The main function is comprised of three sections. Firstly, it sets the functin pointer to point to a defult function given to us called foo(). Secondly, it coppies the bytes from the expoit or exploit2 variables (depending on which you are using wich is determined by step you are on) into the buffer in the struct. Lastly, it the function call using the function pointer.  If the length of the variables exploit/expoint2 is less than the lenght of the bufffer then the call will call the funciton foo(). 


**File name:** client.c   
**Objective:** 
Use exploit to attack the server and get the contents of the etc/passwd file.  
**Code description:**  
Firslt, this file is used to find the length of the buffer. 128 X characers were needed to crash the server. Secondly, it sends the exploit to the server. The compromise1 array contains a copy of the hex instruction form the exploit.nasm file. The length of this array is 134 = 128 (number of 'X' characters) + 2 (newline and null characters) + 4 ('WXYZ'). 31 nop instruction were used. 128 ('X' characters) - 97 (length of the code) = 31 nop instructions for padding. The file contains the attack function that is used to attack the server and uses the fprintf() call to print the contents of the etc/passwd file by overflowing into the function pointer.


**File name:** exploit.nasm   
**Objective:**  
Code to take advantage of a vulnerability in a server by adjusting the program counter to the code that we wish to execute. This program puts the buffer on the stack which puts the return address to the start of the exploit.  
**Code description:**  
100 'x' characters were used to achieve the length of the buffer and the code length is 97 decimal bytes, therefore 3 (100-97) nop instructions were used for padding. The addresses of flagStr, cmdStr, arrayAddr are calculated using the difference between esi and exeStr to make the code location independent. It then restores the array, setts up registers while ensure to remove any 00 or 0a(newline charecters) and then makes the system call. It also had a data segment that initializes all the variables.
