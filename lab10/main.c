#include <stdio.h>
#include <stdlib.h>

#define MEMORY_CAPACITY_WORDS 64
int memory[MEMORY_CAPACITY_WORDS];//the memory we can allocate
int* end;//a pointer to the last element of memory

int mymalloc_calls=0;

void mymalloc_init() {
    memory[0]=MEMORY_CAPACITY_WORDS*4;
    int *end = &memory[MEMORY_CAPACITY_WORDS-1];
    //initialize memory and end here
}

void print_memory() {
    //uncomment this to print contents of memory
    for (int i=0; i<MEMORY_CAPACITY_WORDS; i++) {
        printf("memory[%d]=%d\n",i,memory[i]);
    }
}

void *mymalloc(int bytes_requested) {
    mymalloc_calls++;
    printf("mymalloc_call %d, bytes=%d\n",mymalloc_calls,bytes_requested);
    if(bytes_requested%4 != 0){
	bytes_requested += 4 - bytes_requested%4;
    }
    bytes_requested += 4;
    int i = 0;
    while(memory[i]%2==1 || memory[i] < bytes_requested){
        if(memory[i]%2==1){ 
		i+=(memory[i]-1)/4;
	}else{
		i+=(memory[i])/4;
	}
//	 printf("memory = %d\n",memory[i]);
    }
    int initS = memory[i];
    memory[i] = bytes_requested+1;
    memory[i + bytes_requested/4] = initS - bytes_requested;
    return &memory[i+1];
}

void myfree(int* p) {
//    free(p);
      p = p-1;
      *p = *p-1;

//    printf("p= %d\n",*p);
}

void fill(int* a, int size, int val) {
    for (int i=0; i<size; i++) {
        *(a+i)=val;
    }
}

int main() {
    mymalloc_init();
    print_memory();
    
    //make some random malloc calls
    int max=10;
    int *data[max];
    int N=3;//how many pointers are being used
    for (int i=0; i<N; i++) {
        int size = rand()%10+1;//random number between 1 and 10
        data[i] = (int*)mymalloc(size*sizeof(int));
//	print_memory();
        fill(data[i], size, mymalloc_calls);
//	print_memory();
    }

    //now make some random calls to malloc and free
    for (int i=0; i<10; i++) {
        //choose to malloc or free something with 50/50 probability each
        if (N<max && (double)rand()/RAND_MAX<.5) {
            int size = rand()%10+1;
            data[N] = (int*)mymalloc(size*sizeof(int));
            fill(data[N], size, mymalloc_calls);
            N++;
        }
        if (N>0 && (double)rand()/RAND_MAX<.5) {
            //choose a random pointer to free
            int index = rand()%N;
            printf("freeing index %d\n",index);
            myfree(data[index]);
            //swap last element in data, so first N are the valid pointers
            data[index]=data[N-1];
            N--;
        }
    }
    print_memory();
}
