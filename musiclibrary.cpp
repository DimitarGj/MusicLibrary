/// Dimitar Gjorgievski, UIN 650730211
/// CS 251
/// Project 2 - Music Library
///
/// Assignment details and provided code are created and
/// owned by Adam T Koehler, PhD - Copyright 2023.
/// University of Illinois Chicago - CS 251 Spring 2023

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <fstream>

using namespace std;

// INSERTS GIVEN HELPERS CODE BELOW - DO NOT REMOVE
#include "helpers.cpp"
// INSERTS GIVEN HELPERS CODE ABOVE - DO NOT REMOVE

//
// NOTE TO STUDENTS:
// You may use the functions in the helpers file,
// but you may not modify the file or its contents in any way whatsoever.
//

//
// STUDENT CODE MAY GO BELOW THIS LINE
//

//Helper function that searches for corresponding words in albums/songs that match with terms being searched
int CheckForTerm(string checking, string to_check){
    string check; //Stores parsed string of word to be checked with size of word we are checking for
    tolower(checking); 
    tolower(to_check);
    int size = to_check.size(); //Holds size of word that is to be checked
    
    for(int i=0; i<size; i++){
        if(to_check[i]==checking[0]){ //If the first letter of word that is being checked corresponds with first letter of word we are checking for
            check = to_check.substr(i, checking.size()); //Parse the same amount of letters that are in the word we are checking for
            if(checking==check){
                return checking.size(); //return size of word if corresponding term is found
            }
        }
    }
    return 0;
}

//Helper function to find album of corresponding artist
bool SearchArtist_NoModifier(string artist_name, set<string>& search_terms){
    for(string name: search_terms){ 
        if(artist_name==name){ //Chceking if a parsed first name from set value corresponds with the one of the names taken from remains string
            return true;
        }
    }
    return false;
}

//Function that searches for artist
void SearchArtist(string& remains, set<string>& artists, map<string, set<string>>& a_artist){
    set<string> search_terms; //Holds the search terms in remains
    int index; //Index used for parsing remains
    string modifier = "N/A";
    string name;
    string lastname;
    set<string> found_albums; 
    set<string> intersect; //Holds common albums found from term without a modifier and term with + modifier

    index = remains.find(" ");  
    name = remains.substr(0, index); //Parsing name from remains string
    name[0] = toupper(name[0]);
    search_terms.insert(name);
    remains.erase(0, index+1);

    while(index!=-1){
        index = remains.find(" ");
        
        if(remains[0]=='-'){ //When - modifier is detected
            modifier = remains.substr(0, 1); //Storing modifier sign
            remains.erase(0, 1); 
            search_terms.erase(name); //Erasing first name to replace element with concatenated last name    
            lastname = remains.substr(0, index);
            lastname[0] = toupper(lastname[0]);
            name = name + " " + lastname;     
            if(name[name.size()-1]==' '){
                name.erase(name.size()-1, name.size()); //Deletes unwanted space at the end of string
            }

        }else if(remains[0]=='+'){ //When + modifier is detected            
            modifier = remains.substr(0, 1); //Storing modifier sign
            remains.erase(0, 1); 
            search_terms.erase(name); //Erasing first name to replace element with concatenated last name       
            lastname = remains.substr(0, index);
            lastname[0] = toupper(lastname[0]);
            name = name + " " + lastname;    
            if(name[name.size()-1]==' '){
                name.erase(name.size()-1, name.size()); //Deletes unwanted space at the end of string
            } 

        }else{ //If terms are multiple names with no modifiers
            name = remains.substr(0, index);
            name[0] = toupper(name[0]);
        }
      
        search_terms.insert(name);
        remains.erase(0, index+1);
    }

    if(modifier == "+"){ //When modifier sign is +
        for(auto key: a_artist){
            set<string> artist_search = key.second; 
            set_intersection(artist_search.begin(), artist_search.end(), search_terms.begin(), search_terms.end(), inserter(intersect, intersect.begin())); //Taking set value from map and finding intersection with concatenated name in search_terms
            if(intersect.size()!=0){
                found_albums.insert(key.first); //Storing album if intersection of names is found 
            }
            intersect.clear(); //Clearing interesction to check artists in next album
        }

    }else if(modifier == "-"){ //When modifier sign is -
        index = name.find(" "); 
        string common_name = name.substr(0, index); //Storing common first name
        for(string artist_name: artists){
            if(common_name==artist_name.substr(0, index)){ //If common first name exists
                search_terms.insert(artist_name); //Storing all names with common first name
            }
        }
        
        search_terms.erase(name); //Erasing concatenated name of terms stored from remains string. Only lastnames different from the one in remains string are left
        
        for(auto key: a_artist){
            set<string> artist_search = key.second;
            set_intersection(artist_search.begin(), artist_search.end(), search_terms.begin(), search_terms.end(), inserter(intersect, intersect.begin())); //Taking set value from map and finding intersection with common names in search_terms
            if(intersect.size()!=0){
                found_albums.insert(key.first); //Storing album if intersection of names is found
            }
            intersect.clear(); //Clearing interesction to check artists in next album
        }

    }else{ //If no modifiers were entered
        for(auto key: a_artist){
            set<string> artist_search = key.second;
            for(string artist_name: artist_search){
                index = artist_name.find(" ");
                artist_name = artist_name.substr(0, index); //Taking just the first name
                if(SearchArtist_NoModifier(artist_name, search_terms)){ //Taking artist name from set of a given key album comparing it to artist name taken from remains string 
                    found_albums.insert(key.first);
                }
            } 
        }
    }

    if(found_albums.size()!=0){
        cout<<"Your search results exist in the following albums: "<<endl;
        for(string found: found_albums){
            cout<<found<<endl;
        }
    }else{
        cout<<"Search results don't exist"<<endl;
    }
    
    cout<<endl;
}

