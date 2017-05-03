#MakeFile to build and deploy SearchBooks
# For CSC3004 Software Development

# Put your user name below:
USER= xuy

CC= g++

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -std=c++11

RM= /bin/rm -f

all: SearchBooks PutCGI PutHTML

SearchBooks.o: SearchBooks.cpp 
	$(CC) -c $(CFLAGS) SearchBooks.cpp

SearchBooks: SearchBooks.o
	$(CC) $(CFLAGS) SearchBooks.o -o SearchBooks -L/usr/local/lib -lcgicc

testSearch.o: testSearch.cpp 
	$(CC) -c testSearch.cpp 

testSearch: testSearch.o 
	$(CC) testSearch.o -o testSearch 

PutCGI: SearchBooks
	chmod 757 SearchBooks
	cp SearchBooks /usr/lib/cgi-bin/$(USER)_SearchBooks.cgi 

	echo "Current contents of your cgi-bin directory: "
	ls -l /usr/lib/cgi-bin/

PutHTML:
	cp SearchBooks.html /var/www/html/class/softdev/$(USER)
	cp SearchBooks.js /var/www/html/class/softdev/$(USER)
	cp SearchBooks.css /var/www/html/class/softdev/$(USER)
	cp gears.gif /var/www/html/class/softdev/$(USER)

	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/class/softdev/$(USER)

clean:
	rm -f *.o SearchBooks testSearch
