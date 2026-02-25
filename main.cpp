#include <iostream>
#include<fstream>
#include "library.hpp"
#include "json.hpp"
#include <string>
#include <cctype>
#include <random>

using namespace std; 

int main(){
    mainMenu:
    int choice1; // main menu choice
    int choice2;  // choice to search book by
    int choice3; //choice to pay fine
    string key1, key2; 
    string titleToBorrow; 
    string titleToReturn; 

    //display main window for login or creating account
    string username, password, userType; 
    cout << "--------------------------------------------" << endl; 
    cout << "1. Create Account" << endl; 
    cout << "2. Log In" << endl; 
    cin >> choice1; 

    //create new account
    switch(choice1){
        case 1:
            cout << "Enter username: "; 
            cin >> username; 
            cout << "Enter password: "; 
            cin >> password; 
            cout << "Enter user type: "; 
            cin >> userType; 

            if(checkUsername(username) == true){
                cout << "User Already Exists" << endl; 
                goto mainMenu; 
            }
            else{
                createAccount(username, userType, password); 
                goto mainMenu; 
            }

        //log in to existing account
        case 2:
            cout << "Enter your username: "; 
            cin >> username; 
            cout << "Enter password: "; 
            cin >> password;

            userType = LogIn(username, password); 

            if(userType == "Admin"){
                loginMenuAdmin:
                cout << "--------------------------------------------" << endl; 
                cout << "What would you like to do " << endl; 
                cout << "1. Add New Book " << endl; 
                cout << "2. View All Books" << endl; 
                cout << "3. Search Books" << endl; 
                cout << "4. Delete Book" << endl; 
                cout << "5. Log Out" << endl; 
                cin >> choice2; 
                Book b1;

                switch(choice2){
                    case 1: 
                        b1.saveBookLocally(); 
                        saveBooktoJson(b1); 
                        goto loginMenuAdmin; 
                        break; 
                    case 2:
                        viewAllBooks(); 
                        goto loginMenuAdmin;  
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
                        goto loginMenuAdmin;  
                        break; 
                    case 4:
                        deleteBook(); 
                        cout << "Book deleted successfully. " << endl; 
                        break; 
                    case 5:
                        goto mainMenu; 
                    default:
                        break; 
                }
            }
            else if(userType == "Student"){
                calculateFine(username); 
                loginMenuStudent: 
                cout << "--------------------------------------------" << endl; 
                cout << "What would you like to do " << endl; 
                cout << "1. Search books" << endl; 
                cout << "2. Borrow a book" << endl; 
                cout << "3. Return a book" << endl; 
                cout << "4. View Profile" << endl; 
                cout << "5. Log Out" << endl; 
                cin >> choice2; 

                switch(choice2){
                    case 1:
                        cout << "What would you like to search the book by? (Title/ Author/ ID): "; 
                        cin >> key1; 
                        key1 = toUpper(key1); 
                        cout << "Enter the "<< key1 <<": "; 
                        cin.ignore(); 
                        getline(cin, key2); 
                        key2 = toUpper(key2); 
                        searchBooks(key1, key2); 
                        goto loginMenuStudent; 
                        break; 
                    case 2:
                        cout << "Enter the title of the book you want to borrow: "; 
                        cin.ignore(); 
                        getline(cin, titleToBorrow); 
                        titleToBorrow = toUpper(titleToBorrow); 
                        borrowBook(titleToBorrow, username); 
                        goto loginMenuStudent; 
                        break; 
                    case 3:
                        cout << "Enter the title of the book you want to return: "; 
                        cin.ignore(); 
                        getline(cin, titleToReturn); 
                        titleToReturn = toUpper(titleToReturn);  
                        returnBook(titleToReturn, username); 
                        goto loginMenuStudent; 
                        break; 
                    case 4:
                        cout << "Overdue books: " << endl; 
                        displayOverdueBooks(username); 
                        cout << "-------------------------" << endl; 
                        cout << "Borrowed books: "<< endl; 
                        displayBooks(username); 
                        cout << "Fine: $"; 
                        displayFine(username); 
                        cout << "Press one to pay fine: "; 
                        cin >> choice3; 
                        switch (choice3)
                        {
                            case 1:
                                payFine(username); 
                                goto loginMenuStudent; 
                                break;
                            
                            default:
                                goto loginMenuStudent; 
                                break;
                        }

                        goto loginMenuStudent; 
                    case 5:
                        goto mainMenu; 
                        break; 
                    default:
                        break; 
                }
            }
        default:
            break; 
    }

   
return 0; 
    
}