//Function that searches for album
void SearchAlbum(string& remains, set<string>& albums){
    set<string> search_terms; //Holds the search terms in remains
    int index; //Index used for parsing remains
    int size; //Stores size of a given term 
    set<string> found_albums; //Holds albums corresponding to searched terms

    index = remains.find(" ");
    search_terms.insert(remains.substr(0, index)); //Parsing the remains string
    remains.erase(0, index+1);

    while(index!=-1){ //Parsing rest of remains string
        index = remains.find(" ");
        search_terms.insert(remains.substr(0, index));
        remains.erase(0, index+1);    
    }

    for(string a_search: search_terms){ //Going through the terms of remains
        size = a_search.size();
        for(string album: albums){
            if(CheckForTerm(a_search, album)==size){  //Check for corresponding term in album
                found_albums.insert(album); //Store album is term corresponds with album
                break;
            }
        }
    }
    
    if(found_albums.size()!=0){
        cout<<"Your search results exist in the following albums: "<<endl;
        for(string found: found_albums){
            cout<<found<<endl;
        }
    }else{
        cout<<"Search results don't exist"<<endl;
    }
    
    cout<<endl;
}

//Function that searches for songs
void SearchSongs(string& remains, map<string, set<string>>& a_songs){
    set<string> plus_modifiers; //Holds terms associated with + modifiers
    set<string> minus_modifiers; //Holds terms associated with - modifiers
    set<string> potential_albums; //Holds set potential albums of result, but not final
    string term; //Holds term with no modifier
    int index; //String parsing index

    index = remains.find(" ");
    term = remains.substr(0, index); //Getting first term
    for(auto key: a_songs){ //Go through every album
        set<string> album = key.second;
        for(string song: album){ //Go through every song
            if(CheckForTerm(term, song)==(int)term.size()){ //Check for corresponding term in song
                potential_albums.insert(key.first); //Insert album in potential set if term is found in a song
            }
        }
    }

    remains.erase(0, index+1);

    while(index!=-1){ //Get rest of terms
        index = remains.find(" ");
        if(remains[0]=='+'){ //When + modifier is detected
            string plus_mod = remains.substr(1, index); //Holds term without + modifier
            if(plus_mod[plus_mod.size()-1]==' '){
                plus_mod.erase(plus_mod.size()-1, plus_mod.size()); //Deletes unwanted space at the end of string
            }
            for(auto key: a_songs){
                set<string> album = key.second;
                for(string song: album){
                    if(CheckForTerm(plus_mod, song)==(int)plus_mod.size()){ //Check for corresponding term in song
                        plus_modifiers.insert(key.first); //Insert album in different set if + modifier term is found in a song
                    }
                }
            }
            remains.erase(0, index+1); 
        
        }else if(remains[0]=='-'){ //When - modifier is detected
            string minus_mod = remains.substr(1, index); //Holds term without - modifier
            if(minus_mod[minus_mod.size()-1]==' '){
                minus_mod.erase(minus_mod.size()-1, minus_mod.size()); //Deletes unwanted space at the end of string
            }
            for(auto key: a_songs){
                set<string> album = key.second;
                for(string song: album){
                    if(CheckForTerm(minus_mod, song)==(int)minus_mod.size()){ //Check for corresponding term in song
                        minus_modifiers.insert(key.first); //Insert album in different set if - modifier term is found in a song
                    }
                }
            }
            remains.erase(0, index+1); 

        }else{ //If additional terms with no modifiers are present
            term = remains.substr(0, index);    
            for(auto key: a_songs){
                set<string> album = key.second;
                for(string song: album){
                    if(CheckForTerm(term, song)==(int)term.size()){ 
                        potential_albums.insert(key.first);
                    }
                }
            }
            remains.erase(0, index+1);
        }
    }
    
    cout<<"Normal Albums:"<<endl;
    for(auto alb: potential_albums){
        cout<<alb<<endl;
    }
    cout<<"+ Albums:"<<endl;
    for(auto alb: plus_modifiers){
        cout<<alb<<endl;
    }
    cout<<"- Albums:"<<endl;
    for(auto alb: minus_modifiers){
        cout<<alb<<endl;
    }

    set<string> intersection; //Hold common albums between potential_albums set and plus_modifier set
    set<string> difference; //Hold albums in potential_albums set but not in minus_modifier set
    set<string> union_set; //Holds albums found stored in intersection set and difference set

    if(minus_modifiers.size()!=0 && plus_modifiers.size()!=0){ //When plus_modifier set and minus_modifier set have albums stored
        set_intersection(potential_albums.begin(), potential_albums.end(), plus_modifiers.begin(), plus_modifiers.end(), inserter(intersection, intersection.begin()));
        set_difference(potential_albums.begin(), potential_albums.end(), minus_modifiers.begin(), minus_modifiers.end(), inserter(difference, difference.begin()));
        set_union(difference.begin(), difference.end(), intersection.begin(), intersection.end(), inserter(union_set, union_set.begin()));
        cout<<"Your search results exist in the following albums: "<<endl;
        for(string found: union_set){
            cout<<found<<endl;
        }
    }else if(plus_modifiers.size()!=0 && minus_modifiers.size()==0){ //When only plus_modifier set has albums stored
        set_intersection(potential_albums.begin(), potential_albums.end(), plus_modifiers.begin(), plus_modifiers.end(), inserter(intersection, intersection.begin()));
        cout<<"Your search results exist in the following albums: "<<endl;
        for(string found: intersection){
            cout<<found<<endl;
        }
    }else if(minus_modifiers.size()!=0 && plus_modifiers.size()==0){ //When only minus_modifier set has albums stored
        set_difference(potential_albums.begin(), potential_albums.end(), minus_modifiers.begin(), minus_modifiers.end(), inserter(difference, difference.begin()));
        cout<<"Your search results exist in the following albums: "<<endl;
        for(string found: difference){
            cout<<found<<endl;
        }
    }else{ //When no modifiers were used
        cout<<"Your search results exist in the following albums: "<<endl;
        for(string found: potential_albums){
            cout<<found<<endl;
        }
    }

    cout<<endl;

}

