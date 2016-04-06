/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef MICROBIT_MULTI_BUTTON_H
#define MICROBIT_MULTI_BUTTON_H

#include "MicroBit.h"

#define MICROBIT_MULTI_BUTTON_STATE_1               0x01
#define MICROBIT_MULTI_BUTTON_STATE_2               0x02
#define MICROBIT_MULTI_BUTTON_HOLD_TRIGGERED_1      0x04
#define MICROBIT_MULTI_BUTTON_HOLD_TRIGGERED_2      0x08
#define MICROBIT_MULTI_BUTTON_SUPRESSED_1           0X10 
#define MICROBIT_MULTI_BUTTON_SUPRESSED_2           0x20
#define MICROBIT_MULTI_BUTTON_ATTACHED              0x40

/**
  * Class definition for MicroBitMultiButton.
  *
  * Represents a virtual button, capable of reacting to simultaneous presses of multiple 
  * other buttons.
  */
class MicroBitMultiButton : public MicroBitComponent
{
    uint16_t    button1;        // ID of the first button we're monitoring
    uint16_t    button2;        // ID of the second button we're monitoring
    MicroBitButtonEventConfiguration eventConfiguration;    // Do we want to generate high level event (clicks), or defer this to another service.

    uint16_t    otherSubButton(uint16_t b);
    int         isSubButtonPressed(uint16_t button);
    int         isSubButtonHeld(uint16_t button);
    int         isSubButtonSupressed(uint16_t button);
    void        setButtonState(uint16_t button, int value);
    void        setHoldState(uint16_t button, int value);
    void        setSupressedState(uint16_t button, int value);

    public:

    /**
      * Constructor. 
      * Create a representation of a vurtual button, that generates events based upon the combination
      * of two given buttons.
      * @param id the ID of the new MultiButton object.
      * @param button1 the ID of the first button to integrate.
      * @param button2 the ID of the second button to integrate.
      * @param name the physical pin on the processor that this butotn is connected to.
      *
      * Example:
      * @code 
      * multiButton(MICROBIT_ID_BUTTON_AB, MICROBIT_ID_BUTTON_A, MICROBIT_ID_BUTTON_B); 
      * @endcode
      *
      * Possible Events:
      * @code 
      * MICROBIT_BUTTON_EVT_DOWN
      * MICROBIT_BUTTON_EVT_UP
      * MICROBIT_BUTTON_EVT_CLICK
      * MICROBIT_BUTTON_EVT_LONG_CLICK
      * MICROBIT_BUTTON_EVT_DOUBLE_CLICK
      * MICROBIT_BUTTON_EVT_HOLD
      * @endcode
      */  
    MicroBitMultiButton(uint16_t id, uint16_t button1, uint16_t button2);    

    /**
      * Tests if this MultiButton is currently pressed.
      * @return 1 if both physical buttons are pressed simultaneously.
      *
      * Example:
      * @code 
      * if(uBit.buttonAB.isPressed())
      *     print("Pressed!");
      * @endcode
      */
    int isPressed();

    /**
     * Changes the event configuraiton of this button to the given value.
     * All subsequent events generated by this button will then be informed by this configuraiton.
     *
     * @param config The new configuration for this button. Legal values are MICROBIT_BUTTON_ALL_EVENTS or MICROBIT_BUTTON_SIMPLE_EVENTS.
     *
     * Example:
     * @code
     *
     * // Configure a button to generate all possible events.
     * uBit.buttonAB.setEventConfiguration(MICROBIT_BUTTON_ALL_EVENTS); 
     *
     * // Configure a button to suppress MICROBIT_BUTTON_EVT_CLICK and MICROBIT_BUTTON_EVT_LONG_CLICK events.
     * uBit.buttonAB.setEventConfiguration(MICROBIT_BUTTON_SIMPLE_EVENTS); 
     *
     * @endcode
     *
     * Possible Events:
     * @code
     * MICROBIT_BUTTON_EVT_DOWN
     * MICROBIT_BUTTON_EVT_UP
     * MICROBIT_BUTTON_EVT_CLICK
     * MICROBIT_BUTTON_EVT_LONG_CLICK
     * MICROBIT_BUTTON_EVT_HOLD
     * @endcode
     */
    void setEventConfiguration(MicroBitButtonEventConfiguration config);

    private:    
    void onButtonEvent(MicroBitEvent evt);
};

#endif
