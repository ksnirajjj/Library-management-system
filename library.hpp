#ifndef library_hpp
#define library_hpp

#include <string>
using namespace std; 

class Book{
    private:
        string title; 
        string author; 
        string id; 
        int quantity; 
        int availableCopies; 
    public:
        string getTitle(); 
        string getAuthor(); 
        string getId(); 
        int getQuantity(); 
        int getAvailableCopies(); 
        void saveBookLocally(); 

}; 

void saveBooktoJson(Book book1); 

void viewAllBooks(); 

void searchBooks(string key1, string key2); 

string toUpper(string s); 
#endif