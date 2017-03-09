// C++ program for implementation of Heap Sort
#include <stdio.h>
 
 // A utility function to swap two elements
void swap(int* a, int* b)
{

}

// To heapify a subtree rooted with node i which is
// an index in arr[]. n is size of heap
void heapify(int arr[], int n, int i)
{
    int largest = i;  // Initialize largest as root
    int l = 2*i + 1;  // left = 2*i + 1   index starts from 0 in C
    int r = 2*i + 2;  // right = 2*i + 2
 
    //max-heapify code
}

//build max heap function
void build_max_heap(int arr[], int n)
{
    //build max heap code
}
 
// main function to do heap sort
void heapSort(int arr[], int n)
{
    // Build heap (rearrange array)
    build_max_heap(arr, n);

 
    //heapsort code
}
 
/* A utility function to print array of size n */
void printArray(int arr[], int n)
{
    int i;
    for (i=0; i<n; ++i)
        printf("%d ",arr[i]);
    printf("\n");
}
 
// Driver program
int main()
{
    int arr[] = {12, 11, 13, 5, 6, 7};
    int n = sizeof(arr)/sizeof(arr[0]);
 
    heapSort(arr, n);
 
    printf("Sorted array is \n"); 
    printArray(arr, n);
}