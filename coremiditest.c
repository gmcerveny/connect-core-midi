#include <CoreFoundation/CoreFoundation.h>
#include <CoreMIDI/CoreMIDI.h>
#include <assert.h>

#define BUFFER_SIZE 512

void printDevices();
void printEndpoints();
void printDescription(MIDIDeviceRef obj);
char * MYCFStringCopyUTF8String(CFStringRef aString);
void createOutputPort(MIDIClientRef *client, MIDIPortRef *port);
void sendTestMessage(MIDIPortRef port, MIDIEndpointRef destination);

int main(int argc, char *argv[])
{
  printDevices();
  printEndpoints();

  MIDIClientRef client;
  MIDIPortRef port;
  createOutputPort(&client, &port);
  // TODO: store MIDI Destination
  sendTestMessage(port, MIDIGetDestination(1));

  while(1){
    // forever
  }

  return 0;
}

// TODO: free client and port
void createOutputPort(MIDIClientRef *client, MIDIPortRef *port)
{
  OSStatus err;
  CFStringRef clientName = CFSTR("Test Client");
  err = MIDIClientCreate(clientName, NULL, NULL, client);
  assert(err == 0);

  CFStringRef portName = CFSTR("Test Port");
  MIDIPortRef testPort;
  err = MIDIOutputPortCreate(*client, portName, port);
  assert(err == 0);
}

void sendTestMessage(MIDIPortRef port, MIDIEndpointRef destination)
{
  char buffer[BUFFER_SIZE];
  char msg[3];
  char note_on = 0x9, channel = 0;
  int midiNote = 54;
  int velocity = 127;

  msg[0] = note_on << 4 | channel;
  msg[1] = midiNote;
  msg[2] = velocity;

  MIDIPacketList *packetList = (MIDIPacketList *)buffer;
  MIDIPacket *packet = MIDIPacketListInit(packetList);
  packet = MIDIPacketListAdd(packetList, 
    BUFFER_SIZE,
    packet,
    0,  // now
    sizeof(msg),
    (Byte *)msg);

  OSStatus err;
  err = MIDISend(port, destination, packetList);
  assert(err == 0);
}

void printEndpoints()
{
  ItemCount sourceCount = MIDIGetNumberOfSources();
  printf("Sources:\t%lu\n", sourceCount);

  for (ItemCount i = 0; i < sourceCount; i++) {
    MIDIEndpointRef endpoint = MIDIGetSource(i);
  
    CFStringRef stringRef;
    MIDIObjectGetStringProperty(endpoint, kMIDIPropertyName, &stringRef);
    char *description = MYCFStringCopyUTF8String(stringRef);
    printf("[%lu]:\t%s\n", i, description);
    free(description);
  }

  ItemCount destinationCount = MIDIGetNumberOfDestinations();
  printf("Destinations:\t%lu\n", destinationCount);

  for (ItemCount i = 0; i < destinationCount; i++) {
    MIDIEndpointRef destination = MIDIGetDestination(i);

    CFStringRef stringRef;
    MIDIObjectGetStringProperty(destination, kMIDIPropertyName, &stringRef);
    char *description = MYCFStringCopyUTF8String(stringRef);
    printf("[%lu]:\t%s\n", i, description);
    free(description);
  }
}

void printDevices()
{
  ItemCount deviceCount = MIDIGetNumberOfDevices();
  printf("Devices:\t%lu\n", deviceCount);

  for (ItemCount i = 0; i < deviceCount; i++){
    MIDIDeviceRef device = MIDIGetDevice(i);
    printDescription(device);
  }  
}

void printExternalDevices()
{
  ItemCount externalDeviceCount = MIDIGetNumberOfExternalDevices();
  printf("External Devices:\t%lu\n", externalDeviceCount);

  // TODO: print external device descriptions  
}

void printDescription(MIDIDeviceRef obj)
{
  CFStringRef stringRef;
  MIDIObjectGetStringProperty(obj, kMIDIPropertyName, &stringRef);
  char *description = MYCFStringCopyUTF8String(stringRef);
  printf("Description: %s\n", description);
  free(description);
}

// http://stackoverflow.com/questions/9166291/converting-a-cfstringref-to-char
char * MYCFStringCopyUTF8String(CFStringRef aString) {
  if (aString == NULL) {
    return NULL;
  }

  CFIndex length = CFStringGetLength(aString);
  CFIndex maxSize =
  CFStringGetMaximumSizeForEncoding(length,
                                    kCFStringEncodingUTF8);
  char *buffer = (char *)malloc(maxSize);
  if (CFStringGetCString(aString, buffer, maxSize,
                         kCFStringEncodingUTF8)) {
    return buffer;
  }
  return NULL;
}