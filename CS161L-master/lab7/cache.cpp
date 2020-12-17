#include <iostream>
#include <vector>
#include <math.h> 
#include <cstdlib>
#include <fstream>

using namespace std; 

#define  BLOCK_SIZE     16
//#define  CACHE_SIZE     16384

void direct(int, char *);
void FIFO (int, char*,int);
void LRU (int, char*);

   int Cache[5] = {1024,2048,4096,8192,16384};
   int address[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

bool check(vector< vector<unsigned long long > >  &cache, int j, unsigned long long tag, int i, unsigned long long pos){
	
	int same = -1;
	unsigned long long hold = cache[pos][j];
	
	for(int m = 0; m < j; ++m)
	{
		if((cache[pos][j] & address[m]) && (cache[pos][m] == tag)){
			same = m;
			// cout << same << " ";
			break;
		}
	}

	if(same == -1)
	{
      cache[pos].pop_back();
      cache[pos].erase(cache[pos].begin());
      cache[pos].push_back(tag);
      cache[pos].push_back(hold);
      cache[pos][j] = cache[pos][j] >> 1 | address[j- 1];
	}
	else
	{
	   if(i ==0)
	   {
			cache[pos].pop_back();
			cache[pos].push_back(cache[pos][same]);
			cache[pos].erase(cache[pos].begin() + same);
			cache[pos].push_back(hold);
		}
	}
// 	cout << same<< endl;
   if(same == -1)
   {
      return 1;
      // break;
   }
	return 0;
}

int main (int argc, char *argv[]) {
   
   char *file;
   if(argc==2)
   {
      file = argv[1];  
   }
   else
   {
      exit(0);
   }

   for(int i = 0; i < 2 ; ++i)
   {
      
      if(i == 0)
         cout << "\n\n\t\tLRU Replacement Policy";
      else{
         cout << "\n\n\t\tFIFO Replacement Policy";
      }
      // cout << "\n\t1024\t2048\t4096\t8192\t16384";
      cout << "\n\t1024    2048    4096    8192    16384";
      for(int j = 1; j <= 8; j *=2)
      {
          cout << "\n" << j << "\t";
          if(j ==1)
          {
             direct(j,file);
          }
          else
          {
             FIFO(j,file,i);
            //  cout << "out" << endl;
          }
      }
          
   }
   cout << endl;
   return 0;
}


void FIFO(int j, char *file,int i)
{
   for(int each_cach = 0; each_cach< 5; ++each_cach)
   {
      int CACHE_SIZE = Cache[each_cach]; 
      int number_of_blocks = CACHE_SIZE / BLOCK_SIZE;
      int offset = int(log2(BLOCK_SIZE));
      int index_s = int(log2(number_of_blocks/j));
      std::vector < vector<unsigned long long> > cache (number_of_blocks/ j ,vector<unsigned long long > (1 + j,0));
      
      
      
      int filter = 1;
      for(int f = 1; f < index_s ; ++f)
      {
         filter = (filter << 1);
         filter =filter |1;
      }
      unsigned long long read_line;
      int total = 0, miss = 0;
      ifstream InFile(file);
      while (InFile >> hex >> read_line) {
         total++;
         unsigned long long tag   = read_line >> (index_s + offset); // get tag
         unsigned long long index = (read_line >> offset) & filter;        // get index
         index = index & filter ;
         // check for miss
         
         if(check(cache, j,tag, i, index))
         {
            miss++;
   
         }
      
      }
       double miss_rate = (double)miss / (double)(total);
            if((miss_rate * 100) < 10.00)
            printf(/*"miss rate:      */"%05.2lf\t", miss_rate * 100.0);
           else
            printf(/*"miss rate:      */"%0.2lf\t", miss_rate * 100.0);
      }
      
   return;
}
void direct(int j, char *file)
{
   for(int each_cach = 0; each_cach< 5; ++each_cach)
          {
             // build cache
            int CACHE_SIZE = Cache[each_cach]; 
            int number_of_blocks = CACHE_SIZE / BLOCK_SIZE;
            int offset = (log2(BLOCK_SIZE));
            int index_s = int(log2(number_of_blocks ));
            std::vector<unsigned long long> cache (number_of_blocks ,0);
            // vector< vector <unsigned long long > > cache(number_of_blocks,vector<unsigned long long> (j + 1,0)) ;
            vector<unsigned long long > temp (number_of_blocks,0);

            int filter = 1;
            for(int f = 1; f < index_s ; ++f)
            {
               filter = (filter << 1);
               filter =filter |1;
            }
            unsigned long long read_line;
            int total = 0, miss = 0;
            ifstream InFile(file);
            while (InFile >> hex >> read_line) {
               total++;
               unsigned long long tag   = read_line >> (index_s + offset); // get tag
               unsigned long long index = (read_line >> offset) & filter;        // get index
               index = index & filter ;

               if(cache[index] != tag || temp[index] == 0)
               {
                  cache[index] = tag;
                  temp[index] = 1;
                   miss++;
               }
            }
            double miss_rate = (double)miss / (double)(total);
            printf(/*"miss rate:      */"%0.2lf\t", miss_rate * 100.0);
         }
      
   return;
}
