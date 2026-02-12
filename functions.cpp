#include <iostream>
#include<fstream>
#include "library.hpp"
#include "json.hpp"
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include "picosha2.h"

using namespace std;
using json = nlohmann::json; 

string Book::getTitle(){
    return title; 
}

string Book::getAuthor(){
    return author; 
}

string Book::getId(){
    return id; 
}

int Book::getQuantity(){
    return quantity; 
}

int Book::getAvailableCopies(){
    return availableCopies; 
}

 void Book::saveBookLocally(){ // save books detail locally to be added to json file later
            cout << "Enter title: "; 
            cin.ignore(); 
            getline(cin, title); 
            title = toUpper(title); 
            cout << "Enter the name of the author: "; 
            getline(cin, author); 
            author = toUpper(author); 
            cout << "Enter id: "; 
            cin >> id; 
            cout << "Enter quantity ";
            cin >> quantity; 
            cout << "Enter the available copies "; 
            cin >> availableCopies; 
        }

void saveBooktoJson(Book book1){
    int bookNo = 1; 
    json data; 
    ifstream file("library_data.json"); 
    if(file.peek() != ifstream::traits_type::eof()){
        file >> data; 
    }


    data["Books"][book1.getTitle()] = {
        {"Author", book1.getAuthor()}, 
        {"Id", book1.getId()}, 
        {"Quantity", book1.getQuantity()}, 
        {"Available Copies", book1.getAvailableCopies()} 
    }; 

    ofstream output("library_data.json"); 
    output << data.dump(4); 
}

void viewAllBooks(){
    ifstream file("library_data.json"); 
    json data; 

    if(file.peek() != ifstream::traits_type::eof()){
        file >> data; 
    }

    for(auto it = data["Books"].begin(); it!=data["Books"].end(); it++){
        json& currentBook = it.value(); 

        cout << "-----------------------------------------------" << endl; 
        cout << "Title: " << it.key() << endl; 
        cout << "Author: " << currentBook["Author"] << endl; 
        cout << "Id: " << currentBook["Id"] << endl; 
        cout << "Quantity: " << currentBook["Quantity"] << endl; 
        cout << "Available Copies: " << currentBook["Available Copies"] << endl; 

    }
}

void searchBooks(string key1, string key2){ // key1 = what you are searching book by // key 2 = details of the book
    ifstream file("library_data.json"); 
    json data; 
    file >> data; 
    bool found; 

    for(auto it = data["Books"].begin(); it!= data["Books"].end(); it++){
        json& currentBook = it.value(); 
        if(currentBook[key1] == key2 || (key1 == "Title" && it.key() == key2)){
            cout << "-----------------------------------------------" << endl; 
            cout << "Title: " << it.key() << endl; 
            cout << "Author: " << currentBook["Author"] << endl; 
            cout << "Id: " << currentBook["Id"] << endl; 
            cout << "Quantity: " << currentBook["Quantity"] << endl; 
            cout << "Available Copies: " << currentBook["Available Copies"] << endl; 
            found = true; 
        }
    }
    if(found == false){
        cout << "Book not found" << endl; 
    }
}

string toUpper(string s){
    
    while(s[0] == ' '){
        s.erase(0, 1); 
    }

    int length = s.size()-1; 
    while(s[length] == ' '){
        s.pop_back(); 
        length--; 
    }

    string u=s; 
    for(int i=0; i<s.size(); i++){
        if(i==0){
            u[i] = toupper(s[i]); 
        }
        else if(s[i-1] == ' '){
            u[i] = toupper(s[i]); 
        }
        else{
            u[i] = tolower(s[i]); 
        }
    }
    return u; 
}

//generate salt for pasword hashing
string generateSalt(){
    const string characters = "abcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()"; 
    string salt; 

    for(int i=0; i<10; i++){
        salt+= characters[rand()% characters.length()]; 
    }
    return salt; 
}

//hash the password obtained from the user
string hashPassword(string password, string salt ){
    return picosha2::hash256_hex_string(password + salt);
}

//checking if a given username exists
bool checkUsername(string username){
    ifstream file("library_data.json"); 
    json data; 
    file >> data; 

    for(auto it=data["User Info"].begin(); it!= data["User Info"].end(); it++){
        json& currentUser = it.value(); 

        if(it.key()== username) return true; 
    }
    return false; 
}

void createAccount(string username, string userType, string password){
    string salt = generateSalt(); 
    string hashedPassword = hashPassword(password, salt);

     ifstream file("library_data.json"); 
     json data; 
     file >> data; 

     data["User Info"][username] = {
        {"Salt", salt}, 
        {"Hashed Password", hashedPassword}, 
        {"User Type", toUpper(userType)}, 
     };
    
    ofstream output("library_data.json"); 
    output << data.dump(4); 
}

string LogIn(string username, string password){
    string salt; 

    ifstream file("library_data.json"); 
    json data; 
    file>> data; 

    bool found = false; 

    string userType; 

    for(auto it=data["User Info"].begin(); it!= data["User Info"].end(); it++){
        json& currentUser = it.value(); 

        if(it.key() == username){
            salt = currentUser["Salt"]; 
            if(hashPassword(password,salt) == currentUser["Hashed Password"]){
                cout << "Welcome " << username << endl; 
                found = true; 
                userType = currentUser["User Type"]; 
            }
            else{
                cout << "Incorrect password." << endl; 
                found = true; 
            }
        }

    }
    if(found == false){
        cout << "Username does not exist. " << endl; 
    }
    return userType; 
}

/*tm* getDayandTime(){
    time_t now = time(nullptr); 
    tm* localTime = localtime(&now); 

    return localTime; 
}*/


void borrowBook(string title, string username){
    bool bookPresent = false; 
    ifstream file("library_data.json"); 
    json data; 
    file>>data; 

    for(auto it = data["Books"].begin(); it!=data["Books"].end(); it++){
        json& currentBook = it.value(); 
        if(it.key() == title){
            bookPresent = true; 
            int availableCopies = currentBook.value("Available Copies", 0); 
            if(availableCopies >0){
                availableCopies--; 
                cout << "You have borrowed successfully. " << endl; 
                currentBook["Available Copies"] = availableCopies; 
            }
        }   
    }
    if(bookPresent == false){
            cout << "We do not have that book at the moment. " << endl; 
        }
    else{
        data["Borrowed Books"][username][title] = {
                {"Borrowed"}
            }; 
    }
    
    ofstream output("library_data.json"); 
    output << data.dump(4); 
    
}

void returnBook(string title, string username){
    ifstream file("library_data.json"); 
    json data; 
    file>>data; 
    bool found = false; 

    for(auto it = data["Borrowed Books"][username].begin(); it!=data["Borrowed Books"][username].end(); it++){
        json& currentBook = it.value(); 

        if(it.key()==title){
            found = true; 
        }
    }
    if(found == false){
            cout << "You have not borrowed that book. " << endl; 
            return;  
        }
    else{
        for(auto it = data["Books"].begin(); it!=data["Books"].end(); it++){
            json& currentBook = it.value(); 
            if(it.key() == title){
                int availableCopies = currentBook.value("Available Copies", 0); 
                    availableCopies++; 
                    cout << "You have returned successfully. " << endl; 
                    currentBook["Available Copies"] = availableCopies; 
                }
            }   
    
        data["Borrowed Books"][username].erase(title); 
    }
    ofstream output("library_data.json"); 
    output << data.dump(4); 
}
   
    
