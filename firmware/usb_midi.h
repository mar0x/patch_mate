
#pragma once

#include <stdint.h>
#include <PluggableUSB.h>

#include "serial_num.h"
#include "usb_midi_desc.h"

namespace usb_midi {

struct event_t {
    uint8_t header;
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;

    void reset() { header = 0; }
    bool empty() const { return header == 0; }

    operator bool() const { return header != 0; }
};

/**
   Concrete MIDI implementation of a PluggableUSBModule
   By default, will define one midi in and one midi out enpoints.
 */
class port_t : public PluggableUSBModule
{
private:
    uint8_t rx_ep() const { return pluggedEndpoint; }
    uint8_t tx_ep() const { return pluggedEndpoint + 1; }

    uint8_t epType[2];   ///< Container that defines the two bulk MIDI IN/OUT endpoints types

protected:
    virtual bool setup(USBSetup& /* setup */) override {
        return false;
    }

    virtual int getInterface(uint8_t* interfaceNum) override {
      *interfaceNum += 2; // uses 2 interfaces

      MIDIDescriptor _midiInterface =
      {
        D_IAD(pluggedInterface, 2, MIDI_AUDIO, MIDI_AUDIO_CONTROL, 0),
        D_INTERFACE(pluggedInterface, 0, MIDI_AUDIO, MIDI_AUDIO_CONTROL,0),
        ACIfcDesc(0x01, pluggedInterface + 1),
        D_INTERFACE((uint8_t) (pluggedInterface + 1), 2, MIDI_AUDIO, MIDI_STREAMING, 0),
        ASIfcDesc(),
        JackInDesc(MIDI_JACK_EMD, 0x01),
        JackInDesc(MIDI_JACK_EXT, 0x02),
        JackOutDesc(MIDI_JACK_EMD, 0x03, 2),
        JackOutDesc(MIDI_JACK_EXT, 0x04, 1),

        D_MIDI_JACK_EP(USB_ENDPOINT_OUT(pluggedEndpoint)),
        EPACDesc(1),

        D_MIDI_JACK_EP(USB_ENDPOINT_IN(pluggedEndpoint + 1)),
        EPACDesc(3)
      };
      return USB_SendControl(0, &_midiInterface, sizeof(_midiInterface));
    }

    virtual int getDescriptor(USBSetup& /* setup */) override {
        return 0;
    }

    virtual uint8_t getShortName(char* name) override {
        patch_mate::serial_num_t sn;
        sn.get();

        memcpy(name, "PMX", 3);
        memcpy(name + 3, sn, sizeof(sn));
        return 3 + sizeof(sn);
    }

public:
    port_t() : PluggableUSBModule(2, 2, epType)
    {
        epType[0] = EP_TYPE_BULK_OUT;   // MIDI_ENDPOINT_OUT
        epType[1] = EP_TYPE_BULK_IN;    // MIDI_ENDPOINT_IN
        PluggableUSB().plug(this);
    }

    int available() const {
        return USB_Available(rx_ep()) / sizeof(event_t);
    }

    int read(event_t& p) const {
        int c = USB_Recv(rx_ep(), &p, sizeof(p));

        if (c < (int) sizeof(p)) {
            p.reset();
            return 0;
        }

        return 1;
    }

    void flush(void) { USB_Flush(tx_ep()); }

    void send(const event_t& event) {
        USB_Send(tx_ep(), &event, sizeof(event));
    }
};
}
