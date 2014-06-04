#include <iostream>
#include <vector>
#include <fstream>
#include<algorithm>
#include <string.h>
#include <math.h>

#include <stdlib.h>
#include <limits.h>
#include <omp.h>

using namespace std;

#define NUMTHREADS 32

typedef vector<size_t> occtable_type; // table type for occurrence table

const occtable_type create_table(const unsigned char* str , size_t str_length )
{
	occtable_type occ(UCHAR_MAX+1,str_length);

	if(str_length >= 1)
	{
		for( size_t i=0; i<(str_length-1); ++i)
			occ[str[i]] = (str_length -1)-i;
	}
	
	return occ;
}


int boyermoore_horspool(char* text, size_t text_len,const char* str, size_t str_len , occtable_type occ1)
{
	size_t text_pos =0;
	unsigned char occ_char;	
	int offset = str_len - 1;
	int blocksize = (text_len/NUMTHREADS);
	int remainder = (text_len % NUMTHREADS);
	
	int count =0 , temp,tid ;
	int thresh = blocksize + offset;
	cout<<blocksize;
	
#pragma omp parallel num_threads(NUMTHREADS) private(temp,text_pos,tid) shared(remainder,blocksize,count,occ1,str_len,text,text_len,occ_char,thresh) 
  {
  	temp = 0;
  	text_pos = 0;
  	tid = omp_get_thread_num();
  	

		if (tid == 0){

			text_pos = 0;
			while((text_pos < thresh + remainder) && (((tid*blocksize)+ text_pos) < text_len))
			{
			
			// Selecting charachter at position equal to pattern length . -1 is to nullify
			// the 0th element.
			occ_char = text[(tid*blocksize)+ text_pos + str_len-1];
		
			// If last charachter of pattern matches curent character in text and 
			// if charachters ahead of current in text contains pattern  , we have a match.
			// memcmp compares charachters ahead of current with the pattern in blocks of
			// str_len -1.
		 
			if (occ_char == str[str_len-1] && (memcmp(str,text+(tid*blocksize)+text_pos,str_len-1) == 0))
			{
		  		temp++;
			}	

			text_pos += occ1[occ_char];
		     }
		}

	else {
	text_pos = 0;
	while((text_pos < thresh) && (((tid*blocksize)+ text_pos) < text_len))
		{			
			// Selecting charachter at position equal to pattern length . -1 is to nullify
			// the 0th element.
			occ_char = text[(tid*blocksize)+ text_pos + str_len-1];
		
			// If last charachter of pattern matches curent character in text and 
			// if charachters ahead of current in text contains pattern  , we have a match.
			// memcmp compares charachters ahead of current with the pattern in blocks of
			// str_len -1.
		 
			if (occ_char == str[str_len-1] && (memcmp(str, text+(tid*blocksize)+text_pos, str_len - 1) == 0))
			{
		  		temp++;
			}	
		
		// Look at occurrence table and decide how to increment text pointer
		 #pragma omp atomic
			text_pos += occ1[occ_char];
			
		}

	}			
	    #pragma omp atomic
		count += temp;		
  }
     return count;
}


int main(int argc, char *argv[])
{
int match;
occtable_type occ1;
const char* str;
const char* filename;
string file_str , temp_str;
char* text ; 
unsigned char occ_char;	
size_t text_pos =0;


filename = argv[1];
str = argv[2];

cout<<"Finding string '"<<str<<"' in text file "<<filename<<endl;

 ifstream in;
 in.open(filename);
 while (!in.eof())
 {
    //getting one line in the file 
 	getline(in,temp_str);
 	
 	//Converting the line into lower case to avoid case sensitive search
 	transform(temp_str.begin(),temp_str.end(),temp_str.begin(), ::tolower);
 	
 	//Appending line to final string of txt file
 	file_str.append(temp_str);
 }
 in.close();

// Making char pointer point to first element in string. 
text = &file_str[0];

size_t str_len = strlen(str);
size_t text_len = strlen(text);
cout<<"Length of text to be scanned is "<<text_len<<" and length of pattern is "<<str_len<<endl;

//Calling function to generate the occurrence table . We have to reinterpret_cast in
// order to match the arguments of func. call and func. declaration.
occ1 = create_table(reinterpret_cast <const unsigned char*> (str),str_len);

cout << "No of threads is "<<NUMTHREADS<<endl;	

 match = boyermoore_horspool(text,text_len,str,str_len ,occ1);

  if (match == 0)
	  cout<< " No match found and error in handling the text" <<endl;
   else
      cout<<"Total number of occurances of string in text = "<<match<<endl;



    in.close();
    
  return 0;

}
