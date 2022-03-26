// Comp 322 Assignment 1
// Name: Kim Zhao
// Student ID: 260989483
// No collaborators

#include <iostream>
using namespace std;

#define ARRSIZE 50
int datastore[ARRSIZE] = {};
// Tracks the space left in the datastore array
int space_left = ARRSIZE;
// Stores the sizes of stores with respect to their ID
int sizes_of_stores[ARRSIZE] = {};

// Question 1
int newstore(int ssize) {
    // Assigns a different ID to each new store
    static int ID = -1;

    // If there is enough space for the new store
    if (space_left >= ssize) {
        // Update the space left, and track the size of the new store
        space_left -= ssize;
        ID++;
        sizes_of_stores[ID] = ssize;

        // Assign an ID to the new store. 
        return ID;
    }
    // Otherwise, return -1. 
    return -1;
}

// Question 2
int add_element_to_store(int id, int val, int idx=-1) {
    // If the index isn't out of range (including -1)
    if (idx < sizes_of_stores[id]) {

        // If no index was provided, add it to the first available spot. 
        if (idx == -1) {
            for (int i = idx; i <= (idx + sizes_of_stores[id]); i++) {
                if (datastore[i] == 0) {
                    datastore[i] = val;
                    return 0;
                }
            }
        // If the index was provided, add it to its true index. 
        } else {
            // Calculate the true index in the datastore. 
            for (int i = 0; i < id; i++) {
                if (sizes_of_stores[i] != -1){
                    idx += sizes_of_stores[i];
                }
            }
            datastore[idx] = val; 
            return 0;
        }
    }
    // The addition was unsuccessful. 
    return -1;
}

// Question 3
void print_debug() {
    // Printing the entire datastore
    cout << "available elements in datastore: " << space_left << endl;
    cout << "datastore: "; 
    for (int i = 0; i < ARRSIZE; i++) {
        cout << datastore[i] << " ";
    }
    cout << "\n";

    // Looping through the existing stores
    int index = 0;
    for (int i = 0; i < ARRSIZE; i++) {
        // If the store doesn't exist, break from the loop. 
        if (sizes_of_stores[i] == 0) {
            break; 
        }
        // If the store has been deleted, skip it
        if (sizes_of_stores[i] == -1) {
            continue; 
        }
        // Printing the individual stores
        cout << "store " << i << ": ";
        
        for (int j = 0; j < sizes_of_stores[i]; j++) {
            cout << datastore[index] << " ";
            index++;
        }
        cout << "\n";
    }
    cout << "#####################################" << endl; 
}

// Question 4
void delete_element_from_store_by_value(int id, int val)
{
    // Calculate the first index of the store
    int index = 0; 
    for (int i = 0; i < id; i++) {
        if (sizes_of_stores[i] != -1) {
            index += sizes_of_stores[i];
        }
    }

    // Loop through the store to find the value to delete
    for (int i = index; i < (index + sizes_of_stores[id]); i++) {
        if (datastore[i] == val) {
            datastore[i] = 0;
        }
    }
}

void delete_element_from_store_by_index(int id, int idx)
{
    // Determine if the index is within the store 
    if (idx < sizes_of_stores[id]) {
        // Calculate the true index to delete
        for (int i = 0; i < id; i++) {
            if (sizes_of_stores[i] != -1) {
                idx += sizes_of_stores[i];
            }
        }
        datastore[idx] = 0; 
    }
}

// Question 5
void which_stores_have_element(int val)
{
    // Tracks whether the store has the element
    bool has_element = false; 
    // Tracks the numnber of store that has the element 
    int counter = 0; 
    // Traversing all the store IDs
    for (int i = 0; i < ARRSIZE; i++) {
        if (sizes_of_stores[i] == 0) {
            break; 
        }
        // If the store contains the element, update has_element and counter. 
        for (int j = 0; j < sizes_of_stores[i]; j++) {
            if (datastore[j] == val) {
                has_element = true; 
                counter++; 
                break; 
            }
        }
        // If the element was found previously, print the store IDs
        if (has_element == true && counter == 1) {
            cout << "Element " << val << " is in store IDs: " << i; 
            has_element = false; 
        } else if (has_element == true) {
            cout << ", " << i; 
            has_element = false; 
        }
    }
    // Create a new line when we are done
    if (counter > 0) {
        cout << "\n"; 
    }
    // If the element was not found, print that it is unavailable. 
    if (has_element == false && counter == 0) {
        cout << "Element " << val << " is not available in any store" << endl; 
    }
}
    
