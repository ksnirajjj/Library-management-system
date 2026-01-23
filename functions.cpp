#include <iostream>
#include<fstream>
#include "library.hpp"
#include "json.hpp"

using namespace std;
using json = nlohmann::json; 

class Book{
    private:
        string title; 
        string author; 
        int id; 
    public:
        void saveBook(){ // save books detail locally to be added to json file later
            cout << "Enter title: "; 
            cin >> title; 
            cout << "Enter the name of the author: "; 
            cin >> author; 
            cout << "Enter id: "; 
            cin >> id; 

        }
}; 

