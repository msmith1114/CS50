/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include "helpers.h"
bool binary(int value, int values[], int min, int max);

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    int min = 0;
    int max = n-1;
    if(binary(value,values,min,max) == true)
    {
        return true;
    }
    else
    {
    return false;
    }
}

bool binary(int value, int values[], int min, int max)
{
    int current_index = ((min+max)/2);
    if(min > max)
    {
        return false;
    }
    else if(values[current_index] == value)
    {
        return true;
    }
    else if(values[current_index] < value)
    {
        min = current_index + 1;
        return binary(value,values,min,max);
    }
    else if(values[current_index] > value)
    {
        max = current_index - 1;
        return binary(value,values,min,max);
    }
    else
    {
        return false;
    }
   
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int temp;
    int max_value = 0;
    int s = 0;
    
    //Find Max Value for Counter Array
    for(int j = 0;j < n;j++) //This Works
    {   
        if(values[j] > max_value){
            max_value = values[j];
        }
    }
    int array_holder[max_value+1]; //Works
    //Fill Arrays with 0's and set array_holder to hold max value.
    for(int c = 0;c <= max_value;c++) //This Works
    {
        array_holder[c]=0;
    }
    //Update "Counters" on counter array
    for(int i = 0;i < n;i++) //This works
    {   
        temp = values[i];
        array_holder[temp] = array_holder[temp]+1;
    }
    for(int k = 0; k <= max_value; k++)
    {
        if(array_holder[k] == 0)
        {
            k++;
        }
        while(array_holder[k] > 0)
        {
            values[s] = k;
            array_holder[k] = array_holder[k] - 1;
            s++;
        }
    }
    return;
}
