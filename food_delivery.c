/* Online Food Delivery System - Final Review
   Function 1: New Food Orders              -> Queue
   Function 2: Delivery Assignment Rotation -> Circular Queue
   Function 3: Customer Order History       -> Doubly Linked List
   Function 4: Restaurant Categories        -> Binary Search Tree (BST)
   Function 5: Search Order by ID           -> Hash Table (Linear Probing) */
#include <stdio.h>                         // for printf, scanf
#include <stdlib.h>                        // for malloc, atoi
#include <string.h>                        // for strcmp, strcpy

#define MAX_ORDERS 50                      // maximum orders the queue can hold
#define TABLE_SIZE 100                     // slots in the hash table (more than MAX_ORDERS, so it never fills up)

struct Order {                             // one food order
    int id;                                // order number
    char customer[30];                     // customer name
    char item[30];                         // food item ordered
};

/* ---------- Function 1: Queue of new orders (FIFO) ---------- */
struct Order orders[MAX_ORDERS];           // array that stores the queue
int front = 0, rear = -1;                  // front = first order, rear = last order (empty when front > rear)

int enqueue(struct Order o) {              // adds a new order at the rear
    if (rear == MAX_ORDERS - 1) return 0;  // if array is full, return 0 (failed)
    orders[++rear] = o;                    // move rear ahead and store the order there
    return 1;                              // return 1 (success)
}

int dequeue(struct Order *o) {             // removes the oldest order from the front
    if (front > rear) return 0;            // if queue is empty, return 0 (failed)
    *o = orders[front++];                  // give the front order to caller, then move front ahead
    return 1;                              // return 1 (success)
}

/* ---------- Function 2: Circular queue of delivery agents ---------- */
char *agents[6] = {"Ravi", "Sneha", "Kiran", "Amit", "Priya", "Rohan"};  // names of the 3 delivery agents
int turn = 0;                              // index of the agent whose turn it is (front)

int assignAgent() {                        // picks the next agent in rotation
    int a = turn;                          // remember the agent whose turn it is
    turn = (turn + 1) % 6;                 // move to next agent; after the last (2) it wraps to 0
    return a;                              // return the chosen agent's index
}

/* ---------- Function 3: Customer order history (Doubly Linked List) ---------- */
struct Node {                              // one node of the history list
    struct Order data;                     // the delivered order
    struct Node *prev, *next;              // links to the previous and next node
};
struct Node *head = NULL, *tail = NULL;    // first and last node of the list

void addToHistory(struct Order o) {        // adds a delivered order at the end of the list
    struct Node *n = malloc(sizeof(struct Node));  // create a new node
    n->data = o;                           // store the order in the node
    n->next = NULL;                        // new node is last, so nothing comes after it
    n->prev = tail;                        // its previous node is the old last node
    if (tail == NULL) head = n;            // if list was empty, new node is also the head
    else tail->next = n;                   // otherwise old last node points to the new node
    tail = n;                              // new node becomes the last node
}

void showHistory() {                       // prints the history in both directions
    struct Node *p;                        // pointer used to walk through the list
    if (head == NULL) { printf("No history yet.\n"); return; }  // nothing to show
    printf("Oldest to newest:\n");         // heading for forward travel
    for (p = head; p != NULL; p = p->next) // start at head, move using next
        printf("  #%d %s - %s\n", p->data.id, p->data.customer, p->data.item);  // print order
    printf("Newest to oldest:\n");         // heading for backward travel
    for (p = tail; p != NULL; p = p->prev) // start at tail, move using prev
        printf("  #%d %s - %s\n", p->data.id, p->data.customer, p->data.item);  // print order
}

/* ---------- Function 4: Restaurant categories (Binary Search Tree) ---------- */
struct Category {                          // one node of the tree
    char name[30];                         // category name, e.g. Pizza
    struct Category *left, *right;         // smaller names go left, bigger names go right
};
struct Category *root = NULL;              // top node of the tree

struct Category *insertCategory(struct Category *node, char name[]) {  // adds a category
    if (node == NULL) {                    // found an empty place, so create the node here
        node = malloc(sizeof(struct Category));  // allocate memory for the node
        strcpy(node->name, name);          // store the category name
        node->left = node->right = NULL;   // new node has no children
    }
    else if (strcmp(name, node->name) < 0) // name comes before this node alphabetically
        node->left = insertCategory(node->left, name);   // so insert it in the left subtree
    else                                   // name comes after (or is equal to) this node
        node->right = insertCategory(node->right, name); // so insert it in the right subtree
    return node;                           // return the (updated) node
}

void showCategories(struct Category *node) {  // prints categories in A-Z order (inorder)
    if (node == NULL) return;              // empty subtree, nothing to print
    showCategories(node->left);            // first print everything smaller
    printf("  %s\n", node->name);          // then print this category
    showCategories(node->right);           // finally print everything bigger
}

