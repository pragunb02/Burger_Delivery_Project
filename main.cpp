#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>
const int infinity = INT_MAX;
using namespace std;

// burger Shop Ordering Management System

// Defining Data Types

struct customer
{
    int age;
    string name;
    string burgerName;
    int quantity;
    double bill;

    customer() {}

    customer(int age, string name, int quantity, string burgerName, double bill)
    {
        this->age = age;
        this->name = name;
        this->burgerName = burgerName;
        this->quantity = quantity;
        this->bill = bill;
    }
};

struct takeAwayCustomer
{
    customer cusotomer;
    takeAwayCustomer *next = NULL;

    takeAwayCustomer(int age, string name, int quantity, string burgerName, double bill)
    {
        this->cusotomer = customer(age, name, quantity, burgerName, bill);
    }
};

struct dineInCustomer //queue mai ho raha hai
{
    customer cusotomer;
    dineInCustomer *next = NULL;

    dineInCustomer(int age, string name, int quantity, string burgerName, double bill)
    {
        this->cusotomer = customer(age, name, quantity, burgerName, bill);
    }
};

struct homeDeliveryCustomer
{

    customer cusotomer;
    string address;
    double deliveryCharges;
    int distanceDelivery;
    struct homeDeliveryCustomer *next = NULL;

    homeDeliveryCustomer(int age, string name, int quantity, string burgerName, double bill, string address, double deliveryCharges, int distanceDelivery)
    {
        this->cusotomer = customer(age, name, quantity, burgerName, bill);
        this->address = address;
        this->deliveryCharges = deliveryCharges;
        this->distanceDelivery = distanceDelivery;
    }
};

struct burgerShop
{

    string shopName;
    string *menu;
    int *price;
    string address;
    takeAwayCustomer *nextTakeAwayCustomer = NULL;
    dineInCustomer *nextDineInCustomer = NULL;
    homeDeliveryCustomer *nextHomeDeliveryCustomer = NULL;
};

// Globally declaring the burger Shop's pointer
burgerShop *myburgerShop = NULL;

// Globally Declaring the Current Customer's Pointers for all three Types
takeAwayCustomer *currentTakeAwayCustomer = NULL;
dineInCustomer *currentDineInCustomer = NULL;
homeDeliveryCustomer *currentHomeDeliveryCustomer = NULL;

// Globally declaring the variables for the total of all the orders in queue!
double total, takeAway, dineIn, homeDelivery;
// Globally declaring the variables for the total of all the orders served!
double servedTotal;

// In case of Serving , to keep the record of Customers Served, implementing AVL Tree for efficient Search
// to search the record of Customers by Name
// It can also Display all the customers Served

struct servedCustomer
{

    int age;
    string name;
    int quantity;
    string burgerName;
    double bill;
    string customerType;
    servedCustomer *left;
    servedCustomer *right;

    // Constructor
    servedCustomer(int age, string name, int quantity, string burgerName, double bill, string customerType)
    {
        // setting customers details

        this->age = age;
        this->name = name;
        this->quantity = quantity;
        this->burgerName = burgerName;
        this->bill = bill;
        this->customerType = customerType;

        // child to NULL
        this->left = NULL;
        this->right = NULL;
    }
};

servedCustomer *root = NULL; // Global pointer for the root of AVLTree

// isEmpty or not
int isEmpty(servedCustomer *root)
{
    return root == NULL;
}

// display Customers Details

void display(servedCustomer *root)
{
    cout << "Name :" << root->name << endl;
    cout << "Age  :" << root->age << endl;
    cout << "burger :" << root->burgerName << endl;
    cout << "Quantity : " << root->quantity << endl;
    cout << "Bill : " << root->bill << endl;
    cout << "Customer Type: " << root->customerType << endl;
}

// Traversal for the served Customers

servedCustomer *displayAllServedOrders(servedCustomer *root)
{

    if (root)
    {
        displayAllServedOrders(root->left);
        display(root); // will display all the served Customers
        displayAllServedOrders(root->right);
    }

    return root;
}

