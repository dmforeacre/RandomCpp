/* Daniel Foreacre
   CPS 162 Prof Lehman
   Nyhoff Chp 10 Lab
   10/23/20
   
   This program takes an input string and parses all capital letters
   from it, using both an interative and a recursive method.
*/

#include <iostream>
#include <string>
using namespace std;

// Method prototypes
void findCapsRec(string &, string &, int);
string findCapsNonRec(string &);

int main()
{
   string input;        // String to contain user input
   
   // Request input from user
   cout << "Please input a sentence to parse capitals from: ";
   getline(cin, input);
   
   // Display capital letters using iterative method
   cout << "Capital letters found non-recursively: " << findCapsNonRec(input) << endl;
   
   // Initialize string for capital letters to blank
   string recCaps = "";
   
   // Call recursive method to find capital letters
   findCapsRec(input, recCaps, 0);
   
   // Display recursive result
   cout << "Capital letters found recursively: " << recCaps << endl;

   return 0;
}

// Method to find capital letters in a string recursively
// Param &str Reference to the base string to check
// Param &caps Reference to the string used to collect capitals
// Param index Int used for current index in base string
void findCapsRec(string &str, string &caps, int index)
{
   // Checks if index is less than the end of the string
   // NOTE: base case is if this is false; does nothing
   if (index < str.size())
      // Checks if current index of base string is an ASCII character between 65 & 90 (capitals)
      if (65 <= str[index] && str[index] <= 90)
      {  
         // If it is, add that character to the collection string and recursively make
         // next call with the next index
         caps += str[index];
         findCapsRec(str, caps, index + 1);
      }
      // Otherwise character is not capital, so recursively make next call with next index
      else
         findCapsRec(str, caps, index + 1); 
}

// Method to find capital letters in a string interatively
// Param &str Reference to the base string to check
// Returns String containing the capital letters from the base string
string findCapsNonRec(string &str)
{
   // Initialize string for capital letters to blank
   string caps = "";
   
   // Step through each character of the base string
   for (int i = 0; i < str.size(); i++)
      // If character is capital (ASCII between 65 & 90) add that character to capitals string
      if (65 <= str[i] && str[i] <= 90)
         caps += str[i];
   
   // Return capitals string
   return caps;
}
