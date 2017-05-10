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

vector<Pair> getBookLine(string word);
void getWordLine(Pair outpair,Fifo& sendfifo, string& outMessage);

int main() {
    
    Fifo recfifo(receive_fifo);
  	Fifo sendfifo(send_fifo); 
  	
    string inMessage,outMessage;
    string directory = "";
    vector<Pair> infoVec;
    ifstream infile;
    
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

    cout<< inMessage<<endl;
    
    
    infoVec=getBookLine(inMessage);
    for (unsigned i=0;i<infoVec.size();i++){
    getWordLine(infoVec[i],sendfifo,outMessage);}
}

//finds pair for the word
vector<Pair> getBookLine(string word) {

vector<Pair> tempVector;
Pair tempLinePair;
int pos;
int path;
//char pathbuffer[4];
string wordFileName = "wordfiles/" + word + ".bin";
ifstream wordbinary(wordFileName.c_str(), ios::in | ios::binary);

while(!wordbinary.eof()){
wordbinary.read(reinterpret_cast<char *>(&pos), sizeof(int));
wordbinary.read(reinterpret_cast<char *>(&path), sizeof(int));

cout << "Templinepair position: " << pos << " path : " << path << endl; 
tempLinePair.bookIndex=pos;
tempLinePair.position=path;

tempVector.push_back(tempLinePair);}

return tempVector;

}

void getWordLine(Pair outpair,Fifo& sendfifo, string& outMessage) {
cout << "started getWordLine" << endl;
cout << bookDirec[outpair.bookIndex] << endl;
string bookpath = bookDirec[outpair.bookIndex];
int linepos = outpair.position;
string line;

outMessage="bookpath: "+bookpath;
sendfifo.send(outMessage);
cout << outMessage << endl;

ifstream lineout;
lineout.open(bookpath.c_str());
cout << "opened lineout" << endl;

lineout.seekg(linepos,lineout.beg);
getline(lineout,line);

outMessage=line;
sendfifo.send(outMessage);
cout << outMessage << endl;

}