// Height of servedCustomer tree

int height(servedCustomer *root)
{
    if (!root)
        return 0;

    return max(height(root->left), height(root->right)) + 1;
}

// Balance Factor for each ServedCustomer node

int balanceFactor(servedCustomer *root)
{
    if (!root)
        return 0;

    return height(root->left) - height(root->right);
}

// Maximum of two integers as a helper function for height
int max(int a, int b)
{
    return (a > b) ? a : b;
}

// Searching in servedCustomer tree

servedCustomer *search(servedCustomer *root, string keyName)
{
    if (root == NULL)
    {
        return NULL;
    }
    else if (root->name == keyName)
    {
        return root;
    }
    else if (root->name < keyName)
    {
        return search(root->right, keyName);
    }
    else if (root->name > keyName)
    {
        return search(root->left, keyName);
    }

    return root;
}

// Finding Maximum Node of servedCustomer tree

servedCustomer *maxservedCustomer(servedCustomer *root)
{
    // Maximum Node is Present in the most Right Node  of the served Customer Tree

    servedCustomer *p = root;
    servedCustomer *temp = NULL;

    while (p != NULL)
    {
        temp = p;
        p = p->right;
    }

    return temp;
}

// Balancing the ServedCustomer's Tree thorugh AVL Rotations

// LL Rotation
servedCustomer *LLRotation(servedCustomer *root)
// rotate wese right per krna hai!
{
    // saving the new root and the lost element in case of rotation
    servedCustomer *x = root->left;
    servedCustomer *temp = x->right;

    // Performing rotation
    x->right = root;
    root->left = temp;

    // updating the root
    root = x;
    // returning the root
    return x;
}

// RR Rotation
servedCustomer *RRRotation(servedCustomer *root)
{
    // rotate wese left per krna hai!

    // saving the new root and the lost element in case of rotation

    servedCustomer *x = root->right;
    servedCustomer *temp = x->left;

    // Performing rotation
    x->left = root;
    root->right = temp;

    // updating the root
    root = x;

    // returning the root
    return x;
}

// LR Rotation
servedCustomer *LRRotation(servedCustomer *root)
{
    root->left = RRRotation(root->left);
    return LLRotation(root);
}

// RL Rotation
servedCustomer *RLRotation(servedCustomer *root)
{
    root->right = LLRotation(root->right);
    return RRRotation(root);
}

//  INSERTION in servedCustomer Tree

servedCustomer *insertion(int age, string name, int quantity, string burgerName, double bill, string customerType, servedCustomer *root)
{
    servedCustomer *newNode = new servedCustomer(age, name, quantity, burgerName, bill, customerType);

    if (root == NULL)
    {
        root = newNode;
    }

    else if (root->name > newNode->name)
    {
        root->left = insertion(age, name, quantity, burgerName, bill, customerType, root->left);
    }
    else if (root->name < newNode->name)
    {
        root->right = insertion(age, name, quantity, burgerName, bill, customerType, root->right);
    }

    else
    {
        cout << "No duplicates Values are Allowed " << endl;
        return root;
    }

    int bf = balanceFactor(root);

    if (bf == 2)
    {
        // LL
        if (root->left->name > newNode->name)
        {
            return LLRotation(root);
        }

        // LR
        if (root->left->name < newNode->name)
        {
            return LRRotation(root);
        }
    }
    else if (bf == -2)
    {
        // RR
        if (root->right->name < newNode->name)
        {
            return RRRotation(root);
        }

        // RL
        if (root->right->name > newNode->name)
        {
            return RLRotation(root);
        }
    }

    return root; // in case there is no need of rotation
}