/* ---------- Function 5: Search order by ID (Hash Table with Linear Probing) ---------- */
struct Order table[TABLE_SIZE];            // the hash table slots (a slot with id 0 is empty)

void insertOrder(struct Order o) {         // stores an order in the hash table
    int i = o.id % 10;                     // hash function: home slot = last digit of the ID
    while (table[i].id != 0)               // while this slot is already taken (collision)
        i = (i + 1) % TABLE_SIZE;          // try the next slot (wrap around at the end)
    table[i] = o;                          // store the order in the free slot
}

struct Order *searchOrder(int id) {        // finds an order by its ID (NULL if not found)
    int i = id % 10;                       // start at the home slot of this ID
    while (table[i].id != 0) {             // keep going until an empty slot is reached
        if (table[i].id == id) return &table[i];   // ID matches, return the order
        i = (i + 1) % TABLE_SIZE;          // otherwise try the next slot
    }
    return NULL;                           // empty slot reached, so the ID is not present
}

/* ---------- Main menu ---------- */
int main() {                               // program starts here
    int choice, id, nextId = 101;          // choice = menu option, id = ID to search, nextId = next order number
    char buf[20], name[30];                // buf = typed number (as text), name = category name
    struct Order o, *found;                // o = temporary order, found = result of a search

    while (1) {                            // repeat the menu until user exits
        printf("\n===== FOOD DELIVERY SYSTEM =====\n"     // print the menu title
               "1. Place new food order\n2. Assign next order (rotation)\n"  // menu options 1 and 2
               "3. View pending orders\n4. View order history\n"             // menu options 3 and 4
               "5. Add restaurant category\n6. View categories (A-Z)\n"      // menu options 5 and 6
               "7. Search order by ID\n8. Exit\n"                            // menu options 7 and 8
               "Enter choice: ");                           // ask for the choice
        scanf("%19s", buf);                // read what the user typed as text
        choice = atoi(buf);                // convert it to a number (letters become 0)

        if (choice == 1) {                 // option 1: place a new order
            o.id = nextId;                 // give the order its number
            printf("Customer name: ");     // ask for customer name
            scanf("%29s", o.customer);     // read the name (one word)
            printf("Food item: ");         // ask for food item
            scanf("%29s", o.item);         // read the item (one word)
            if (enqueue(o)) {              // try to add the order to the queue
                insertOrder(o);            // also store it in the hash table for searching
                printf("Order #%d placed successfully.\n", nextId++);  // success, then next order number
            } else                         // queue is full
                printf("Order queue is full.\n");  // show error message
        }
        else if (choice == 2) {            // option 2: assign the next order
            if (dequeue(&o)) {             // take the oldest order out of the queue
                int a = assignAgent();     // get the agent whose turn it is
                addToHistory(o);           // save the order in the customer history
                printf("Order #%d (%s for %s) assigned to Agent %s.\n",  // print the assignment
                       o.id, o.item, o.customer, agents[a]);             // values for the message
            } else                         // dequeue failed
                printf("No pending orders to assign.\n");  // queue is empty
        }
        else if (choice == 3) {            // option 3: show pending orders
            if (front > rear) printf("No pending orders.\n");  // empty queue message
            for (int i = front; i <= rear; i++)  // go from front to rear
                printf("#%d  %-8s %s\n", orders[i].id, orders[i].customer, orders[i].item);  // print one order
        }
        else if (choice == 4)              // option 4: show order history
            showHistory();                 // print the history list
        else if (choice == 5) {            // option 5: add a restaurant category
            printf("Category name: ");     // ask for the category
            scanf("%29s", name);           // read the category name (one word)
            root = insertCategory(root, name);  // insert it into the BST
            printf("Category added.\n");   // confirm to the user
        }
        else if (choice == 6) {            // option 6: show all categories
            if (root == NULL) printf("No categories added.\n");  // empty tree message
            else { printf("Categories (A-Z):\n"); showCategories(root); }  // print sorted list
        }
        else if (choice == 7) {            // option 7: search an order by ID
            printf("Enter order ID: ");    // ask for the ID
            scanf("%19s", buf);            // read it as text
            id = atoi(buf);                // convert it to a number (letters become 0)
            if (id <= 0) printf("Invalid ID.\n");  // IDs start from 101, so 0 or negative is wrong
            else if ((found = searchOrder(id)) != NULL)  // search; if an order is returned
                printf("Found: Order #%d - %s ordered %s.\n", found->id, found->customer, found->item);  // print it
            else                           // search returned NULL
                printf("Order not found.\n");  // no order with this ID
        }
        else if (choice == 8) { printf("Goodbye!\n"); break; }  // option 8: exit the loop
        else printf("Invalid choice. Try again.\n");  // any other number is invalid
    }
    return 0;                              // program ended successfully
}
