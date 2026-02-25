#include <iostream>
#include<fstream>
#include "library.hpp"
#include "json.hpp"
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include "picosha2.h"
#include "time.h"

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

double getDate(){
    time_t now = time(nullptr); 
    return static_cast<double>(now/86400); 
}

void borrowBook(string title, string username){
    bool bookPresent = false; 
    ifstream file("library_data.json"); 
    json data; 
    file>>data; 

    if(data["Borrowed Books"][username].size() >= 3){
        cout << "You have already borrowed 3 books. Please return one or more to borrow new books " << endl; 
        return;
    }; 
    

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
                {"Date Borrowed", getDate()}
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

vector<string> overdueBooks(string username){
    ifstream file("library_data.json"); 
    json data; 
    file >> data; 
    vector<string> bookList = {}; 

    for(auto it = data["Borrowed Books"][username].begin(); it!= data["Borrowed Books"][username].end(); it++){
        json& currentUser = it.value(); 
 
        if(getDate()-currentUser["Date Borrowed"].get<double>() > 14){
            bookList.push_back(it.key()); 
        }
    }
    return bookList; 
}

void displayOverdueBooks(string username){
    vector <string> bookList = overdueBooks(username); 

    for(int i=0; i<bookList.size(); i++){
        cout << (i+1) <<". " << bookList[i] << endl; 
    }
}

void displayBooks(string username){
    ifstream file("library_data.json"); 
    json data; 
    file >> data; 
    int i = 1; 
    for(auto it= data["Borrowed Books"][username].begin(); it!= data["Borrowed Books"][username].end(); it++){
        cout << i << ". " << it.key() << endl; 
        i++; 
    }
}

double calculateFine(string username){
    ifstream file("library_data.json"); 
    json data; 
    file >> data; 
    double fine = 0; 

    cout << setprecision(2) << fixed; 

    for(auto it = data["Borrowed Books"][username].begin(); it!= data["Borrowed Books"][username].end(); it++){
        json& currentUser = it.value(); 

        if(getDate()-currentUser["Date Borrowed"].get<double>() > 14){
            fine +=  (getDate()-currentUser["Date Borrowed"].get<double>()-14)*2; 
        }
    }

    data["Fines"][username] = {
        {"Fine", fine}
    }; 

    ofstream output("library_data.json"); 
    output << data.dump(4); 
    
    return fine; 
}

void displayFine(string username){
    ifstream file("library_data.json"); 
    json data; 
    file >> data; 

    for(auto it= data["Fines"].begin(); it!= data["Fines"].end(); it++){
        json& currentUser = it.value(); 
        if(it.key() == username){
            cout << currentUser["Fine"].get<double>() << endl; 
        }
    }
}

void payFine(string username){
    ifstream file("library_data.json"); 
    json data; 
    file >> data; 

    for(auto it= data["Fines"].begin(); it!= data["Fines"].end(); it++){
        json& currentUser = it.value(); 
        
       
        if(it.key() == username){
            currentUser["Fine"] = 0;  
        }
    }

    for(auto it = data["Borrowed Books"][username].begin(); it!= data["Borrowed Books"][username].end(); it++){
        json& currentUser = it.value(); 

        currentUser["Date Borrowed"] = getDate(); 
       
    }
    ofstream output("library_data.json"); 
    output << data.dump(4); 

    cout << "Fine has been paid and your borrowal has been renewed." << endl; 

}

void deleteBook(){
    cout << "Enter the title of Book you want to delete: "; 
    cin.ignore(); 
    string title; 
    getline(cin, title); 
    title = toUpper(title); 


    ifstream file("library_data.json"); 
    json data; 
    file >> data;

    data["Books"].erase(title); 
    ofstream output("library_data.json"); 
    output << data.dump(4); 
}