void placeOrderTakeAwayCustomer(int age, string name, string burgerName, int quantity, double bill)
{
    // making new Customer
    currentTakeAwayCustomer = new takeAwayCustomer(age, name, quantity, burgerName, bill);

    if (myburgerShop->nextTakeAwayCustomer == NULL)
    {
        // if first then insert in front
        myburgerShop->nextTakeAwayCustomer = currentTakeAwayCustomer;
    }
    else
    {
        // finding the last Node
        takeAwayCustomer *temp = myburgerShop->nextTakeAwayCustomer;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        if (temp->cusotomer.age < currentTakeAwayCustomer->cusotomer.age)
        {
            // insert at front
            takeAwayCustomer *firstCustomer = myburgerShop->nextTakeAwayCustomer;
            myburgerShop->nextTakeAwayCustomer = currentTakeAwayCustomer;
            currentTakeAwayCustomer->next = firstCustomer;
        }
        else
        {
            // insert at end
            temp->next = currentTakeAwayCustomer;
            currentTakeAwayCustomer->next = NULL;
        }
    }
    cout << "Your Order has been Placed MR/MRS " << name << " and your order is " << burgerName << " with " << quantity << " quantity and total bill is " << bill << endl;
}
void serveOrderTakeAwayCustomer()
{
    if (myburgerShop->nextTakeAwayCustomer == NULL)
    {
        cout << "No Take Away Customer to Serve" << endl;
    }
    
    else
    {
        // serving the first customer
        takeAwayCustomer *temp = myburgerShop->nextTakeAwayCustomer;
        // if it has some next element
        if(temp->next != NULL){
            myburgerShop->nextTakeAwayCustomer = temp->next;
        }
        else{
            myburgerShop->nextTakeAwayCustomer = NULL;
        }
        
        cout << "Take Away Customer Served : " << temp->cusotomer.name << endl;

        string customerType = "Take-Away";
        // Now before deleting the node we need to update the servedCustomer Tree
        root = insertion(temp->cusotomer.age, temp->cusotomer.name, temp->cusotomer.quantity, temp->cusotomer.burgerName, temp->cusotomer.bill, customerType, root);

        delete temp; // deleting the customer
    }
}

// Now defining Order Placing and Serving of Dine-In Customer
// Based on : First Come First Served (FIFO) (QUEUE)

void placeOrderDineInCustomer(int age, string name, string burgerName, int quantity, double bill)
{
    // making new Customer
    currentDineInCustomer = new dineInCustomer(age, name, quantity, burgerName, bill);

    if (myburgerShop->nextDineInCustomer == NULL)
    {
        // if first insert in front
        myburgerShop->nextDineInCustomer = currentDineInCustomer;
    }
    else
    {
        // finding the last Node
        dineInCustomer *temp = myburgerShop->nextDineInCustomer;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = currentDineInCustomer;
        currentDineInCustomer->next = NULL;
    }
    cout << "Your Order has been Placed MR/MRS " << name << " and your order is " << burgerName << " with " << quantity << " quantity and total bill is " << bill << endl;
}
void serveOrderDineInCustomer()
{
    if (myburgerShop->nextDineInCustomer == NULL)
    {
        cout << "No Dine-In Customer to Serve" << endl;
    }
    else
    {
        // serving the first customer
        dineInCustomer *temp = myburgerShop->nextDineInCustomer;
        if(temp->next != NULL){
            myburgerShop->nextDineInCustomer = temp->next;
        }
        else{
            myburgerShop->nextDineInCustomer = NULL;
        }

        cout << "Dine-In Customer Served : " << temp->cusotomer.name << endl;

        string customerType = "Dine-In";
        // Now before deleting the node we need to update the servedCustomer Tree
        root = insertion(temp->cusotomer.age, temp->cusotomer.name, temp->cusotomer.quantity, temp->cusotomer.burgerName, temp->cusotomer.bill, customerType, root);

        delete temp; // deleting the customer
    }
}

// Now defining Order Placing and Serving of Home Delivery Customer
// Based on : (when all orders are ready)(LIFO)(Stack)

