#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <map>
#include "fifo.h"
#include <utility>
#include <algorithm>
// Include stemming stuff
#include "english_stem.h"
#include <string>
#include <vector>
#include "BookDirec.h"

using namespace std;

string receive_fifo = "Namerequest";
string send_fifo = "Namereply";

int main() {
 
    int inMessage;
    string directory = "";
  
  	stemming::english_stem<char, std::char_traits<char> > StemEnglish;
  
    StopWordMap(infile, stopwords);
    ProcessDirectory(directory,bookDirec);
    //recfifo.openread();
    //inMessage = recfifo.recv();
    
    cout<<"Search for the word: "<< inMessage<<endl;
    cin >> inMessage;
    
    string saveWord=inMessage;
    //Convert word to lower case
    transform(inMessage.begin(), inMessage.end(), inMessage.begin(), ::tolower);
    //Reduce to Stem
    StemEnglish(inMessage);
    
    string fileName = "wordfiles/" + inMessage + ".txt";
    int x; 
    ifstream infile; 
    infile.open(fileName.c_str(), ios::binary | ios::in) 
    infile.read(&x, 7);
    
    
    
}