// Question 6
void delete_store(int id)
{
    // Give the space back to the datastore
    space_left += sizes_of_stores[id];
    
    // Delete the values of the store from the datastore
    for (int i = 0; i < sizes_of_stores[id]; i++) {
        delete_element_from_store_by_index(id, i); 
    }

    // Calculate the index to shift forward for the following stores
    int shift = 0; 
    for (int i = 0; i < id; i++) {
        if (sizes_of_stores[i] != -1) {
            shift += sizes_of_stores[i];
        }
    }
    // Shift the values of the following stores
    for (int i = shift; i < (ARRSIZE); i++) {
        datastore[i] = datastore[i+sizes_of_stores[id]];
    }
    // Set the size to be -1 (for deleted stores)
    sizes_of_stores[id] = -1; 
}

// Question 7
int resize_store(int id, int newsize)
{
    // Sanity checks
    // If resizing a deleted store, return -1
    if (sizes_of_stores[id] == -1) {
        return -1;
    }
    // Resizing to 0 means deleting the store
    if (newsize == 0) {
        delete_store(id);
        return 0;
    }
    // If new size is the same as original size, do nothing
    if (newsize == sizes_of_stores[id]) {
        return 0;
    }

    // Check if there is enough space to accomodate the new size
    int extra_space = newsize - sizes_of_stores[id];
    if (extra_space <= space_left) {

        // If resizing to a smaller size, shift forward
        if (extra_space < 0) {
            // Calculate the index to shift forward for the following stores
            int shift = sizes_of_stores[id] - newsize;  
            for (int i = 0; i < id; i++) {
                if (sizes_of_stores[i] != -1) {
                    shift += sizes_of_stores[i];
                }
            }
            
            // Shift the values of the following stores
            for (int i = shift-1; i < (ARRSIZE); i++) {
                datastore[i] = datastore[i+shift];
            }
        
        // If resizing to a larger size
        } else {
            // Calculate the index to shift backward for the following stores
            int shift = extra_space;
            for (int i = 0; i < id; i++) {
                if (sizes_of_stores[i] != -1) {
                    shift += sizes_of_stores[i];
                }
            }
            // Shift the values of the following stores
            for (int i = (ARRSIZE-1); i >= shift; i--) {
                if (i >= newsize) {
                    datastore[i] = datastore[i-shift];
                } else {
                    datastore[i] = 0;
                }
            }
        }
        // Resize the store, and update the remaining space
        sizes_of_stores[id] = newsize; 
        space_left -= extra_space; 

        // We have successfully resized the store. 
        return 0;
    }
    // If there isn't enough space, return -1
    return -1;
}

// DO NOT ADD ANY LOGIC TO THE MAIN FUNCTION.
// YOU MAY MODIFY FOR YOUR OWN TESTS ONLY BUT THE ORIGINAL MAIN
// FUNCTION SHOULD BE SUBMITTED AS IS

int main()
{
    int s1 = newstore(3); // create new empty data store of size 3
    int s2 = newstore(5); // create new empty data store of size 5

    if (s1 != -1)
    {
        add_element_to_store(s1, 13);
        add_element_to_store(s1, 15);
        add_element_to_store(s1, 21);
        add_element_to_store(s1, 42); // this should return -1
    }
    
    if (s2 != -1)
    {
        add_element_to_store(s2, 7, 2);
        add_element_to_store(s2, 15, 0);
        add_element_to_store(s2, 22, 1);
    }
    print_debug();

    delete_element_from_store_by_value(s1, 13);
    delete_element_from_store_by_value(s1, 71);
    delete_element_from_store_by_index(s2, 2);
    delete_element_from_store_by_index(s1, 5);
    print_debug();

    which_stores_have_element(15);
    which_stores_have_element(29);

    delete_store(s1);
    print_debug();

    resize_store(s2, 20);
    int s3 = newstore(40);
    print_debug();

    s3 = newstore(30);
    add_element_to_store(s3, 7, 29);
    print_debug();

}