void placeOrderHomeDeliveryCustomer(int age, string name, string burgerName, int quantity, double bill, string address, int deliveryCharges, int distanceDelivery)
{
    // making new Customer
    currentHomeDeliveryCustomer = new homeDeliveryCustomer(age, name, quantity, burgerName, bill, address, deliveryCharges, distanceDelivery);

    if (myburgerShop->nextHomeDeliveryCustomer == NULL)
    {
        // if first insert in front
        myburgerShop->nextHomeDeliveryCustomer = currentHomeDeliveryCustomer;
    }
    else
    {
        // finding the last Node
        homeDeliveryCustomer *temp = myburgerShop->nextHomeDeliveryCustomer;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = currentHomeDeliveryCustomer;
        currentHomeDeliveryCustomer->next = NULL;
    }
    cout << "Your Order has been Placed MR/MRS " << name << " and your order is " << burgerName << " with " << quantity << " quantity and total bill is " << bill << endl;
}

void serveOrderHomeDeliveryCustomer()
{
    if (myburgerShop->nextHomeDeliveryCustomer == NULL)
    {
        cout << "No Home Delivery Customer to Serve" << endl;
    }
    else
    {

        // serving the last customer first
        homeDeliveryCustomer *first = myburgerShop->nextHomeDeliveryCustomer;

        if (first->next == NULL)
        {
            // if it is the only customer

            myburgerShop->nextHomeDeliveryCustomer = NULL;

            cout << "Home Delivery Customer Served : " << first->cusotomer.name << endl;
            string customerType = "Home-Delivery Customer";
            root = insertion(first->cusotomer.age, first->cusotomer.name, first->cusotomer.quantity, first->cusotomer.burgerName, first->cusotomer.bill, customerType, root);

            // now deleting the node
            delete (first);
        }
        else {
            homeDeliveryCustomer *s = first->next;
            while(s->next !=NULL){
                first = first->next;
                s = s->next;
            }

            first->next = NULL;

            cout << "Home Delivery Customer Served : " << s->cusotomer.name << endl;
            string customerType = "Home-Delivery Customer";
            root = insertion(s->cusotomer.age, s->cusotomer.name, s->cusotomer.quantity, s->cusotomer.burgerName, s->cusotomer.bill, customerType, root);

            // deleting the node

            delete (s);
        }

        
        // deleting the customer
    }
}

// It will serve all the waiting orders

void serveAllOrders()
{

    while (myburgerShop->nextTakeAwayCustomer != NULL)
    {
        serveOrderTakeAwayCustomer();
    }
    while (myburgerShop->nextDineInCustomer != NULL)
    {
        serveOrderDineInCustomer();
    }
    while (myburgerShop->nextHomeDeliveryCustomer != NULL)
    {
        serveOrderHomeDeliveryCustomer();
    }
}

void displayAllOrdersTakeAwayCustomers()
{
    if (myburgerShop->nextTakeAwayCustomer == NULL)
    {
        cout << "There is no Order for Walking Customer till yet" << endl;
    }
    else
    {
        takeAwayCustomer *traversal = myburgerShop->nextTakeAwayCustomer;
        while (traversal != NULL)
        {

            cout << "-----------------------------------------------------" << endl;
            cout << "Take-Away Customer : " << traversal->cusotomer.name << endl;
            cout << "Age : " << traversal->cusotomer.age << endl;
            cout << "burger Name : " << traversal->cusotomer.burgerName << endl;
            cout << "Quantity : " << traversal->cusotomer.quantity << endl;
            cout << "Bill : " << traversal->cusotomer.bill << " RS/_" << endl;
            cout << "-----------------------------------------------------" << endl;

            traversal = traversal->next;
        }
    }
}

