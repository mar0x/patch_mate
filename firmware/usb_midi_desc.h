
#pragma once

#include <stdint.h>
#include <PluggableUSB.h>

namespace usb_midi {

enum {
    MIDI_AUDIO = 0x01,          // USB interfaceClass
    MIDI_AUDIO_CONTROL = 0x01,  // USB interfaceSubClass
    MIDI_STREAMING = 0x03,      // USB interfaceSubClass

    MIDI_CS_INTERFACE = 0x24,
    MIDI_CS_ENDPOINT = 0x25,
    MIDI_JACK_EMD = 0x01,
    MIDI_JACK_EXT = 0X02,

    MIDI_BUFFER_SIZE = USB_EP_SIZE, // 64
};

_Pragma("pack(1)")

/// Midi Audio Control Interface Descriptor
struct ACIfcDesc {
    uint8_t len = 9; // USB_INTERFACE_DESC_SIZE
    uint8_t dtype = MIDI_CS_INTERFACE;
    uint8_t dsubType = 0x01;
    uint16_t bcdADc = 0x0100;
    uint16_t wTotalLength = 0x0009;
    uint8_t bInCollection;
    uint8_t interfaceNumbers;

    ACIfcDesc(uint8_t bInC, uint8_t num) :
      bInCollection(bInC),
      interfaceNumbers(num)
    { }
};

/// MIDI Audio Stream Descriptor Interface
struct ASIfcDesc
{
    uint8_t len = 7;
    uint8_t dtype = MIDI_CS_INTERFACE;
    uint8_t dsubType = 0x01;
    uint16_t bcdADc = 0x0100;
    uint16_t wTotalLength = 0x0041;
};

struct JackInDesc
{
    uint8_t len = 6;
    uint8_t dtype = MIDI_CS_INTERFACE;
    uint8_t dsubType = 0x02;
    uint8_t jackType;
    uint8_t jackID;
    uint8_t jackStrIndex = 0;

    JackInDesc(uint8_t jackProp, uint8_t id) :
      jackType(jackProp), jackID(id)
    { }
};

struct JackOutDesc
{
    uint8_t len = 9;
    uint8_t dtype = MIDI_CS_INTERFACE;
    uint8_t dsubType = 0x03;
    uint8_t jackType;
    uint8_t jackID;
    uint8_t nPins = 1;
    uint8_t srcJackID;
    uint8_t srcPinID = 1;
    uint8_t jackStrIndex = 0;

    JackOutDesc(uint8_t jackProp, uint8_t id, uint8_t srcID) :
        jackType(jackProp), jackID(id), srcJackID(srcID)
    { }
};

/// MIDI Jack EndPoint Descriptor, common to midi in and out jacks.
struct JackEPDesc
{
    EndpointDescriptor len;
    uint8_t refresh;
    uint8_t sync;
};

#define D_MIDI_JACK_EP(_addr) \
  { 9, 5, _addr, USB_ENDPOINT_TYPE_BULK, MIDI_BUFFER_SIZE, 0, 0, 0}

/// MIDI Jack  EndPoint AudioControl Descriptor, common to midi in and out ac jacks.
struct EPACDesc
{
    uint8_t len = 5;
    uint8_t dtype = MIDI_CS_ENDPOINT;
    uint8_t subtype = 0x01;
    uint8_t embJacks = 1;
    uint8_t jackID;

    EPACDesc(uint8_t id) : jackID(id) { }
};

/// Top Level MIDI Descriptor used to create a Midi Interface instance \see MIDI_::getInterface()
struct MIDIDescriptor
{
    // IAD
    IADDescriptor             iad;
    // MIDI Audio Control Interface
    InterfaceDescriptor       Audio_ControlInterface;
    ACIfcDesc                 Audio_ControlInterface_SPC;

    // MIDI Audio Streaming Interface
    InterfaceDescriptor       Audio_StreamInterface;
    ASIfcDesc                 Audio_StreamInterface_SPC;

    JackInDesc                MIDI_In_Jack_Emb;
    JackInDesc                MIDI_In_Jack_Ext;
    JackOutDesc               MIDI_Out_Jack_Emb;
    JackOutDesc               MIDI_Out_Jack_Ext;

    JackEPDesc                MIDI_In_Jack_Endpoint;
    EPACDesc                  MIDI_In_Jack_Endpoint_SPC;
    JackEPDesc                MIDI_Out_Jack_Endpoint;
    EPACDesc                  MIDI_Out_Jack_Endpoint_SPC;
};

}
