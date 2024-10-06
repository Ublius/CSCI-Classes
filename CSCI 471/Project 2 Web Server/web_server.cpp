// **************************************************************************************
// * Web Server (web_s.cc)
// * -- Accepts TCP connections and serves a web page.
// **************************************************************************************
#include "web_server.h"
#include <regex>
#include <fstream>
#include <sys/stat.h>

std::string filename;

// **************************************************************************************
// * processConnection()
// * - Handles reading the request from the client and sending back an appropriate
// * - response, always returns 0
// **************************************************************************************
int processConnection(int sockFd) {
  DEBUG << "Reading request" << ENDL;
  int returnCode = readRequest(sockFd);
  DEBUG << "Got return code " << returnCode << ENDL;
  switch (returnCode) {
  case 400:
    DEBUG << "Sending 400 error" << ENDL;
    // properly formatted HTTP response with the error code 400
    sendLine(sockFd, "HTTP/1.0 400 Bad Request");
    // empty line to end header
    sendLine(sockFd, "");
    break;
  case 404:
    DEBUG << "Sending 404 error" << ENDL;
    // properly formatted HTTP response with the error code 404
    sendLine(sockFd, "HTTP/1.0 404 Not Found");
    // content-type: text/html" to indicate we are sending a message
    sendLine(sockFd, "content-type: text/html");
    // Send a blank line to terminate the header
    sendLine(sockFd, "");
    // Friendly error message
    sendLine(sockFd, "File not found");
    // Send a blank line to terminate the body
    sendLine(sockFd, "");
    break;
  case 200:
    serveFile(sockFd, filename);
    break;
  default:
    ERROR << "Unknown response code, skipping" << ENDL;
  }
  return 0;
}

// *****************************************************************************
// - Read the header
// - Return the appropriate status code and filename
// *****************************************************************************
int readRequest(int sockFd) {
  // Set default return code to 400
  int returnStatus = 400;
  int numChars;

  // REGEX
  std::cmatch result = std::cmatch();
  std::regex endline("\r\n");
  std::regex doubleEndline("\r\n\r\n");
  std::regex get("GET\\s+\\/[\\w\\d]*.?[\\w\\d]*\\s+HTTP\\/\\d\\.\\d\r\n");
  std::regex noFile("GET\\s+\\/\\s+HTTP/\\d\\.\\d\r\n");
  std::regex file("(image\\d\\.jpg|file\\d\\.html)");
  std::regex fileStrict("(image\\d\\.jpg|file\\d\\.html\\s)");

  // Read the header
  char *buffer = (char *)malloc(MAX_LEN * sizeof(char));

  // Read the characters
  DEBUG << "Reading " << MAX_LEN << " bytes from socket " << sockFd << ENDL;
  int bytesRead = read(sockFd, buffer, MAX_LEN);
  if (bytesRead == -1 || bytesRead == 0) {
    ERROR << "Error reading from client" << ENDL;
    return 400;
  }

  // Search for double endline
  if (!regex_search(buffer, result, doubleEndline)) {
    ERROR << "No double endline found" << ENDL;
    return 400;
  }

  numChars = result.prefix().length() + 4; // 4 is the length of the double endline
  DEBUG << "Found double endline at position " << numChars << ENDL;

  // Copy double endline
  char *header = (char *)malloc(numChars * sizeof(char));
  memcpy(header + 0, buffer + 0, numChars);
  free(buffer);

  // First line of the header
  regex_search(header, result, endline);
  numChars = result.prefix().length() + 2; // 2 is the length of the endline
  DEBUG << "Found endline at position " << numChars << ENDL;
  char *firstLine = (char *)malloc(numChars * sizeof(char));
  memcpy(firstLine, header, numChars);
  free(header);

  // Check if the first line is a GET request
  if (regex_search(firstLine, get)) {
    DEBUG << "Found GET request" << ENDL;
    if (!regex_search(firstLine, noFile)) {
      if(regex_search(firstLine, result, file) && regex_search(firstLine, fileStrict)) {
        DEBUG << "Found file request" << ENDL;
        returnStatus = 200;
        filename = std::string(result[0].str());
        DEBUG << "Filename: " << filename << ENDL;
      } else {
        ERROR << "Invalid file request" << ENDL;
        returnStatus = 404;
      }
    } else {
      DEBUG << "No file requested" << ENDL;
    }
  } else {
    ERROR << "Invalid request" << ENDL;
  }

  free(firstLine);
  return returnStatus;
}

