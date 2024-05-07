#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>

// TIE-02201 Programming 2: Basics - Summer 2020

// The program models a typical data structure of a library
// with certain algorithms for finding books and checking if they are available

// Suvi Suominen, op_nro: 284432, e-mail: suvi.suominen@tuni.fi
// id: suomines


using namespace std;

struct Book{
    string title;
    string author;
    int reservations;
};

bool compare_authors_alphabetically(const Book &a, const Book &b){
    // function helps to sort the books alphabetically by author

    return (a.author < b.author);

}

bool compare_titles_alphabetically (const Book &a, const Book &b){
    // helps to sort the books alphabetically by title

    return a.title < b.title;
}

bool compare_reservations (const Book &a, const Book &b){
    // helps to sort the books by the smallest reservation queue
    return a.reservations < b.reservations;
}

vector<string> split(string s, const char delimiter){
    // function splits the given line of a file into parts
    // by the given delimiter and return the split text as a vector

    // function also checks for empty queues and fields in the file

    vector<string> result;
    string line = s;

    if (line == ""){
        cout << "Error: empty field" << endl;
        return result = {};

    }else{

        while(line.find(delimiter) != string::npos){

            string new_part = line.substr(0, line.find(delimiter));
            line = line.substr(line.find(delimiter)+1, line.size());

            if(new_part.empty()){
                // returning an empty vector if there's an erroneous line
                // in the file

                cout << "Error: empty field" << endl;
                return result = {};

            }else{
                result.push_back(new_part);
            }

        }// adding the final part after the last delimiter
        // and checking that it's not missing
        if (line != ""){
            result.push_back(line);
            return result;

        }else{
            cout << "Error: empty field" << endl;
            return result = {};
        }
    }
}

vector<Book> create_book(vector<string> splitted, vector<Book>& books){
    // creating a new book and adding it to the existing vector "books"
    // that contains all of the books of any one library

    string author = splitted.at(1);
    string title = splitted.at(2);

    if (splitted.at(3) == "on-the-shelf"){
        // if the book is available, there are no reservations

        int reservations = 0;
        Book new_book {title, author, reservations};
        books.push_back(new_book);

    }else{

        int reservations = stoi(splitted.at(3));
        Book new_book {title, author, reservations};
        books.push_back(new_book);

    }
    return books;
}

vector<Book> loanable(const map <string, vector<Book>> libraries){
    // checks for any book with the reservations value of zero,
    // which means the book is on the shelf
    // and returns these books in a vector

    vector<Book> no_reservations;
    map <string, vector<Book>> l = libraries;

    map<string, vector<Book>>::iterator i;
    i = l.begin();

    while (i != l.end()){
        // iterating over the libraries-map
        // and then going through the books-vector one by one

        vector<Book> books = i->second;

        for (auto book : books){

            if (book.reservations == 0){
                no_reservations.push_back(book);
            }
        }++i;
    }
    return no_reservations;
}

void books_by_library_and_author (vector<Book> books, string author){
    // checking that the given author exists and printing all the books
    // by the author in the collection of the given library

    vector<Book> b = books;
    vector<string> authors;
    string a = author;

    for (auto book : b){
        authors.push_back(book.author);

    }vector<string>::iterator i = find(authors.begin(), authors.end(), a);
    if (i == authors.end()){
        cout << "Error: unknown author" << endl;

    }else{
        for (auto book: b){
            if (book.author == a){
                cout << book.title << " --- ";

                if (book.reservations != 0){
                    cout << book.reservations
                         << " reservations" <<endl;
                }else{
                    cout << "on the shelf" <<endl;
                }
            }
        }
    }
}

string strip_quotation_marks (string s){
    // removing the possible quotation marks from the title

    string title = s;

    title.erase(remove( title.begin(), title.end(), '\"' ),
                title.end());

    return title;
}