void displayAllOrdersHomeDeliveryCustomers()
{
    if (myburgerShop->nextHomeDeliveryCustomer == NULL)
    {
        cout << "There is no Order for Home Delivery Customer till yet" << endl;
    }
    else
    {
        homeDeliveryCustomer *traversal = myburgerShop->nextHomeDeliveryCustomer;
        while (traversal != NULL)
        {
            cout << "-----------------------------------------------------" << endl;
            cout << "Home Delivery Customer : " << traversal->cusotomer.name << endl;
            cout << "Age : " << traversal->cusotomer.age << endl;
            cout << "burger Name : " << traversal->cusotomer.burgerName << endl;
            cout << "Quantity : " << traversal->cusotomer.quantity << endl;
            cout << "Delivery Distance : " << traversal->deliveryCharges << "KM"<<endl;
            cout << "Delivery Charges : " << traversal->distanceDelivery << endl;
            cout << "Bill : " << traversal->cusotomer.bill << " RS/_" << endl;
            cout << "Delivery Address : " << traversal->address << endl;
            cout << "-----------------------------------------------------" << endl;

            traversal = traversal->next;
        }
    }
}

void displayAllOrdersDineInCustomers()
{
    if (myburgerShop->nextDineInCustomer == NULL)
    {
        cout << "There is no Order for Dine-In Customer till yet" << endl;
    }
    else
    {
        dineInCustomer *traversal = myburgerShop->nextDineInCustomer;
        while (traversal != NULL)
        {
            cout << "-----------------------------------------------------" << endl;
            cout << "Walking Customer : " << traversal->cusotomer.name << endl;
            cout << "Age : " << traversal->cusotomer.age << endl;
            cout << "burger Name : " << traversal->cusotomer.burgerName << endl;
            cout << "Quantity : " << traversal->cusotomer.quantity << endl;
            cout << "Bill : " << traversal->cusotomer.bill << " RS/_" << endl;
            cout << "-----------------------------------------------------" << endl;

            traversal = traversal->next;
        }
    }
}

void displayAllOrders()
{

    cout << "The Take-Away Customers Are :" << endl;
    displayAllOrdersTakeAwayCustomers();

    cout << "The Dine-IN Customers Are :" << endl;
    displayAllOrdersDineInCustomers();

    cout << "The Home Delivery Customers Are :" << endl;
    displayAllOrdersHomeDeliveryCustomers();
}

void totalbillofPendingOrders()
{
    takeAwayCustomer *p = myburgerShop->nextTakeAwayCustomer;
    while (p != NULL)
    {
        takeAway += p->cusotomer.bill;
        p = p->next;
    }
    dineInCustomer *q = myburgerShop->nextDineInCustomer;
    while (q != NULL)
    {
        dineIn += q->cusotomer.bill;
        q = q->next;
    }
    homeDeliveryCustomer *r = myburgerShop->nextHomeDeliveryCustomer;
    while (r != NULL)
    {
        homeDelivery += r->cusotomer.bill;
        r = r->next;
    }
    total = takeAway + dineIn + homeDelivery;

    cout << "The total bill of pending orders for Take-Away customers are : " << takeAway << " RS/_" << endl;
    cout << "The total bill of pending orders for Dine-In customers are : " << dineIn << " RS/_" << endl;
    cout << "The total bill of pending orders for Delivery customers are : " << homeDelivery << " RS/_" << endl;
    cout << "The Total orders pending are : " << total << " RS/_" << endl;
}

double totalEarnings(servedCustomer *root){

    if(root){
        totalEarnings(root->left);
        servedTotal += root->bill;
        totalEarnings(root->right);
    }

    return servedTotal;
}

// making a graph for the available delivery options

//                           0              1             2        3          4           5
string deliveryPoints[] = {"burgerSHOP", "Sector-62", "Sector-128", "Delhi", "Sector-59", "Sector-18"};

// first value in the pair is vertex and second is the distance (weight) in KM

vector<vector<pair<int, int>>> deliveryMap = {

    // first value in the pair is vertex and second is the distance (weight) in KM
    {{1, 2}, {2, 3}, {3, 5}, {5, 4}}, //  0  (burger Shop)
    {{0, 2}, {5, 1}},                 //  1  (Sector-62)
    {{0, 3}, {3, 1}},                 //  2  (Sector-128)
    {{0, 5}, {4, 2}, {5, 2}, {2, 1}}, //  3  (Delhi)
    {{3, 2}, {5, 2}},                 //  4  (Sector-18)
    {{0, 4}, {1, 1},{3,2} ,{4, 2}}          //  5  (Sector-59)

};

