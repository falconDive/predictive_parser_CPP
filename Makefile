parser: parser.cc lexer.cc inputbuf.cc 
	g++ lexer.cc inputbuf.cc parser.cc -o parser

clean: rm *.o