void reservable (string b, const map <string, vector<Book>> libraries){
    // checking if the wanted book exists in the library and if so,
    // finding the libraries with the shortest reservation queue
    // and printing the reservation value as well as the libraries where
    // the book can be found with this queue number

    map <string, vector<Book>> l = libraries;
    string title = b;
    vector<Book> titles;

    map<string, vector<Book>>::iterator i;
    i = l.begin();

    for (auto library : l){

        vector<Book> books = library.second;

        for (auto book : books){

            if (title == book.title){
                titles.push_back(book);
            }
        }
    }vector<Book>::size_type len = titles.size();
    if (len == 0){
        cout << "Book is not a library book." << endl;

    }else{
        // sorting the books by the reservations from smalles to largest
        // finding the smallest reservartion (the first element) and checking
        // if there's multiples of the same book with the same reservation value

        sort(titles.begin(), titles.end(), compare_reservations);
        int smallest_reservation = titles[0].reservations;

        if (smallest_reservation >= 100){
            cout << "The book is not reservable from any library." << endl;

        }else{
            if (smallest_reservation != 0){
                cout << smallest_reservation << " reservations " << endl;

            }else{
                cout << "on the shelf" << endl;

            }
            for (auto l : libraries){
                vector <Book> books = l.second;
                for (auto b : books){
                    if (b.title == title &&
                            b.reservations == smallest_reservation){
                        cout << "--- " << l.first << endl;
                    }
                }
            }
        }
    }
}



int main()
{

    map <string, vector<Book>> libraries;
    // initiating the data structure: key is the library name
    // and the vector contains all of the books of this library in struct form

    string filename;

    cout << "Input file: ";
    getline(cin, filename);

    ifstream file(filename);
    if (not file){

        cout << "Error: the input file cannot be opened" << endl;
        return EXIT_FAILURE;

    }else{

        string line;
        vector<string> splitted;

        while (getline(file, line)){


            splitted = split(line, ';');

            if (splitted.empty()){
                // stopping the program if the function found
                // an empty field in the file and therefore returns
                // an empty vector

                return EXIT_FAILURE;

            }else{

                string library_name = splitted.at(0);

                if (libraries.find(library_name) == libraries.end()){
                    // if the library is not already added to the map
                    // we create a new vector for the library books

                    vector<Book> books;
                    create_book(splitted, books);

                    libraries.insert({ library_name, books });

                }else{

                    create_book(splitted, libraries.at(library_name));
                    // the "books"-vector already exists this just
                    // adds the new book to the vector
                }
            }
        }
        file.close();
        // the file is read and data structure ready, moving to UI

        while (true){

            string input;
            cout << "> ";
            getline(cin, input);

            if (input == "quit"){
                return EXIT_SUCCESS;


            }else if (input == "libraries"){

                for (auto l : libraries){
                    cout << l.first << endl;
                }

            }else if (input == "loanable"){

                vector<Book> on_shelf = loanable(libraries);
                sort(on_shelf.begin(),on_shelf.end(), compare_titles_alphabetically);
                stable_sort(on_shelf.begin(), on_shelf.end(),
                            compare_authors_alphabetically);

                for (auto b : on_shelf){
                    cout << b.author << ": " << b.title << endl;
                }
            }

            else{

                vector<string> split_input = split(input, ' ');
                // splitting the input to handle the commands containing
                // multiple words


                if (split_input[0] == "material"){

                    if (libraries.find(split_input[1]) == libraries.end()){
                        cout << "Error: unknown library name" << endl;

                    }else{

                        vector<Book> books = libraries.at(split_input[1]);
                        sort(books.begin(), books.end(), compare_titles_alphabetically);
                        stable_sort(books.begin(), books.end(),
                                    compare_authors_alphabetically);

                        for (auto book : books){
                            cout << book.author << ": " << book.title << endl;
                        }
                    }


                }else if (split_input[0] == "books"){

                    if (split_input.size() != 3){
                        cout << "Error: error in command books" << endl;

                    }else if (libraries.find(split_input[1]) == libraries.end()){
                        cout << "Error: unknown library name" << endl;

                    }else{

                        vector<Book> books = libraries.at(split_input[1]);
                        sort(books.begin(), books.end(), compare_titles_alphabetically);

                        string author = split_input[2];
                        books_by_library_and_author(books, author);
                    }

                }else if (split_input[0] == "reservable"){

                    string::size_type i = split_input[0].length();
                    input = input.substr(i+1);
                    // removing the word "reservable" from the input

                    string title = strip_quotation_marks(input);

                    reservable(title, libraries);


                }else{
                    cout << "Error: Unknown command: " << split_input[0] << endl;
                }
            }
        }
    }
}
