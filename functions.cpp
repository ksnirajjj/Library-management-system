#include <iostream>
#include<fstream>
#include "library.hpp"
#include "json.hpp"
#include <string>
#include <cctype>
#include <cstdlib>
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
            cout << "Enter the name of the author: "; 
            getline(cin, author); 
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


int main(){
    for(int i=0; i<10; i++){
        cout << hashPassword("niraj", "shrestha") << endl; 
    }
}