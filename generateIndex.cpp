
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

void oneMap(vector<string> bookDir, map<string, vector<Pair> >& refs, map<string, bool> stopwords, int& start, int& fileIndex);

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

    ProcessDirectory(directory, bookDirec);

    cout << bookDirec.size();
    //while(fileIndex != bookDirec.size()-1)
    //{
    oneMap(bookDirec, refs, stopwords, start, fileIndex);
    cout << "1 " << fileIndex << endl;
    oneMap(bookDirec, refs, stopwords, start, fileIndex);
    cout << "2 " << fileIndex << endl;
    //oneMap(bookDirec,refs,stopwords,start,fileIndex);
    cout << "3 " << fileIndex << endl;
    //oneMap(bookDirec,refs,stopwords,start,fileIndex);
    cout << "4 " << fileIndex << endl;
    //oneMap(bookDirec,refs,stopwords,start,fileIndex);
    cout << "5 " << fileIndex << endl;
    //oneMap(bookDirec,refs,stopwords,start,fileIndex);
    cout << "6 " << fileIndex << endl;
    //}

    cout << "Choose Word to search for: ";
    cin >> word;
    // Convert to lower case
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    StemEnglish(word);

    cout << "right before refs.at(word) " << endl;
    cout << word << endl;
    pairsofint = refs.at(word);
    cout << "right after " << endl;
    cout << pairsofint[0].bookIndex << endl;
    cout << pairsofint[0].position << endl;
    cout << bookDirec[pairsofint[0].bookIndex] << endl;
    //wordpos=pairsofint[0].positions;
    //cout<< wordpos[0]<<endl;
    //fstream testfile(bookDirec[pairsofint[0].bookIndex])
    //GotoLine(bookDirec[pairsofint[0].bookIndex], pairsofint[0].position);
    //string testline;
    //testfile >> testline;
    //cout << testline << endl;
    /*Pair outpair = getBookLine(word);
cout << outpair.position << endl;
cout << outpair.bookIndex << endl;
cout << getWordLine(outpair) << endl;*/

    /*int x; 
string fileName1 = "wordfiles/" + word + ".txt";
   
    infile.open(fileName1.c_str(), ios::binary | ios::in);
    infile.read(&x, 4); // reads 7 bytes into a cell that is either 2 or 4 
    
  cout << "DONE!" << endl;*/
    return 0;
}

//returns line of word in the book

void oneMap(vector<string> bookDir, map<string, vector<Pair> >& refs, map<string, bool> stopwords, int& start, int& fileIndex)
{

    ifstream infile;
    int count = 0;
    string line, w, filePath;
    Pair index;

    stemming::english_stem<char, std::char_traits<char> > StemEnglish;

    while (count < 2500) {

        filePath = bookDir[fileIndex];
        infile.open(filePath.c_str());
        infile.seekg(start);
        getline(infile, line);
        cout << "**" << line << endl;

        index.bookIndex = fileIndex;
        cout << filePath << "Here" << endl;
        try {

            while (!infile.eof()) {
                // normalize to lower case
                while (line.length() > 0) {

                    w = getNext(line);
                    start = infile.tellg();

                    size_t found = w.find_first_not_of("abcdefghijklmnopqrstuvwxyz ");
                    if (w.length() > 2 && found == string::npos && stopwords.find(w) == stopwords.end()) {
                        count++;

                        transform(w.begin(), w.end(), w.begin(), ::tolower);
                        StemEnglish(w);
                        //temppos=index.positions;
                        //temppos.push_back(infile.tellg());
                        index.position = infile.tellg();

                        cout<<infile.tellg()<<" is line number of the word "<< w << " word number: " << count << endl;
                        //cout << "Position of index = " << index.position << " path = " << index.bookIndex << endl;
                        refs[w].push_back(index);
                    }
					
					
					
					
                    if (count == 25) {
                        cout << "found first 25,000,000 words" << endl;

                        infile.eof();
                        writeBinary();

                        return;
                    }
                }
                getline(infile, line);
                //cout << endl << line << endl;
            }

            infile.close();
        }
        catch (ifstream::failure e) {
            cout << "failed" << endl;
            //cout<<e<<endl;
        }
        fileIndex++;
    }
}

void writeBinary()
{
    int itCount = 0;
    cout << "write binary started";
    for (map<string, vector<Pair> >::iterator it = refs.begin(); it != refs.end(); it++) {
    	string fileName = "wordfiles/" + it->first + ".bin";
    	fstream outfile(fileName.c_str(), ios::out | ios::binary | fstream::out | fstream::app);
    	cout << "Word:" << it->first << endl;
        for (vector<Pair>::iterator vecIt = it->second.begin(); vecIt != it->second.end(); vecIt++) {
            
            //openFile(fileName, "outfile");
            //cout << "Writing binary for " << it->first << "    Count: " << itCount << endl;
            Pair tempPair = it->second[0];
            int pairpos = tempPair.position;
            int pairpath = tempPair.bookIndex;
            
            outfile.write(reinterpret_cast<const char*>(&pairpath), sizeof(pairpath));
            outfile.write(reinterpret_cast<const char*>(&pairpos), sizeof(pairpath));
            int pos = outfile.tellg();
            cout << ":" << pos << ":" << pairpath << ":" <<pairpos << endl;
            itCount++;
        }
        outfile.close();
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


