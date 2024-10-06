// ********************************************************
// * A common set of system include files needed for socket() programming
// ********************************************************
#include <filesystem>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#include "logging.h"

// Functions
int processConnection(int socketFD);
int readRequest(int sockFd);
void sendLine(int sockFd, std::string line);
void serveFile(int sockFd, std::string filename);

#define MAX_LEN 1024