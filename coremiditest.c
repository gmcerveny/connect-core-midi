#include <CoreFoundation/CoreFoundation.h>
#include <CoreMIDI/CoreMIDI.h>

void printDevices();
void printEndpoints();
void printDescription(MIDIDeviceRef obj);
char * MYCFStringCopyUTF8String(CFStringRef aString);

int main(int argc, char *argv[])
{
  printDevices();
  printEndpoints();

  return 0;
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

  for (ItemCount i = 0; i < sourceCount; i++) {
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