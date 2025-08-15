#include "mySort.h"
#include <stdio.h>
#include <stdlib.h>
// CODE: include necessary library(s)
// you have to write all the functions and algorithms from scratch,
// You will submit this file, mySort.c holds the actual implementation of sorting functions

void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

// Bubble Sort
void bubbleSort(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++) // set the loop to compare each neighbour elememts and push the largest one to the end
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
        }
    }
}

// CODE: implement the algorithms for Insertion Sort, Merge Sort, Heap Sort, Counting Sort

// insertion sort
void insertionSort(int arr[], int n)
{
    if (n <= 0) // if the array is empty, return error
    {
        printf("the array is not valid");
        return;
    }
    for (int i = 1; i < n; i++)
    {                                                       // assume the first element is sorted,and compare from the second element
        for (int j = i; j >= 1 && arr[j] < arr[j - 1]; j--) // if the previous element is larger then switch
        {                                                   // j is the index of the element that should be compared,
            int interm = arr[j];                            // a temperaory variable to store the value of jth element
            arr[j] = arr[j - 1];                            // switch values
            arr[j - 1] = interm;
        }
    }
}

// merge sort
void merge(int arr[], int l, int mid, int r) // the dividing function, in order to split the array
{
    int leftSize = mid - l + 1; // the size of left hand side array
    int rightSize = r - mid;    // the size of right hand side array

    // create a temporary array, use the memory allocation
    int *left = (int *)malloc(leftSize * sizeof(int)); // the purpose of the dynamic memory allocation is to storing the spliting arrays
    int *right = (int *)malloc(rightSize * sizeof(int));
    if (left == NULL || right == NULL)
    {
        printf("fails");
        return;
    }
    // fill the temp array
    for (int i = 0; i < leftSize; i++)
    {
        left[i] = arr[l + i]; // fill the left array
    }
    for (int i = 0; i < rightSize; i++)
    {
        right[i] = arr[mid + 1 + i]; // fill the right array
    }

    // merge the temp arr
    int i = 0, j = 0, k = l; // initial the temp indexs,to merge the sub array from the smallest sub arrays
    while (i < leftSize && j < rightSize)
    {
        if (left[i] <= right[j])
        {
            arr[k++] = left[i++];
        }
        else
        {
            arr[k++] = right[j++];
        }
    }

    // add the remaining elements
    while (i < leftSize)
    {
        arr[k++] = left[i++];
    }
    while (j < rightSize)
    {
        arr[k++] = right[j++];
    }

    // free memory
    free(left);
    free(right);
}

// the main function for merge sort
void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        int mid = l + (r - l) / 2; // the mid element, this is recursion that the mid element is defined at there can be used at function merge

        // call the left and right elements, using the recursion
        mergeSort(arr, l, mid);
        mergeSort(arr, mid + 1, r);

        // merge the sub array that has already sorted
        merge(arr, l, mid, r);
    }
}

// Heap sort
void heapify(int arr[], int n, int i) // heptify a subtree rotted at i
{                                     // assume the i is the index of the root node, n is length, arr[] is a array
    int largest = i;                  // let the largest node i, the top root
    int l = 2 * i + 1;                // left hand side node
    int r = 2 * i + 2;                // right hand side node

    if (l < n && arr[l] > arr[largest]) // if left hand side exists and large than max value
    {
        largest = l; // update left hand side
    }

    if (r < n && arr[r] > arr[largest]) // if right hand side is existed and large than this value
    {
        largest = r; // update the right hand side value
    }

    if (largest != i) // make sure the top root is the largest
    {
        swap(&arr[i], &arr[largest]); // exchange the max node and current node
        heapify(arr, n, largest);     // recusion, until it is a max heap
    }
}

void heapSort(int arr[], int n)
{
    if (n <= 0)
    {
        printf("Error, the array is not valid");
        return;
    }
    for (int i = n / 2 - 1; i >= 0; i--) // construct the max heap according the purposed-sorted array from previous function
    {
        heapify(arr, n, i);
    }

    for (int i = n - 1; i > 0; i--)
    {
        swap(&arr[0], &arr[i]); // switch the top root and the larger element at the lowest level
        heapify(arr, i, 0);
    }
}

// counting sort

void countingSort(int arr[], int n) // length
{
    if (n <= 0)
    {
        printf("Error, the array is not valid");
        return;
    }
    int max = arr[0]; // initialize the max and min elements
    int min = arr[0];
    for (int i = 0; i < n; i++) // determine the length of the empty array (counting array)
    {
        if (arr[i] > max)
        {
            max = arr[i]; // determine the max and min value for elements
        }
        if (arr[i] < min)
        {
            min = arr[i];
        }
    }
    int range = max - min + 1;                     // the range is the max + min (for abs value) and +1 since the index starts from 0
    int *temp = (int *)calloc(range, sizeof(int)); // create an empty array to count, size is the max element in the given array
    if (temp == NULL)
    {
        printf("failed");
        return;
    }
    for (int i = 0; i < n; i++)
    {
        temp[arr[i] - min]++; // fill the empty array to count the number of a element that appear in the array that should be sorted
    } // add the abs(min) to the arr[i] (since arr[i] could be positive or negative) to get the real index in the empty array and count +1
    int j = 0;                      // the index of the arr[]
    for (int i = 0; i < range; i++) // replace the odd elements in arr[] to the new-sorted elements
    {
        while (temp[i] > 0)
        {
            arr[j++] = i + min; // convert back the index to the real value
            temp[i]--;          // deduct the counting times
        }
    }
    free(temp); // free the memory
}