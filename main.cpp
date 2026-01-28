#include <iostream>
#include<fstream>
#include "library.hpp"
#include "json.hpp"
#include <string>
#include <cctype>

using namespace std; 

int main(){
    mainMenu:
    int choice1; // main menu choice
    int choice2;  // choice to search book by
    string key1, key2; 
    cout << "--------------------------------------------" << endl; 
    cout << "What would you like to do " << endl; 
    cout << "1. Add New Book " << endl; 
    cout << "2. View All Books" << endl; 
    cout << "3. Search Books" << endl; 
    cin >> choice1; 
    Book b1;

    switch(choice1){
        case 1: 
            b1.saveBookLocally(); 
            saveBooktoJson(b1); 
            goto mainMenu; 
            break; 
        case 2:
            viewAllBooks(); 
            goto mainMenu; 
            break; 
        case 3:
            menu2:
            cout << "What would you like to search the book by? (Title/ Author/ ID): "; 
            cin >> key1; 
            key1 = toUpper(key1); 
            cout << "Enter the "<< key1 <<": "; 
            cin.ignore(); 
            getline(cin, key2); 
            key2 = toUpper(key2); 
            searchBooks(key1, key2); 
            goto mainMenu; 
            break; 
        default:
            break; 
    }
}