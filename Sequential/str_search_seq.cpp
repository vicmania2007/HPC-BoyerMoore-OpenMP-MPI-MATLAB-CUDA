
#include <iostream>
#include <vector>
#include <stdio.h>
#include<climits>
#include <cstdlib>
#include <algorithm>
#include<fstream>
#include<string.h>
using namespace std;

typedef vector<size_t> occtable_type;

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
const char* str;
const char* filename;
string file_str , temp;
char* text ; 
unsigned char occ_char;	
int count =0 ;

filename = argv[1];
str = argv[2];

cout<<"Finding string '"<<str<<"' in text file "<<filename<<endl;

 ifstream in;
 in.open(filename);
 while (!in.eof())
 {
 	getline(in,temp);
 	transform(temp.begin(),temp.end(),temp.begin(), ::tolower);
 	file_str.append(temp);
 }
 in.close();
 
text = &file_str[0];

size_t str_len = strlen(str);
occ1 = create_table(reinterpret_cast <const unsigned char*> (str),str_len);

cout <<"The occurance table Generated is : "<<endl;
for(int i = 0; i<str_len;i++)
	cout<<str[i]<<"		"<<occ1[str[i]]<<endl;

size_t text_pos =0;
size_t text_len = strlen(text);
cout<<"Length of text to be scanned is "<<text_len<<endl;


while(text_pos <= (text_len - str_len ))	
{
		occ_char = text[text_pos + str_len -1];
		
		if (occ_char == str[str_len-1] && (memcmp(str, text+text_pos, str_len - 1) == 0))
		{
		  count++;
		 // cout<<"Word at postion "<<text_pos<<" of text matches the string "<<endl;
		}
		
		text_pos += occ1[occ_char];
}


cout<<"Total number of occurances of string in text = "<<count<<endl;

return 0;

}