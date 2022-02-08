// Assignment 3: Cuckoo Hashing algorithm
// Doina Bein
// An open addressing method called Cuckoo Hashing
// INPUT: an input file containing strings of maximum 255 characters, 
// one string per line
// OUTPUT: a detailed list of where the strings are inserted.     

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

// cuckoo tables' size                                                        
const int tablesize = 17;
// combine the two 1-dimensional table into one 2-dimensional table           
string  t[tablesize][2];

// define the prime number used in calculating the hash values
const int prime = 31;

// compute the hash functions
size_t f(string, size_t);

// place a string in one of the hash tables
bool place_in_hash_tables (string);

int main() {

  // the strings to be stored in the hash tables
  string s;
  size_t i, len;
  bool placed;

  // clear the tables
  for(i=0; i< tablesize; i++) {
      t[i][0] = "";
      t[i][1] = "";
  }

  char filename[255] = "";

   // display the header
  cout << endl << "CPSC 335.01 - Programming Assignment #3: ";
  cout << "Cuckoo Hashing algorithm" << endl;
    
  // read the strings from a file
  cout << "Input the file name (no spaces)!" << endl;
  cin >> filename;

  // open the file for reading
  ifstream infile(filename);
  /* read line by line from the file */
  while ( getline(infile,s) ) {
    // place null character at the end of the line instead of <return>
    len = s.size();
    s.push_back('\0'); // s[len-1]='\0' was deleting the last character of the string
    // insert the string in the cuckoo table
    placed = place_in_hash_tables(s);
    // check whether the placement was successful
    if (!placed) {
      cout << "Placement has failed" << endl;
      return -1;
    }
  }
  infile.close();
  return 0;
}


bool place_in_hash_tables (string s) {
  
  bool placed;
  size_t pos;
  int index;
  string temp_s, temp;
  
  temp_s = s;

  // use a counter to detect loops; if counter >= 2*tablesize, then loop
  int counter = 0;
  
  // start with table T1
  index = 0;
  
  placed = false;

  pos = f(temp_s, index);

  while((!placed ) && (counter < 2*tablesize)) {
    if (t[pos][index] == "") {
      cout << "String <" << temp_s << "> will be placed at";
      cout  << " t[" << pos <<"][" << index << "]" << endl;
      t[pos][index] = temp_s;
      placed = true;
      return placed;
    }
    else {
      // entry at index <pos> in the <index> hash table is not available 
      // so obtain the string stored over there in variable <temp> and place 
      // the string <temp_s> there
      // now the string <temp> needs to be placed in the other table
      cout << "String <" << temp_s << "> will be placed at" << " t[" << pos;
      cout <<"][" << index << "]" << " replacing <" << t[pos][index] << ">";
      cout << endl;

      temp = t[pos][index];   // string to be evicted
      t[pos][index] = temp_s;   
      
      index = index? 0:1;   // alternate between tables
      pos = f(temp, index);   // calculate hash for evicted string
      temp_s = temp;        // set temp_s to evicted string

      counter ++;

    }
  }
  return placed;
};


// compute the hash functions
size_t f(string s, size_t index) {
  size_t po, val=0, len;
  int i, temp;
  po = 1;

  len = s.size();

  if (index == 0) {
    val = s[0];
    val = val % tablesize;
    if (val < 0) val += tablesize;

    if (len == 1) 
      return val;
    
    for (i = 1; i < len; i++) 
    {
      temp = s[i];
      po *= prime;

      po = po % tablesize;
      if (po < 0) po += tablesize;
      
      val += temp * po;
      val = val % tablesize;

      if (val < 0) val += tablesize;
    }    
    return val;
}
  else {    // using horner's polynomial
    // x_0 * a^0 + x_1 * a^1 + ... + x_k-2 * a^(k-2) + x_k-1 * a^(k-1) reduces to
    // (x_0  + a(x_1 + a(x_2 + a(x_3 + a(... + x_k-3_ + a(x_k-2_ + a(x_k-1_)))))))
    // we start from the innermost loop with x_k-1_ as the first letter and x_0 as the last letter of the string
    val = 0;
    for (i = 0; i < len; i++)   // x_0 will be s[len-1] since the hash function is sum_{i=0}^{len-1}(s[len - i - 1]*a^i)
    {
      val = (val * prime + s[i]) % tablesize;   //  v = (s[i] + a * v)  mod tablesize

      if (val < 0) val += tablesize;    // correction for unintentional negative values
    }
    return val;
}
return val;
}
