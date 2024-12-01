#include "traceroute.h"

// ****************************************************************************
// * Compute the Internet Checksum over an arbitrary buffer.
// * (written with the help of ChatGPT 3.5)
// ****************************************************************************
uint16_t checksum(unsigned short *buffer, int size) {
  unsigned long sum = 0;
  while (size > 1) {
    sum += *buffer++;
    size -= 2;
  }
  if (size == 1) {
    sum += *(unsigned char *)buffer;
  }
  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  return (unsigned short)(~sum);
}

// Build ICMP Echo Request
void buildPacket(char *buffer, int sequenceNumber, int pid) {
  memset(buffer, 0, PACKET_SIZE);
  struct icmp *icmpHdr = (struct icmp *)buffer;
  icmpHdr->icmp_type = ICMP_ECHO; // Set ICMP type to ECHO request
  icmpHdr->icmp_code = 0;
  icmpHdr->icmp_id = htons(pid & 0xFFFF);
  icmpHdr->icmp_seq = htons(sequenceNumber);
  icmpHdr->icmp_cksum = 0;
  icmpHdr->icmp_cksum = checksum((unsigned short *)icmpHdr, sizeof(struct icmp)); // Compute checksum

  DEBUG << "Built ICMP packet with Sequence Number: " << sequenceNumber << ", PID: " << pid << ENDL;
}

// Send ICMP packet
void sendPacket(int sockfd, const char *buffer, const struct sockaddr_in &destAddr, int ttl) {
  // Set the TTL for the packet
  if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
    ERROR << "Failed to set TTL = " << ttl << ENDL;
    perror("setsockopt");
    return;
  }
  DEBUG << "TTL set to " << ttl << ENDL;

  // Send the packet
  ssize_t bytes_sent = sendto(sockfd, buffer, sizeof(struct icmp), 0, (struct sockaddr *)&destAddr, sizeof(destAddr));
  if (bytes_sent <= 0) {
    WARNING << "Failed to send datagram for TTL = " << ttl << ENDL;
    perror("sendto");
  } else {
    INFO << "Datagram sent: TTL = " << ttl << ", Bytes = " << bytes_sent << ENDL;
  }
}

// Receive ICMP response
bool receiveResponse(int sockfd, int ttl, int pid) {
  fd_set readfds;
  FD_ZERO(&readfds); // Initialize file descriptor set
  FD_SET(sockfd, &readfds); // Add socket to file descriptor set

  struct timeval timeout = {15, 0}; // 15-second timeout
  int retval = select(sockfd + 1, &readfds, nullptr, nullptr, &timeout); // Wait for response

  if (retval == -1) { // Error in select()
    ERROR << "Select() failed." << ENDL;
    perror("select()");
    exit(EXIT_FAILURE);
  } else if (retval == 0) { // Timeout
    WARNING << "No response with TTL = " << ttl << ENDL;
    return false;
  }

  char recvBuffer[PACKET_SIZE];
  struct sockaddr_in recvAddr;
  socklen_t addrLen = sizeof(recvAddr);
  ssize_t len = recvfrom(sockfd, recvBuffer, sizeof(recvBuffer), 0, (struct sockaddr *)&recvAddr, &addrLen);

  if (len > 0) {
    struct ip *ipHdr = (struct ip *)recvBuffer; // Extract IP header
    int ipHdr_len = ipHdr->ip_hl * 4; // Calculate IP header length
    struct icmp *icmpHdr = (struct icmp *)(recvBuffer + ipHdr_len); // Extract ICMP header

    if (icmpHdr->icmp_type == ICMP_ECHOREPLY && icmpHdr->icmp_id == htons(pid & 0xFFFF)) {
      char addrStr[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &(recvAddr.sin_addr), addrStr, INET_ADDRSTRLEN); // Convert IP address to string
      INFO << "Destination reached: " << addrStr << ENDL;
      return true;
    } else if (icmpHdr->icmp_type == ICMP_TIME_EXCEEDED) { // ICMP Time Exceeded
      char addrStr[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &(recvAddr.sin_addr), addrStr, INET_ADDRSTRLEN);
      INFO << "TTL " << ttl << ": " << addrStr << ENDL;
    } else {
      TRACE << "Received ICMP type " << (int)icmpHdr->icmp_type << " from " << inet_ntoa(recvAddr.sin_addr) << ENDL;
    }
  } else { // Error in recvfrom()
    ERROR << "Failed to receive packet for TTL = " << ttl << ENDL;
    perror("recvfrom");
  }
  return false;
}

int main(int argc, char *argv[]) {
  std::string destIP;

  // ********************************************************************
  // * Process the command line arguments
  // ********************************************************************
  int opt = 0;
  while ((opt = getopt(argc, argv, "d:v:")) != -1) {

    switch (opt) {
    case 'd':
      destIP = optarg;
      break;
    case 'v':
      LOG_LEVEL = atoi(optarg);
      ;
      break;
    case ':':
    case '?':
    default:
      std::cout << "useage: " << argv[0] << " -d [destination ip] -v [Log Level]" << std::endl;
      exit(-1);
    }
  }
  if (destIP.empty()) { // Destination IP is required
    FATAL << "Destination IP is required." << ENDL;
    FATAL << "usage: " << argv[0] << " -d [destination ip] -v [Log Level]" << ENDL;
    exit(-1);
  }

  // Create raw socket
  int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (sockfd < 0) { // Error in socket creation
    FATAL << "Socket creation failed. Need root privileges." << ENDL;
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  INFO << "Raw socket created successfully." << ENDL;

  // Set destination address
  struct sockaddr_in destAddr;
  memset(&destAddr, 0, sizeof(destAddr)); // Clear the structure
  destAddr.sin_family = AF_INET; // Set address family to Internet
  if (inet_pton(AF_INET, destIP.c_str(), &destAddr.sin_addr) != 1) { // Convert IP address to binary
    FATAL << "Invalid destination IP address: " << destIP << ENDL;
    exit(EXIT_FAILURE);
  }

  INFO << "Destination IP set to " << destIP << ENDL;

  int pid = getpid();
  int datagrams_sent = 0;
  bool reached = false;

  for (int ttl = 2; ttl <= MAX_TTL && !reached; ++ttl) { // Loop over TTL values
    char sendBuffer[PACKET_SIZE]; // Buffer to store ICMP packet
    buildPacket(sendBuffer, ttl, pid); // Build ICMP packet
    sendPacket(sockfd, sendBuffer, destAddr, ttl); // Send ICMP packet

    datagrams_sent++; // Increment datagram count
    if (receiveResponse(sockfd, ttl, pid)) {
      reached = true; // Destination reached
    }
  }

  std::cout << "Total datagrams sent: " << datagrams_sent << std::endl;
  if (!reached) {
    WARNING << "Did not reach destination within " << MAX_TTL << " hops." << ENDL;
  }

  close(sockfd); // Close the socket
  INFO << "Socket closed. Program finished." << ENDL;
  return 0;
}