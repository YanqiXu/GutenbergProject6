#define _GLIBCXX_USE_CXX11_ABI 0
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <utility>
// Include stemming stuff
#include "english_stem.h"
#include "BookDirec.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <algorithm>
#include <cstdlib>
#include <map>

using namespace std;


void oneMap(vector<string> bookDir, map<string, vector <Pair> >& refs, map<string,bool> stopwords, int& start, int& fileIndex);

void writeBinary();
//void openFile(string fileName, string outfile);

Pair getBookLine(string word);
string getWordLine(Pair outpair);


int main()
{
vector<Pair> pairsofint;
int start = 0;
int fileIndex = 0;
  string word;
  string directory = "";
  ifstream infile;
  
  stemming::english_stem<char, std::char_traits<char> > StemEnglish;
  
  StopWordMap(infile, stopwords);
  
  ProcessDirectory(directory,bookDirec);
  
   
  for (int i=0; i<2; i++)
  {
      oneMap(bookDirec,refs,stopwords,start,fileIndex);
  }  
  cout << "Choose Word to search for: ";
  cin >> word;
  // Convert to lower case
  transform(word.begin(), word.end(), word.begin(), ::tolower);
  StemEnglish(word);
  //vector<int> wordpos;
  cout << "right before refs.at(word) " << endl;
  cout << word << endl;
  pairsofint=refs.at(word);
  cout << "right after " << endl;
  cout<<pairsofint[0].bookIndex<<endl;
  cout<<pairsofint[0].position<<endl;
  cout << bookDirec[pairsofint[0].bookIndex]<<endl;
  //wordpos=pairsofint[0].positions;
  //cout<< wordpos[0]<<endl;
//fstream testfile(bookDirec[pairsofint[0].bookIndex])
//GotoLine(bookDirec[pairsofint[0].bookIndex], pairsofint[0].position);
//string testline;
//testfile >> testline;
//cout << testline << endl;
Pair outpair = getBookLine(word);
cout << outpair.position << endl;
cout << outpair.bookIndex << endl;
cout << getWordLine(outpair) << endl;
  cout << "DONE!" << endl;
  return 0;
}

//returns line of word in the book
string getWordLine(Pair outpair) {
cout << "started getWordLine" << endl;
cout << bookDirec[outpair.bookIndex] << endl;
string bookpath = bookDirec[outpair.bookIndex];
cout << "bookpath: " << bookpath << endl;
int linepos = outpair.position;
ifstream lineout;
lineout.open(bookpath.c_str());
cout << "opened lineout" << endl;
char output[100];
lineout >> output;
string finalOutput = output;
return finalOutput;
}

//finds pair for the word
Pair getBookLine(string word) {


Pair tempLinePair;
//char pathbuffer[4];
string wordFileName = "/wordfiles/" + word + ".txt";
ifstream wordbinary(wordFileName.c_str(), ios::in | ios::binary);
wordbinary.read(reinterpret_cast<char *>(&tempLinePair), sizeof(tempLinePair));

//char posbuffer[4];
//string wordFilePos = "/wordfiles/" + word + ".txt";
//wordbinary(wordFileName.c_str(), ios::in | ios::binary);
//wordbinary.read(tempLinePair.position, 4);

//tempLinePair.position= posbuffer;
//tempLinePair.bookIndex = pathbuffer;

cout << "Templinepair position: " << tempLinePair.position << " path : " << tempLinePair.bookIndex << endl; 
return tempLinePair;

}




void oneMap(vector<string> bookDir, map<string, vector <Pair> >& refs, map<string,bool> stopwords, int& start, int& fileIndex) {
  
  ifstream infile;
  int count = 0;
  string line,w,filePath;
  Pair index;
  vector<int> temppos;
  
  stemming::english_stem<char, std::char_traits<char> > StemEnglish;
  
  
  for (int i=fileIndex;i<2500+fileIndex;i++) {
  
  index.bookIndex=fileIndex;
  filePath=bookDir[fileIndex];
  cout<<filePath<<"##"<<endl;
  try{
  
    infile.open(filePath.c_str());
    infile.seekg(start);
    getline(infile,line);
    cout<<line<<endl;
    
    while(!infile.eof()){
      // normalize to lower case
      while (line.length()>0) 
      { 
	   w = getNext(line);
	   
	   count++;
	   if (count==2500){
		cout << "found first 25,000,000 words" << endl;
	   fileIndex=i;
	   start=infile.tellg();
	   
	   infile.eof();
	   writeBinary();
	   fileIndex=i;
	   
	   return; 

	   }
	   else{
	   transform(w.begin(), w.end(), w.begin(), ::tolower);
	   
	   
	   if (stopwords.find(w)==stopwords.end()){
			if (w.length()<1) break;
	   		//temppos=index.positions;
	   		//temppos.push_back(infile.tellg());
	   		index.position=infile.tellg();
			StemEnglish(w);
	   		//cout<<infile.tellg()<<" is line number of the word "<< w << " word number: " << count << " should = " << i <<endl;
			//cout << "Position of index = " << index.position << " path = " << index.bookIndex << endl;
	   		refs[w].push_back(index);
	   		}
	   }
      }
    getline(infile,line);
    //cout << endl << line << endl;
    }

    infile.close();
  }catch(ifstream::failure e){
cout << "failed" << endl;
    //cout<<e<<endl;
  }
  }
}

void writeBinary() {
int itCount = 0;
cout << "write binary started";
for(map<string,vector <Pair> >::iterator it = refs.begin(); it != refs.end(); it++) {
for (vector<Pair>::iterator vecIt = it->second.begin(); vecIt != it->second.end(); vecIt++) {
string fileName = "wordfiles/" + it->first + ".txt";
//openFile(fileName, "outfile");
cout << "Writing binary for " << it->first << "    Count: " << itCount << endl;
Pair tempPair = it->second[0];
int pairpos = tempPair.position; 
int pairpath = tempPair.bookIndex;
ofstream outfile(fileName.c_str(), ios::out | ios::binary);
outfile.write (reinterpret_cast<const char *>(&pairpath), sizeof(pairpath));
outfile.write (reinterpret_cast<const char *>(&pairpos), sizeof(pairpath));
itCount++;
}
}
return;
}


//void openFile(string fileName, string outfile) {
//.outfile.open(fileName, std::fstream::in | std::fstream::out | std::fstream::app);
//if (!fileName) {
//outfile.open(fileName, std::fstream::in | std::fstream::out | std::fstream::trunc);
//}
//return;
//}


