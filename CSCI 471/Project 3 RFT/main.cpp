//
// Created by Phillip Romig on 7/16/24.
//
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <system_error>
#include <unistd.h>
#include <array>

#include "timerC.h"
#include "unreliableTransport.h"
#include "logging.h"


#define WINDOW_SIZE 10
int main(int argc, char* argv[]) {

    // Defaults
    uint16_t portNum(12345);
    std::string hostname("isengard.mines.edu");
    std::string inputFilename("input.dat");

    int opt;
    try {
        while ((opt = getopt(argc, argv, "f:h:p:d:")) != -1) {
            switch (opt) {
                case 'p':
                    portNum = std::stoi(optarg);
                    break;
                case 'h':
                    hostname = optarg;
                    break;
                case 'd':
                    LOG_LEVEL = std::stoi(optarg);
                    break;
                case 'f':
                    inputFilename = optarg;
                    break;
                case '?':
                default:
                    std::cout << "Usage: " << argv[0] << " [-h hostname] [-p port] [-d debug_level]" << std::endl;
                    break;
            }
        }
    } catch (std::exception &e) {
        FATAL << "Invalid command line arguments: " << e.what() << ENDL;
        return(1);
    }

    TRACE << "Command line arguments parsed." << ENDL;
    TRACE << "\tServername: " << hostname << ENDL;
    TRACE << "\tPort number: " << portNum << ENDL;
    TRACE << "\tDebug Level: " << LOG_LEVEL << ENDL;
    TRACE << "\tOutput file name: " << inputFilename << ENDL;

    // *********************************
    // * Open the input file
    // *********************************

    try {
        // ***************************************************************
        // * Initialize your timer, window, and the unreliableTransport etc.
        // **************************************************************
        timerC timer;
        timer.setDuration(1000);  // Set timer duration to 1000 ms
        unreliableTransportC unreliableTransport(hostname, portNum);
        std::ifstream inputFile(inputFilename, std::ios::binary);
        if (!inputFile.is_open()) {
            FATAL << "Unable to open input file: " << inputFilename << ENDL;
            exit(1);
        }

        // ***************************************************************
        // * Send the file one datagram at a time until they have all been
        // * acknowledged
        // **************************************************************
        std::array<datagramS, WINDOW_SIZE> sndpkt;
        uint16_t base = 0;
        uint16_t nextSeqNum = 1;
        bool allSent(false);
        bool allAcked(false);

        // Loop for Go-Back-N protocol
        while ((!allSent) || (!allAcked)) {
            // If within the window, read and send the next packet
            if (!allSent && nextSeqNum < base + WINDOW_SIZE) {
                datagramS packet{};
                packet.seqNum = nextSeqNum;
                inputFile.read(packet.data, MAX_PAYLOAD_LENGTH);
                packet.payloadLength = inputFile.gcount();
                packet.checksum = computeChecksum(packet);

                TRACE << "Sending packet " << packet.seqNum << ENDL;
                unreliableTransport.udt_send(packet);
                sndpkt[nextSeqNum % WINDOW_SIZE] = packet;

                if (base == nextSeqNum) {
                    timer.start();
                }

                nextSeqNum++;
                if (packet.payloadLength == 0) {
                    allSent = true;  // Set allSent to true once end of file is reached
                }
            }

            // Call udt_recieve() to see if there is an acknowledgment.  I
            datagramS ackPkt{};
            ssize_t bytesRead = unreliableTransport.udt_receive(ackPkt);
            if (bytesRead > 0 && validateChecksum(ackPkt)) {
                TRACE << "Received ACK for packet " << ackPkt.ackNum << ENDL;
                base = ackPkt.ackNum + 1;
                if (base == nextSeqNum) {
                    timer.stop();
                    allAcked = allSent;  // Set allAcked to true if all packets have been sent and acknowledged
                } else {
                    timer.start();  // Restart timer for next unacknowledged packet
                }
            }

            // Check to see if the timer has expired.
            if (timer.timeout()) {
                TRACE << "Timer expired; resending window from " << base << ENDL;
                timer.start();
                for (uint16_t i = base; i < nextSeqNum; i++) {
                    TRACE << "Resending packet " << sndpkt[i % WINDOW_SIZE].seqNum << ENDL;
                    unreliableTransport.udt_send(sndpkt[i % WINDOW_SIZE]);
                }
            }
        }

        // Cleanup and close the file and network.
        inputFile.close();
        TRACE << "File transfer complete." << ENDL;

        // Send a FIN packet after the entire file has been acknowledged
        datagramS finPacket{};
        finPacket.seqNum = nextSeqNum; // Use the next sequence number for the FIN packet
        finPacket.payloadLength = 0;    // FIN packets usually have no data
        finPacket.checksum = computeChecksum(finPacket);
        TRACE << "Sending FIN packet " << finPacket.seqNum << ENDL;
        unreliableTransport.udt_send(finPacket);
    } catch (const std::exception &e) {
        FATAL << "An error occurred: " << e.what() << ENDL;
        exit(1);
    }
    return 0;
}
