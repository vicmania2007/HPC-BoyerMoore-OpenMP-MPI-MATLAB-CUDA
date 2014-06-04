
#include <iostream>
#include <vector>
#include <stdio.h>
#include <climits>
#include <cstdlib>
#include <cmath>
#include <algorithm> // for the transform function
#include <fstream>  // for inputting the text file
#include <string.h> // for string functions
#include <mpi.h>
#include <omp.h>
using namespace std;

#define NUMTHREADS 2

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


int main(int argc,char *argv[])
{
occtable_type occ1;
MPI_File file;
const char* str;
char* filename;

char* text ; 
unsigned char occ_char;	
size_t text_pos =0;
int text_len,ierr;
int temp_count =0,rank, size, type=99 ,final_count =0;
char *block;
char ***lines;
int *nlines;
int overlap;

/****OpenMP variable declaration*****/ 
int blocksize_mp;
int remainder;
int offset_mp;	
int temp,tid ;
int thresh;

MPI_Offset filesize;
    MPI_Offset blocksize;
    MPI_Offset start;
    MPI_Offset end;

 filename = argv[1];
 str = argv[2];
 size_t str_len = strlen(str);

 // OFfset for openmp blocks
 offset_mp = str_len;


// We add an overlap to ensure that splitting the string does not cause incorrect results. 
 overlap = atoi(argv[3]);
 
 	//Calling function to generate the occurrence table . We have to reinterpret_cast in
	// order to match the arguments of func. call and func. declaration.
	occ1 = create_table(reinterpret_cast <const unsigned char*> (str),str_len);

//cout<<"Finding string '"<<str<<"' in text file "<<filename<<endl;

	MPI_Status status;	
	
	//Initialize MPI environment.
	MPI_Init(&argc, &argv);
	
	//Getting number of processes.
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	//Getting rank of process.
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	ierr = MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &file);
    // Handling scenario where we are unable to open file
    if (ierr) {
        if (rank == 0) 
        	cout<<"MPI_File_open Failed !"<<endl;
        MPI_Finalize();
        exit(2);
    }
    
    // Calculating length of text and determining how much text should be given to each
    // node.
    
    MPI_File_get_size(file, &filesize);
    blocksize = filesize/size;

    // Creating pointers to start and end of block
    start = rank * blocksize;
    end   = start + blocksize - 1;
        
    /* Allocating memory for block*/
    block = (char*)malloc((blocksize + 1)*sizeof(char));
    
    //Reading data of file into blocks
    MPI_File_read_at_all(file, start, block, blocksize, MPI_CHAR, MPI_STATUS_IGNORE);
   
    block[blocksize] = '\0';	

      if(rank == 0){  
      cout<<"String '"<<str<<"' is being searched in file '"<<filename<<"'"<<endl;
    }
    
    cout<<"Length of file "<<filesize<<" is divided into blocksize "<<blocksize<<" for node "<<rank<<endl;
  
	// Making pointer point to start of block
	text =&block[0];

/* Creating blocks for OpenMP from blocksize */
   blocksize_mp = blocksize/NUMTHREADS; 
   remainder = blocksize % NUMTHREADS;	
   thresh = blocksize_mp + offset_mp;	

   cout <<"Length of blocksize for openmp threads to process"<<blocksize_mp<<endl;
   cout <<"Thresh length is"<<thresh<<endl;

	
#pragma omp parallel num_threads(NUMTHREADS) private(temp,text_pos,tid) shared(remainder,blocksize_mp,temp_count,occ1,str_len,text,text_len,occ_char,thresh) 
  {
  	temp = 0;
  	text_pos = 0;
  	tid = omp_get_thread_num();

		if (tid == 0){
      			text_pos = 0;

			cout << " text_pos " << text_pos << endl;
			cout << " text length " << blocksize_mp << endl;	
	
			while((text_pos < thresh + remainder) && (((tid*blocksize_mp)+ text_pos) < blocksize + offset_mp))
			{
			
			//cout <<" 1. entered here "<< endl;

			// Selecting charachter at position equal to pattern length . -1 is to nullify
			// the 0th element.
			occ_char = text[(tid*blocksize_mp)+ text_pos + str_len-1];
		
			// If last charachter of pattern matches curent character in text and 
			// if charachters ahead of current in text contains pattern  , we have a match.
			// memcmp compares charachters ahead of current with the pattern in blocks of
			// str_len -1.
		 
			if (occ_char == str[str_len-1] && (memcmp(str,text+(tid*blocksize_mp)+text_pos,str_len-1) == 0))
			{
		  		temp++;
			}	

			text_pos += occ1[occ_char];
		     }
		}

	else {
	text_pos = 0;
	while((text_pos < thresh) && (((tid*blocksize_mp)+ text_pos) < blocksize + offset_mp))
		{			

			// Selecting charachter at position equal to pattern length . -1 is to nullify
			// the 0th element.
			occ_char = text[(tid*blocksize_mp)+ text_pos + str_len-1];
		
			// If last charachter of pattern matches curent character in text and 
			// if charachters ahead of current in text contains pattern  , we have a match.
			// memcmp compares charachters ahead of current with the pattern in blocks of
			// str_len -1.
		 
			if (occ_char == str[str_len-1] && (memcmp(str, text+(tid*blocksize_mp)+text_pos, str_len - 1) == 0))
			{
		  		temp++;
			}	
		
		// Look at occurrence table and decide how to increment text pointer
		     #pragma omp atomic
			text_pos += occ1[occ_char];
		}

	}			
	    #pragma omp atomic
		temp_count += temp;
		//cout << temp << endl; 	
 }

// Adding the tempsum of each thread to the total sum using mpi_reduce

MPI_Reduce(&temp_count,&final_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

if(rank == 0){
	cout<<"Total number of occurances of string in text = "<<final_count<<endl;
}

MPI_File_close(&file);

MPI_Finalize();

return 0;

}




