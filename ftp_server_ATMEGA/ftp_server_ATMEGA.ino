/*
   FTP server for IDE v1.8.5 and w5100

   Port fron https://github.com/nailbuster/esp8266FTPServer

*/
#include <SPI.h>
#include <Ethernet.h>
#include <SdFat.h>      // https://github.com/jbeynon/sdfatlib
#include "FtpServer.h"

#define FTP_USER    "arduino"  // You can change
#define FTP_PASS    "mega"     // You can change

// SD chip select pin
const uint8_t SD_CS = 2;

SdFat sd;
SdFile file;
FtpServer ftpSrv;

void CallBackDelete(const char *fileName) {
  Serial.println("[CallBack] DeleteFile=" + String(fileName));
}

void CallBackStor(const char *fileName) {
  Serial.println("[CallBack] StoreFile=" + String(fileName));
}

void CallBackRmdir(const char *dirName) {
  Serial.println("[CallBack] RemoveDirectory=" + String(dirName));
}

void CallBackMkdir(const char *dirName) {
  Serial.println("[CallBack] MakeDirectory=" + String(dirName));
}

void CallBackRename(const char *fileName1, const char *fileName2) {
  Serial.println("[CallBack] Rename file from=" + String(fileName1) + " to=" + String(fileName2));
}

void setup(void){
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   // You can change
  byte myIP[] = { 192, 168, 10, 190 };                   // You can change

  Serial.begin(9600);
  Serial.print("SD begin....");
  if (!sd.begin(SD_CS, SPI_HALF_SPEED)) {
    Serial.println("failed....");
    while(1) {}
  }
  Serial.println("ok....");
  sd.ls(&Serial,LS_DATE | LS_SIZE | LS_R);

  Serial.print("Ethernet begin....");
  Ethernet.begin(mac,myIP);
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
//    Serial.println(Ethernet.localIP()[thisByte]);
    if (Ethernet.localIP()[thisByte] != myIP[thisByte] ) {
      Serial.println("fail....");
      while (1) {}
    }
  }
  Serial.println("ok....");
  Serial.print("localIP: ");
  Serial.println(Ethernet.localIP());
//  Serial.print("subnetMask: ");
//  Serial.println(Ethernet.subnetMask());
//  Serial.print("gatewayIP: ");
//  Serial.println(Ethernet.gatewayIP());
//  Serial.print("dnsServerIP: ");
//  Serial.println(Ethernet.dnsServerIP());

  // Initialize the FTP server
  ftpSrv.begin(FTP_USER, FTP_PASS);
  // Set Callback Function for Delete
  ftpSrv.setCallBackDelete(CallBackDelete);
  // Set Callback Function for Rmdir
  ftpSrv.setCallBackRmdir(CallBackRmdir);
  // Set Callback Function for Stor
  ftpSrv.setCallBackStor(CallBackStor);
  // Set Callback Function for Mkdir
  ftpSrv.setCallBackMkdir(CallBackMkdir);
  // Set Callback Function for Rename
  ftpSrv.setCallBackRename(CallBackRename);

}

void loop(void){
  ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!  
}