//Helper function that exports artists to file
void ExportSongs(ofstream& OutF, string& album, map<string, set<string>>& a_songs){

    for (auto albums : a_songs) {
        string key = albums.first;
        if(key==album){
            set<string> songs = albums.second;
            for (auto song : songs) {
                OutF<<song;
                OutF<<endl;
            }
            OutF<<album;
            OutF<<endl;
            break;
        }
    }
}

//Helper function that exports artists to file
void ExportArtists(ofstream& OutF, string& album, map<string, set<string>>& a_artist){

    for (auto albums : a_artist) {
        string key = albums.first;
        if(key==album){
            set<string> artists = albums.second;
            for (auto artist : artists) {
                OutF<<artist;
                OutF<<endl;
            }
            break;
        }
    }
}

//Function that exports playlist data to file
void Export(string& remain, map<string, set<string>>& a_artist, map<string, set<string>>& a_songs, set<string>& artists, set<string>& albums){
    ofstream OutF;
    string file_name;

    if(remain.size()!=0){
        file_name = remain;
    }else{
        file_name = "musicdatabase.txt";
    }

    OutF.open(file_name);

    if(!OutF.is_open()){
        cout<<"Error: Could not open music library file - "<<file_name<<endl;
        return;
    }

    for(string album: albums){
        OutF<<album;
        OutF<<endl;
        ExportArtists(OutF, album, a_artist);
        ExportSongs(OutF, album, a_songs);
    }

    OutF.close();
}

//Function that erases loaded data
void Clear(map<string, set<string>>& a_artist, map<string, set<string>>& a_songs, set<string>& artists, set<string>& albums){
    a_artist.clear();
    a_songs.clear();
    artists.clear();
    albums.clear();
}

//Function that returns number of songs
int TotalSongs(map<string, set<string>>& a_songs){
    int total = 0;
    
    for(auto album: a_songs){
        set<string> songs = album.second;
        for(auto song: songs){
            total++;
        }
    }
    return total;
}

//Helper function that prints songs
void PrintSongs(string& album, map<string, set<string>>& a_songs){

    for (auto albums : a_songs) {
        string key = albums.first;
        if(key==album){
            set<string> songs = albums.second;
            for (auto song : songs) {
                cout <<"   "<< song <<endl;
            }
            break;
        }
    }   
}