// *****************************************************************************
// Send one line (including the line terminator <LF><CR>)
// *****************************************************************************
void sendLine(int sockFd, std::string line) {
  std::string formattedLine = line + "\r\n"; // Ensure proper CRLF termination
  int bytesWritten = write(sockFd, formattedLine.c_str(), formattedLine.size());
  if (bytesWritten != static_cast<int>(formattedLine.size())) {
    ERROR << "Error writing to client" << ENDL;
    exit(1);
  }
}

// *****************************************************************************
// Serve a file
// *****************************************************************************
void serveFile(int sockFd, std::string filename) {
  DEBUG << "Serving file " << filename << ENDL;

  struct stat fileInfo;
  if (stat(filename.c_str(), &fileInfo) == -1) {
    DEBUG << "File not found OR Invalid permissions" << ENDL;
    DEBUG << "Sending 404 error" << ENDL;
    // properly formatted HTTP response with the error code 404
    sendLine(sockFd, "HTTP/1.0 404 Not Found");
    // content-type: text/html" to indicate we are sending a message
    sendLine(sockFd, "content-type: text/html");
    // Send a blank line to terminate the header
    sendLine(sockFd, "");
    // Friendly error message
    sendLine(sockFd, "File not found");
    // Send a blank line to terminate the body
    sendLine(sockFd, "");
    return;
  }

  int len = fileInfo.st_size;
  DEBUG << "File size: " << len << ENDL;

  sendLine(sockFd, "HTTP/1.0 200 OK");

  // Check if the file is an image
  std::regex image("image\\d\\.jpg");
  if (regex_search(filename, image)) {
    sendLine(sockFd, "content-type: image/jpeg");
  } else {
    sendLine(sockFd, "content-type: text/html");
  }

  // Send the content length
  sendLine(sockFd, "content-length: " + std::to_string(len));

  // Send a blank line to terminate the header
  sendLine(sockFd, "");

  DEBUG << "Opening file " << filename << ENDL;
  std::ifstream file(filename);
  if (!file.is_open()) {
    ERROR << "Error opening file" << ENDL;
    DEBUG << "Sending 400 error" << ENDL;
    // properly formatted HTTP response with the error code 400
    sendLine(sockFd, "HTTP/1.0 400 Bad Request");
    // empty line to end header
    sendLine(sockFd, "");
    return;
  }

  char *buffer = (char *)malloc(MAX_LEN * sizeof(char));
  memset(buffer, 0, MAX_LEN);
  std::streamsize bytesRead;
  size_t bytesWritten = 0, totalBytesRead = 0;

  while((bytesRead = file.readsome(buffer, MAX_LEN)) != 0) {
    DEBUG << "Read " << bytesRead << " bytes from file" << ENDL;
    totalBytesRead += bytesRead;
    
    for (size_t i = 0; i < bytesRead; i++) {
      DEBUG << "Sending " << buffer[i] << " to socket " << sockFd << ENDL;
      int bWrite = write(sockFd, &buffer[i], 1);
      if (bWrite != 1) {
        ERROR << "Error writing to client" << ENDL;
        DEBUG << "Sending 400 error" << ENDL;
        // properly formatted HTTP response with the error code 400
        sendLine(sockFd, "HTTP/1.0 400 Bad Request");
        // empty line to end header
        sendLine(sockFd, "");
        return;
      }
      bytesWritten += 1;
    }
    memset(buffer, 0, MAX_LEN);
    DEBUG << std::endl << "Total bytes written: " << bytesWritten << ENDL;
  }

  if (bytesWritten != len) {
    ERROR << "File content length mismatched" << ENDL;
     DEBUG << "Sending 400 error" << ENDL;
    // properly formatted HTTP response with the error code 400
    sendLine(sockFd, "HTTP/1.0 400 Bad Request");
    // empty line to end header
    sendLine(sockFd, "");
  }

  DEBUG << "Successfully sent " << bytesWritten << " bytes to client" << ENDL;
  free(buffer);
  file.close();
}

