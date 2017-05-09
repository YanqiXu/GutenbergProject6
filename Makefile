#MakeFile to build and deploy the Sample US CENSUS Name Data using ajax
# For CSC3004 Software Development

# Put your user name below:
USER= xuy

CC= g++

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -g

RM= /bin/rm -f

all: SearchBooks SearchBooks PutCGI PutHTML
#all: SearchBooks 


SearchBooks.o: SearchBooks.cpp fifo.h BookDirec.h
	$(CC) -c $(CFLAGS) SearchBooks.cpp

SearchAjax.o: SearchAjax.cpp fifo.h 
	$(CC) -c $(CFLAGS) SearchAjax.cpp

SearchBooks: SearchBooks.o fifo.o
	$(CC) SearchBooks.o  fifo.o -o SearchBooks

fifo.o:		fifo.cpp fifo.h
		g++ -c fifo.cpp
SearchAjax: SearchAjax.o  fifo.h
	$(CC) SearchAjax.o  fifo.o -o SearchAjax -L/usr/local/lib -lcgicc

PutCGI: SearchAjax
	chmod 757 SearchAjax
	cp SearchAjax /usr/lib/cgi-bin/$(USER)_SearchBooks.cgi 

	echo "Current contents of your cgi-bin directory: "
	ls -l /usr/lib/cgi-bin/

PutHTML:
	cp shake_ajax.html /var/www/html/class/softdev/$(USER)
	cp shake_ajax.css /var/www/html/class/softdev/$(USER)

	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/class/softdev/$(USER)

clean:
	rm -f *.o shake_ajax shakeserver testclient
