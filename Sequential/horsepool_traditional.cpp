#include <iostream>
#include <vector>
#include <fstream>
#include<algorithm>
#include <string.h>

#include <stdlib.h>
#include <limits.h>

using namespace std;

int boyermoore_horspool(char* haystack, size_t hlen,
                           char* needle, size_t nlen)
{
    size_t scan = 0;
    size_t bad_char_skip[UCHAR_MAX + 1]; /* Officially called:
                                          * bad character shift */                 	                    
	
    cout << " " << hlen <<endl; 
    cout << " " << nlen <<endl;
  			
    /* Sanity checks on the parameters */
    if (nlen <= 0 || hlen == 0 || !needle)
        return NULL;
 
    /* ---- Preprocess ---- */
    for (scan = 0; scan <= UCHAR_MAX; scan = scan + 1)
        bad_char_skip[scan] = nlen;
 
    /* C arrays have the first byte at [0], therefore:
     * [nlen - 1] is the last byte of the array. */
    size_t last = nlen - 1;
 
    /* Then populate it with the analysis of the needle */
    for (scan = 0; scan < last; scan = scan + 1)
    {
             bad_char_skip[needle[scan]] = last - scan;
         //cout << "1. " << bad_char_skip[needle[scan]] << endl;           
    }
        
    for (int i = 0; i <= last; i++)
    {
      cout << "1. " << bad_char_skip[needle[i]] << endl;
    }
    
    size_t count = 0;    
    /* ---- Do the matching ---- */
    
    while (hlen >= nlen)
    {
	 //count++; 
    	 /* scan from the end of the needle */
        for (scan = last; haystack[scan] == needle[scan]; scan = scan - 1)
        {     
        if (scan == 0)
      		{	 /* If the first byte matches, we've found it. */
                //return haystack;
		  count++;
      		}
           //cout << "Count times needle is repeated : " << count << endl; 
        }
        hlen     -= bad_char_skip[haystack[last]];
        haystack += bad_char_skip[haystack[last]];
    }
    cout << endl << " Number of times, string matched with the text : " << count << endl;
    // return 0;
}


int main(int argc, char *argv[])
{
  const char *file = NULL;
  char *needle = NULL;
  int match; 
  char *str = NULL;	
  
  string haystack;	
  string line;
  char* ptr;
  
  file = argv[1];
  needle = argv[2];
  	
  ifstream in;
  in.open(file);
  	while (!in.eof()) 
	{
		getline(in, line);
		transform(line.begin(),line.end(),line.begin(), ::tolower);
 		///cout<<line<<endl;
 		haystack.append(line);
 	}
 	 
  // cout << " " << haystack << endl;
  ptr = &haystack[0];
  size_t haystack_length = strlen(ptr);
  //cout << " " << haystack_length <<endl;
  size_t needle_length = strlen(needle);
  //cout << " " << needle_length <<endl;
  
  match = boyermoore_horspool(ptr, haystack_length, needle, needle_length);
  if (match == 0)
	cout << " No match found and error in handling the text" <<endl;
  return 0;
  in.close();
}