// **************************************************************************************
// * main()
// * - Sets up the sockets and accepts new connection until processConnection() returns 1
// **************************************************************************************
int main (int argc, char *argv[]) {

  // ********************************************************************
  // * Process the command line arguments
  // ********************************************************************
  int opt = 0;
  while ((opt = getopt(argc,argv,"d:")) != -1) {
    
    switch (opt) {
    case 'd':
      LOG_LEVEL = std::stoi(optarg);;
      break;
    case ':':
    case '?':
    default:
      std::cout << "useage: " << argv[0] << " -d <num>" << std::endl;
      exit(-1);
    }
  }

  // *******************************************************************
  // * Creating the inital socket is the same as in a client.
  // ********************************************************************
  int     listenFd = -1;
  // Call socket() to create the socket you will use for lisening.
  listenFd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenFd == -1) {
    FATAL << "Error creating socket" << ENDL;
    exit(-1);
  }
  DEBUG << "Calling Socket() assigned file descriptor " << listenFd << ENDL;

  
  // ********************************************************************
  // * The bind() and calls take a structure that specifies the
  // * address to be used for the connection. On the cient it contains
  // * the address of the server to connect to. On the server it specifies
  // * which IP address and port to lisen for connections.
  // ********************************************************************
  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  //Randomly generate a port number
  srand(time(NULL));
  int port = (rand() % 10000) + 1024;

  // *** assign 3 fields in the servadd struct sin_family, sin_addr.s_addr and sin_port
  // *** the value your port can be any value > 1024.
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);


  // ********************************************************************
  // * Binding configures the socket with the parameters we have
  // * specified in the servaddr structure.  This step is implicit in
  // * the connect() call, but must be explicitly listed for servers.
  // ********************************************************************
  bool bindSuccesful = false;
  DEBUG << "Calling bind(" << listenFd << ", " << &servaddr << ", " << sizeof(servaddr) << ")" << ENDL;
  while (!bindSuccesful) {
    // ** Call bind()
    // You may have to call bind multiple times if another process is already using the port
    // your program selects.
    if (bind(listenFd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
      FATAL << "Error binding to socket" << ENDL;
      port = (rand() % 10000) + 1024;
      servaddr.sin_port = htons(port);
    } else {
      bindSuccesful = true;
    }
  }
  // *** DON'T FORGET TO PRINT OUT WHAT PORT YOUR SERVER PICKED SO YOU KNOW HOW TO CONNECT.
  std::cout << "Server listening on port " << port << std::endl;

  // ********************************************************************
  // * Setting the socket to the listening state is the second step
  // * needed to being accepting connections.  This creates a queue for
  // * connections and starts the kernel listening for connections.
  // ********************************************************************
  int listenQueueLength = 1;
  // ** Cal listen()
  DEBUG << "Calling listen(" << listenFd << ", " << listenQueueLength << ")" << ENDL;
  if (listen(listenFd, listenQueueLength) == -1) {
    FATAL << "Error listening on socket" << ENDL;
    exit(-1);
  }

  // ********************************************************************
  // * The accept call will sleep, waiting for a connection.  When 
  // * a connection request comes in the accept() call creates a NEW
  // * socket with a new fd that will be used for the communication.
  // ********************************************************************
  bool quitProgram = false;
  while (!quitProgram) {
    int connFd = 0;

    // Call the accept() call to check the listening queue for connection requests.
    // If a client has already tried to connect accept() will complete the
    // connection and return a file descriptor that you can read from and
    // write to. If there is no connection waiting accept() will block and
    // not return until there is a connection.
    // ** call accept() 
    DEBUG << "Calling accept(" << listenFd << ", NULL, NULL)" << ENDL;
    connFd = accept(listenFd, NULL, NULL);
    if (connFd == -1) {
      FATAL << "Error accepting connection" << ENDL;
      exit(-1);
    }
    DEBUG << "Accepted connection on file descriptor " << connFd << ENDL;
    
    // Now we have a connection, so you can call processConnection() to do the work.
    quitProgram = processConnection(connFd);
   
    close(connFd);
  }

  close(listenFd);

}
