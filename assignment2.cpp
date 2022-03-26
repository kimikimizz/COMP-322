// Comp 322 Assignment 2
// Name: Kim Zhao
// No collaborators

#include <iostream>
using namespace std;

// Store that will also serve as the node for the Linked List
class Store
{    
    private: 
        int *store_data;
        Store *next;
        int id;
        int ssize;

    public: 
        friend class Datastore;
        // Constructor
        Store(int ID, int ssize) 
        {
            this->store_data = new int[ssize]{};
            this->id = ID;
            this->ssize = ssize;
            this->next = NULL;
        }
        // Destructor
        ~Store()
        {
            // store_data is the only dynamically allocated memory
            delete [] store_data;
        }

        // Getter methods
        int* getData() {
            return this->store_data;
        }

        int getSize() {
            return this->ssize;
        }

        int getID() {
            return this->id;
        }

        Store* getNext() {
            return this->next;
        }

        // Setter method
        void set(int index, int value) {
            int *temp = this->store_data + index;
            *temp = value;
        }

        // Resizing the store
        void resizeStore(int new_size)
        {
            // Allocating a new array of size new_size
            int *new_data = new int[new_size]{};
            // If the new size is larger 
            if (new_size > ssize) {
                // Transfering all the data from the original array
                for (int i = 0; i < ssize; i++) {
                    int *temp = new_data + i;
                    *temp = *(this->store_data + i);
                } 
            // If the new size is smaller
            } else {
                // Transfering the data from the original array up until index new_size
                for (int i = 0; i < new_size; i++) {
                    int *temp = new_data + i;
                    *temp = *(this->store_data + i);
                } 
            }
            // Delete the original data
            delete[] store_data;
            // Assigni the new data
            store_data = new_data;
            // Updating the ssize
            this->ssize = new_size;
        }
        
};

// Linked List Datastore
class Datastore
{
    private: 
        Store *head, *tail;
        int total_used;
        int total_stores;
    
    public:   
        friend class Store;  
        // Constructor
        Datastore()
        {
            this->head = NULL;
            this->tail = NULL;
            this->total_stores = 0;
        }

        // Destructor
        ~Datastore()
        {
            // Deleting all the stores
            for (int i = 0; i < total_stores; i++) {
                Store *to_delete = head;
                Store *new_head = head->next;
                delete to_delete;
                this->head = new_head;
            }
        }

        // Append to the end of the Linked List
        void addLast(Store *newStore) 
        {
            // Update the total_stores, and the total_used
            total_stores++;
            total_used += newStore->ssize;
            // If the linked list is empty
            if (this->head == NULL) {
                this->head = newStore;
                this->tail = newStore;
            } 
            // Otherwise, assign pointer to tail->next
            else {
                this->tail->next = newStore;
                this->tail = tail->next;
            }
        }

        // Getter methods
        int getSize()
        {
            return total_stores;
        }

        int getTotal() 
        {
            return total_used;
        }

        Store* getFirst() 
        {
            return this->head;
        }

        Store* getLast() 
        {
            return this->tail;
        }

        Store* getNext(Store store) 
        {
            return store.next;
        }

        Store* getStore(int ID) 
        {
            Store *store = head;
            // Loop through the list to find the store
            for (int i = 0; i < total_stores; i++) {
                if ((*store).id == ID) {
                    return store;
                } else {
                    store = store->next;
                }
            }
            // return NULL if not found
            return NULL;
        }


        // Resize store and update the total_used in datastore
        void resizeStore(int id, int new_size) 
        {
            Store *store = getStore(id);
            int old_size = (*store).ssize;
            (*store).resizeStore(new_size);
            this->total_used = this->total_used - old_size + new_size;
        }

        // Remove Store
        void removeStore(int ID)
        {
            // If there is only one store in the datastore, reset the linked list
            if (total_stores == 1) {
                total_stores--;
                total_used -= head->ssize;
                delete head;
                head = NULL;
                tail = NULL;
            } 
            // If the store to remove is the first store
            else if (head->id == ID) {
                total_stores--;
                total_used -= head->ssize;
                Store *to_delete = head;
                head = head->next;
                delete to_delete;
            } else {
                Store *store = head;
                for (int i = 0; i < total_stores; i++) {
                    if (store->next->id == ID) {
                        total_used -= store->next->ssize;
                        Store *to_delete = store->next;
                        store->next = store->next->next;
                        total_stores--;
                        delete to_delete;
                        break;
                    }
                    store = store->next;
                }
            }
        }

};

// Keep track of each store within the same datastore
Datastore datastore = Datastore();

// Question 1
int newstore(int ssize) { 
    // Assigns a different ID to each new store
    static int ID = -1;
    ID++;
    // Add the new store to the datastore
    Store *newStore = new Store(ID, ssize);
    datastore.addLast(newStore);
    return ID;
}

// Question 2
int add_element_to_store(int id, int val, int idx=-1) {
    Store *store = datastore.getStore(id);
    // If the index isn't out of range (including -1)
    if (idx < (*store).getSize()) {
        // If no index was provided, add it to the first available spot. 
        if (idx == -1) {
            for (int i = 0;  i < (*store).getSize(); i++) {
                if (*((*store).getData()+i) == 0) {
                    (*store).set(i, val);
                    return 0;
                }
            }
        } else {
            (*store).set(idx, val);
            return 0;
        }
    }
    return -1;
}

// Question 3
void print_debug() {
    // Printing the entire datastore
    cout << "Total used elements in datastore: " << datastore.getTotal() << endl;
    cout << "datastore: "; 
    Store *store = datastore.getFirst(); 
    // Looping through the datastore to print everything
    for (int i = 0; i < datastore.getSize(); i++) {
        for (int j = 0; j < (*store).getSize(); j++) {
            cout << *((*store).getData() + j) << " ";
        }
        store = (*store).getNext();
    }
    cout << "\n";

    
    store = datastore.getFirst(); 
    // Looping through the existing stores
    for (int i = 0; i < datastore.getSize(); i++) {
        cout << "store " << (*store).getID() << ": ";
        for (int j = 0; j < (*store).getSize(); j++) {
            cout << *((*store).getData() + j) << " ";
        }
        cout << "\n";
        store = (*store).getNext();
    }

    cout << "#####################################" << endl; 
}

// Question 4
void delete_element_from_store_by_value(int id, int val)
{
    Store *store = datastore.getStore(id);
    for (int i = 0; i < (*store).getSize(); i++) {
        if(*((*store).getData() + i) == val) {
            (*store).set(i, 0);
            break;
        }
    }
}

void delete_element_from_store_by_index(int id, int idx)
{
    Store *store = datastore.getStore(id);
    if (idx < (*store).getSize()) {
        (*store).set(idx, 0);
    }
    
}

// Question 5
void which_stores_have_element(int val)
{
    cout << "Element " << val << " is ";
    Store *store = datastore.getFirst();
    int counter = 0;
    // Looping through all stores
    for (int i = 0; i < datastore.getSize(); i++) {
        for (int j = 0; j < (*store).getSize(); j++) {

            if (*((*store).getData() + j) == val) {

                if (counter == 0) {
                    cout << "in store IDs: " << (*store).getID();
                } else {
                    cout << ", "  << (*store).getID();
                }
                counter++;
                break;
            }
        }
        store = (*store).getNext();
    }

    if (counter == 0) {
        cout << "not available in any store" << endl;
    } else {
        cout << "\n";
    }
}

// Question 6
void delete_store(int id)
{
    datastore.removeStore(id);
}

// Question 7
int resize_store(int id, int newsize) 
{
    datastore.resizeStore(id, newsize);
    return 0;
}

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
