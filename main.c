// CODE: include necessary library(s)
#include "mySort.h"
#include <stdio.h>
#include <stdlib.h>

// Utility functions
void printArray(int arr[], int n);

// Test the sorting algorithms
int main()
{
    int arr[] = {64, -134, -5, 0, 25, 12, 22, 11, 90};
    int n = sizeof(arr) / sizeof(arr[0]);

    int testArr[n];

    // Bubble Sort
    memcpy(testArr, arr, n * sizeof(int));
    printf("Original array: ");
    printArray(testArr, n);
    bubbleSort(testArr, n);
    printf("Bubble sorted array: ");
    printArray(testArr, n);

    // insertion sort
    memcpy(testArr, arr, n * sizeof(int));
    printf("Original array: ");
    printArray(testArr, n);
    insertionSort(testArr, n);
    printf("Insersion sorted array: ");
    printArray(testArr, n);

    // merge sort
    memcpy(testArr, arr, n * sizeof(int));
    printf("Original array: ");
    printArray(testArr, n);
    mergeSort(testArr, 0, n - 1);
    printf("merge sorted array: ");
    printArray(testArr, n);

    // Heap sort
    memcpy(testArr, arr, n * sizeof(int));
    printf("Original array: ");
    printArray(testArr, n);
    heapSort(testArr, n);
    printf("heap sorted array: ");
    printArray(testArr, n);

    // Count sort
    memcpy(testArr, arr, n * sizeof(int));
    printf("Original array: ");
    printArray(testArr, n);
    countingSort(testArr, n);
    printf("count sorted array: ");
    printArray(testArr, n);

    // CODE: do the same test cases for Insertion Sort, Merge Sort, Heap Sort, Counting Sort
    // You will submit main.c, and your project will be marked based on main.c as well

    return 0;
}

// Helper functions
void printArray(int arr[], int n)
{
    if (n <= 0)
    {
        printf("Error, not valid array");
        return;
    }
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}