//Helper function that prints artists
void PrintArtists(string& album, map<string, set<string>>& a_artist){
    
    for (auto albums : a_artist) {
        string key = albums.first;
        if(key==album){
            set<string> artists = albums.second;
            for (auto artist : artists) {
                cout <<" "<< artist <<endl;
            }
            break;
        }
    }
    
}

//Function that prints stats
void Stats(string& remain, map<string, set<string>>& a_artist, map<string, set<string>>& a_songs, set<string>& artists, set<string>& albums){
    cout<<"Overall Music Library Stats"<<endl;
    cout<<"==========================="<<endl;
    cout<<"Total Records: "<<albums.size()<<endl;
    cout<<"Total Unique Artists: "<<artists.size()<<endl;
    cout<<"Total Songs: "<<TotalSongs(a_songs)<<endl;

    if(remain.size()!=0){
        cout<<endl;
        cout<<"Your Current Music Library Includes"<<endl;
        cout<<"==================================="<<endl;
        
        for(auto alb: albums){
            cout<<alb<<endl;
            PrintArtists(alb, a_artist);
            PrintSongs(alb, a_songs);
        }
    }

    cout<<endl;

}

//Funtion to load data
void Load(string& remain, map<string, set<string>>& a_artist, map<string, set<string>>& a_songs, set<string>& artists, set<string>& albums){
    ifstream InF;
    string file_name = "musicdatabase.txt";
    string file_data;
    string curr_album; //Holds the current album being stored
    int has_artist = 1; //Checks if album has an artist

    if(remain.size()!=0){
        file_name = remain;
    }

    InF.open(file_name);

    if(!InF.is_open()){
        cout<<"Error: Could not open music library file - "<<file_name<<endl;
        return;
    }

    getline(InF, file_data);
    albums.insert(file_data);
    curr_album = file_data;
    getline(InF, file_data);

    while (!InF.eof() && !InF.fail())
    {
        if(file_data[0]=='0' || file_data[0]=='1'){ //Song insertion
            a_songs[curr_album].insert(file_data);
            if(has_artist == 1){
                a_artist[curr_album].insert("N/A"); //Record that the album doesn't have an artist in artist map, but don't record in artists set  
            }
        }else if(file_data == curr_album){ //Indicates next album is about to be inserted
            has_artist = 1; //Artist not yet inserted
            getline(InF, file_data);
            if(file_data.size()!=0){ //To avoid inserting empty space in albums set when encountering last album in file
                albums.insert(file_data);
            }
            curr_album = file_data; //New album being inserted
        }else{
            has_artist = 0; //Artist is inserted
            a_artist[curr_album].insert(file_data);
            artists.insert(file_data);  
        }
        getline(InF, file_data);
    }
    
    InF.close();
}

int main()
{
    string userEntry;
    string command, remains;
    map<string, set<string>> a_artist; //Holds album as key and artist as value
    map<string, set<string>> a_songs; //Holds album as key and songs as value
    set<string> artists;
    set<string> albums; 

    // TODO: Declare any necessary variables for the music libary

    // Display welcome message once per program execution
    cout << "Welcome to the Music Library App" << endl;
    cout << "--------------------------------" << endl;

    // Stay in menu until exit command
    do
    {
        cout << endl;
        cout << "Enter a command (help for help): " << endl;
        getline(cin, userEntry);
        cout << endl;

        // Split the user entry into two pieces
        splitFirstWord(userEntry, command, remains);
        tolower(command);

        // take an action, one per iteration, based on the command
        if (command == "help")
        {
            helpCommand();
        }
        else if (command == "clear")
        {
            Clear(a_artist, a_songs, artists, albums);
        }
        else if (command == "export")
        {
            Export(remains, a_artist, a_songs, artists, albums);
        }
        else if (command == "load")
        {
            Load(remains, a_artist, a_songs, artists, albums);  
        }
        else if (command == "stats")
        {
            Stats(remains, a_artist, a_songs, artists, albums);
        }          
        else if (command == "search")
        {
            if(remains.size()!=0){
                int index = remains.find(" "); 
                string category = remains.substr(0, index); //Holds category of check; artist/album/song
                remains.erase(0, index+1); //Removing category from remains

                if(category=="artist"){
                    SearchArtist(remains, artists, a_artist);
                }else if(category=="album"){
                    SearchAlbum(remains, albums);
                }else{
                   SearchSongs(remains, a_songs);
                } 

            }else{
                cout<<"Error: Search terms cannot be empty."<<endl;
                cout<<"No results found."<<endl;
                cout<<endl;
            }
        }

    }while(command != "exit");    

    cout << "Thank you for using the Music Library App" << endl;
    return 0;
}