/*
   Daniel Foreacre
   CPS 162 Prof Lehman
   Chapter 13 Lab
   11/14/20
   
   This program loads a list of numbers from a text file, then creates a binary tree with that data.
   The heapify method then converts that tree to a heap, and the heapsort then sorts the data
   in ascending order.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

class IntHeap
{
   private:
      int size;               // Int holding size of heap
      vector<int> vect;       // Pointer to vector holding heap data
      
      // Method to swap two integers
      // Param &first Reference to an int
      // Param &second Reference to an int
      void swap(int &first, int &second)
      {
         int temp = first;
         first = second;
         second = temp;
      }
      
      // Method to percolate values down the tree
      // Param loc Int for index at beginning of semiheap
      // Param end Int for the index of the end element
      void percolate_down(int loc, int end)
      {
         // Location of left child
         int child = 2 * loc;
         
         // Traverse down the subtree, starting at loc       
         while(loc <= end)
         {
            // If loc has 2 children, and the right child is larger than the left,
            // make child point to the right child
            if(child < end && vect[child] < vect[child + 1])
               child++;
               
            // If the child is larger than the parent, swap them
            if(child < end && vect[loc] < vect[child])
            {
               swap(vect[loc], vect[child]);
               // Now check the child subtree to keep the heap order
               loc = child;
               child = 2 * child;
            }
            else
               break;
         }
      }
      
   public:
      // Default constructor sets size to 0, clears vector and reserves index 0
      IntHeap()
      {
         size = 0;
         vect.clear();
         vect.push_back(0);
      }
      
      // Constructor that takes in a reference to a vector as an argument
      IntHeap(vector<int> &v)
      {
         // Size of heap has to be 1 less because the 0 index is not counted
         size = v.size() - 1;
         vect = v;
      }
      
      // Method to get heap size
      // Returns Size as an int
      int getSize()
      {
         return size;
      }
      
      // Method to check if heap is empty
      // Returns True if size is zero
      bool isEmpty()
      {
         bool empty = false;
         if (size == 0)
            empty = true;
         return empty;
      }
      
      // Method to insert an item onto the heap
      // Param item The int to insert into the heap
      void insert(int item)
      {
         // Increment size of heap
         size++;
         
         // Add item to end of vector
         vect.push_back(item);
         
         // Set location of current item and its parent
         int loc = size,
             parent = loc / 2;
         
         // Percolate up while not at root (parent = 1)
         // and child is greater than parent
         while(parent >= 1 && vect[loc] > vect[parent])
         {
            // Swap the values of the 2 nodes
            swap(vect[loc], vect[parent]);
            // Move up to parent node
            loc = parent;
            parent = loc / 2;
         }
      }
      
      // Method to 'heapify' the binary tree to make sure it follows heap order
      void heapify()
      {
         // Start at 2nd row of nodes and work up the tree,
         // calling percolate_down for each node
         for(int i = size / 2; i >= 1; i--)
            percolate_down(i, size);
      }
      
      // Method to sort the heap after it has been heapified
      void heapSort()
      {
         // Start at greatest (last) element of heap, swap it with the root
         // and percolate it down to correct position
         for(int i = size; i > 1; i--)
         {
            swap(vect[1], vect[i]);
            percolate_down(1, i);
         }
         
         // Check if first element is in correct place
         if(vect[1] > vect[2])
            swap(vect[1], vect[2]);
      }
      
      // Method to display contents of the heap as a list
      void displayList()
      {
         cout << endl;
         for(int i = 1; i <= size; i++)
            cout << vect[i] << " ";
      }
      
      // Method to display contents of the heap as a binary tree
      void displayTree()
      {
         // Index for current line display
         int index;
         
         // Calculate height of tree
         int height = (int)log2(size) + 1;
         // Calculate size of the fully filled tree
         int maxSize = pow(2, height) - 1;
         
         // Start display on new line
         cout << endl;
         
         // Prints separate line for each node of the completely filled tree
         for (int i = 1; i <= maxSize; i++)
         {
            // Determine index of current line based on its position in the tree
            if(i % 2 == 1)
               index = maxSize - (i / 2);
            else if(i % 4 == 2)
               index = (maxSize / 2) - (i / 4);
            else if(i % 8 == 4)
               index = (maxSize / 4) - (i / 8);
            else if(i % 16 == 8)
               index = (maxSize / 8) - (i / 16);
            else if(i % 32 == 16)
               index = (maxSize / 16) - (i / 32);
            else if(i % 64 == 32)
               index = (maxSize / 32) - (i / 64);
            
            // Find height of current node to print indent
            height = (int)log2(index) + 1;
            for(int i = 1; i < height; i++)
               cout << "        ";  
               
            // If the current index is greater than the actual size
            // (ie there are empty nodes) then print an X
            if(index > size)
               cout << setw(4) << "X" << endl;
            // Otherwise print the contents of index
            else
               cout << setw(4) << vect[index] << endl;
         }
      }
};

// Method prototypes
void loadFile(fstream &, vector<int> &);
void createHeap(IntHeap *, vector<int> &);
void doHeapify(IntHeap *);
void doSort(IntHeap *);
void display(IntHeap *);
void anyKey();

// Constants
const string FILE_NAME = "heaplist.txt";  // Name of data file

int main()
{
   
   fstream ifile;                       // Fstream object for file
   char option;                         // Char for menu input
   vector<int> list;                    // Vector of ints for file input
   IntHeap *heap = new IntHeap();       // IntHeap object for heap
   
   // Fill first index with placeholder
   list.push_back(0);
   
   // Main menu loop
   do
   {
      // Display options for menu
      cout << "\n\nMain Menu"
           << "\n1) Read data from file"
           << "\n2) Create heap from data"
           << "\n3) Heapify heap"
           << "\n4) Sort heap"
           << "\n5) Display heap"
           << "\n0) Exit"
           << "\n\nPlease input your choice: ";
      // Get user input
      cin.get(option);
      cin.ignore(20, '\n');
      
      // Switch statement for menu
      switch(option)
      {
         case '1':  loadFile(ifile, list);
                    break;
         case '2':
                    break;
         case '3':
                    break;
         case '4':
                    break;
         case '5':  display(heap);
                    break;
         case '0':  break;
         default:   cout << "\n\nPlease input a valid option.";         
      };
   }while(option != '0');
   
   return 0;
}

void loadFile(fstream &ifile, vector<int> &l)
{
   string input;    // String for file input
   
   // Open file
   ifile.open(FILE_NAME);
   
   // Check if file opened
   if(!ifile)
      cout << "\n\nUnable to open file. Check that it exists in current directory.";
   else
   {
      cout << "\n\nFile successfully opened.";
      // Read data from file and add to list
      while(ifile >> input)
         l.push_back(stoi(input));
      
      cout << "\n\nData read from file.";
      
      // Display completed list
      cout << "\n\nList contains the following data:\n";
      for(int i = 1; i < l.size(); i++)
         cout << l[i] << " ";
   }  
   
   ifile.close();
   
   anyKey();
}
void createHeap(IntHeap *h, vector<int> &l)
{
   // Create IntHeap with list as argument
   h = new IntHeap(l);
   
   anyKey();
}

void doHeapify(IntHeap *h)
{
   // Heapify contents
   h->heapify();
   
   anyKey();
}

void doSort(IntHeap *h)
{ 
   // Sort heap
   h->heapSort();
   
   anyKey();
}

void display(IntHeap *h)
{
   if(h->isEmpty())
      cout << "Heap is empty please fill first.";
   else
   {
      // Display contents as binary tree      
      cout << "\nHeap as binary tree:\n";
      h->displayTree();
      
      // Display contents as list
      cout << "\nHeap as contiguous list:\n";
      h->displayList();
   }
   
   anyKey();
}

void anyKey()
{
   cout << "\n\nPress any key to continue.";
   cin.get();
   cin.ignore(20, '\n');
}