vector<int> dijkstra(int sourceNode)
{
    vector<int> distance(6, infinity);
    set<pair<int, int>> s;
    distance[sourceNode] = 0; // burger Shop
    s.insert(make_pair(0, sourceNode));

    while (!s.empty())
    {
        auto top = *(s.begin());
        int u = top.first;  //   current weight
        int v = top.second; //  current vertex

        s.erase(s.begin());

        // traversing the adjacency list of v
        for (auto child : deliveryMap[v])
        {
            int childVertex = child.first;
            int childWeight = child.second;

            if (u + childWeight < distance[childVertex])
            {
                distance[childVertex] = u + childWeight;
                s.insert(make_pair(distance[childVertex], childVertex));
            }
        }
    }

    return distance;
}

int main()
{
    // making burger shop
    myburgerShop = new burgerShop;

    // setting the name
    myburgerShop->shopName = "The burger Delight";

    // setting the address
    myburgerShop->address = "Liberty Chowk, Lahore";

    // Setting the menu
    myburgerShop->menu = new string[11]{"",
                                       "chickenTikka", "arabicRanch",
                                       "chickenFajita", "cheeseLover",
                                       "chickenSupreme", "allveggie",
                                       "garlicWest", "BeefBold",
                                       "phantom", "mexicanDelight"};

    // setting the price

    myburgerShop->price = new int[11]{0, 2000, 2500, 2400, 2200, 2700, 2000, 2100, 3000, 3000, 2800};

    int option = -99;

    // now starting the main program
    do
    {

        cout << "-------------------------------------------------------------------------" << endl;
        cout << "---------------------------------" << myburgerShop->shopName << "-----------------------" << endl;
        cout << "-------------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------------" << endl;

        cout << "Located at " << myburgerShop->address << endl;
        cout << "Our Menu is as follows: " << endl;
        for (int i = 1; i <= 10; i++)
        {
            cout << i << ". " << myburgerShop->menu[i] << " - " << myburgerShop->price[i] << endl;
        }

        cout << "-------------------------------------------------------------------------" << endl;
        cout << "---------------------------------Operations------------------------------" << endl;
        cout << "-------------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------------" << endl;

        // Order placing
        cout << "1. Place order for Take-Away Customer" << endl;
        cout << "2. Place order for Home Delivery Customer" << endl;
        cout << "3. Place order for Dine-In Customer" << endl;

        // order serving
        cout << "4. Serve order for Take-Away Customer" << endl;
        cout << "5. Serve order for Home Delivery Customer" << endl;
        cout << "6. Serve order for Dine-In Customer" << endl;
        cout << "7. Serve All Orders " << endl;

        // Displaying orders
        cout << "8. Display all orders of Take-Away Customer" << endl;
        cout << "9. Display all orders of Home Delivery Customers" << endl;
        cout << "10. Display all orders of Dine-In Customers" << endl;
        cout << "11. Display all orders of all Customers" << endl;

        // Served orders
        cout << "12. Display all served Orders" << endl;
        cout << "13. Search Served Orders " << endl;
        // cout << "14. Clear the Served Orders List " << endl;
        cout << "14. Display total bill of Pending Orders " << endl;
        cout << "15. Display the total Earnings of Served Orders " << endl;
        cout << "0.  EXIT " << endl;

        cout << "Enter your choice: ";

        cin >> option;

        // for taking input of Customer Details
        int age, quantity, burgerIndex;
        double bill;
        string address;
        string name;

        switch (option)
        {
        case 1:
        { // placing order for take away customer
            cout << "Enter the name of the customer: ";
            cin >> name;
            cout << "Enter the age of the customer: ";
            cin >> age;
            cout << "Enter the quantity of the burger: ";
            cin >> quantity;
            cout << "Enter the option for the burger: ";
            cin >> burgerIndex;

            bill = quantity * myburgerShop->price[burgerIndex];
            placeOrderTakeAwayCustomer(age, name, myburgerShop->menu[burgerIndex], quantity, bill);
        }
        break;

        case 2:

        {                                               // placing order for Home Delivery customer
            vector<int> distanceFromShop = dijkstra(0); // As Zero is our Source Node

            int optionDelivery = -999;

            do
            {
                cout << "The delivery is available for following Areas : " << endl;
                for (int i = 1; i <= 5; i++)
                {
                    cout << i << ". " << deliveryPoints[i] << endl;
                }

                cout << "Enter your option :" << endl;
                cin >> optionDelivery;

            } while (!(optionDelivery >= 0 && optionDelivery <= 5));

            // setting the delivery address of the Customer
            address = deliveryPoints[optionDelivery];

            cout << "Enter the name of the customer: ";
            cin >> name;
            cout << "Enter the age of the customer: ";
            cin >> age;
            cout << "Enter the quantity of the burger: ";
            cin >> quantity;
            cout << "Enter the option for the burger: ";
            cin >> burgerIndex;

            int deliveryChargesPerKM = 50;
            int deliveryCharges = deliveryChargesPerKM * distanceFromShop[optionDelivery];
            bill = quantity * myburgerShop->price[burgerIndex] + deliveryCharges;

            // distance from the shop
            int distanceFromTheShop = distanceFromShop[optionDelivery];
            placeOrderHomeDeliveryCustomer(age, name, myburgerShop->menu[burgerIndex], quantity, bill, address, distanceFromTheShop, deliveryCharges);
        }
        break;

        case 3:
        { // placing order for Dine-in customer

            cout << "Enter the name of the customer: ";
            cin >> name;
            cout << "Enter the age of the customer: ";
            cin >> age;
            cout << "Enter the quantity of the burger: ";
            cin >> quantity;
            cout << "Enter the option for the burger: ";
            cin >> burgerIndex;

            bill = quantity * myburgerShop->price[burgerIndex];
            placeOrderDineInCustomer(age, name, myburgerShop->menu[burgerIndex], quantity, bill);
        }
        break;

        case 4:
            // serving order for Take Away customer
            serveOrderTakeAwayCustomer();
            break;

        case 5:
            // serving order for Home Delivery customer
            serveOrderHomeDeliveryCustomer();
            break;

        case 6:
            // serving order for Dine-in customer
            serveOrderDineInCustomer();
            break;

        case 7:
            // serving all orders
            serveAllOrders();
            break;

        case 8:
            // displaying all orders of Take-away customers
            displayAllOrdersTakeAwayCustomers();
            break;

        case 9:
            // displaying all orders of Home Delivery customers
            displayAllOrdersHomeDeliveryCustomers();
            break;

        case 10:
            // displaying all orders of Dine-in customers
            displayAllOrdersDineInCustomers();
            break;

        case 11:
            // displaying all orders of all customers
            displayAllOrders();
            break;

        case 12:
        { // displaying all served orders
            servedCustomer *r = displayAllServedOrders(root);
            if (!r)
                cout << "No Served Customer yet " << endl;
        }
        break;

        case 13:
        { // searching served orders
            cout << "Enter the name of the customer you want to search: " << endl;
            cin >> name;
            servedCustomer *searchedCustomer = search(root, name);
            if (searchedCustomer == NULL)
                cout << "No such Customer was Served " << endl;
            else
                display(searchedCustomer);
        }
        break;

        case 14:
        { // pending orders bill in queue for all!

            // resetting the state of total orders
            takeAway = 0, dineIn = 0, homeDelivery = 0, total = 0; 
            totalbillofPendingOrders();
        }
        break;

        case 15:
        { // total earnings from served orders!

        // resetting thr state of total served orders
        servedTotal = 0;
        double totalx = totalEarnings(root);
        cout << "The Total Earnings are : " << totalx << endl;
        }
        break;
        }

    } while (option != 0);

    cout << "burger Order Management System -- Terminated" << endl;
    cout << "Thank you for Using our Services " << endl;

    return 0;
}