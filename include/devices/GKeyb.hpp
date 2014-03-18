#pragma once
/**
 * Trillek Virtual Computer - GKeyb.hpp
 * Generic Western/Latin Kaeyboard
 * @see https://github.com/trillek-team/trillek-computer/blob/master/Keyboard.md
 */

#ifndef __GKEYBOARD_DEV_HPP__
#define __GKEYBOARD_DEV_HPP__ 1

#include "Types.hpp"
#include "VComputer.hpp"

#include <deque>
#include <cstdio>

namespace vm {
  namespace dev {
    namespace gkeyboard {

      /**
       * Structure to store a snapshot of the device state
       */
      struct GKeyboardState {
        public:


          bool do_int;
      };

      /**
       * Keyboard mode
       */
      enum KeybMode {
        KEY,
        RAW,
        RAW_KEY
      };

      enum SCANCODES { /// Scan codes of the events
        SCAN_UNKNOWN         = 0x3FF,
        SCAN_NULL            = 0,      // Read value when the buffer is empty
        SCAN_SPACE           = 32,
        SCAN_APOSTROPHE      = 39,     // '
        SCAN_COMMA           = 44,     // ,
        SCAN_MINUS           = 45,     // -
        SCAN_PERIOD          = 46,     // .
        SCAN_SLASH           = 47,     // /
        SCAN_0               = 48,
        SCAN_1               = 49,
        SCAN_2               = 50,
        SCAN_3               = 51,
        SCAN_4               = 52,
        SCAN_5               = 53,
        SCAN_6               = 54,
        SCAN_7               = 55,
        SCAN_8               = 56,
        SCAN_9               = 57,
        SCAN_SEMICOLON       = 59,     // ;
        SCAN_EQUAL           = 61,     // =
        SCAN_A               = 65,
        SCAN_B               = 66,
        SCAN_C               = 67,
        SCAN_D               = 68,
        SCAN_E               = 69,
        SCAN_F               = 70,
        SCAN_G               = 71,
        SCAN_H               = 72,
        SCAN_I               = 73,
        SCAN_J               = 74,
        SCAN_K               = 75,
        SCAN_L               = 76,
        SCAN_M               = 77,
        SCAN_N               = 78,
        SCAN_O               = 79,
        SCAN_P               = 80,
        SCAN_Q               = 81,
        SCAN_R               = 82,
        SCAN_S               = 83,
        SCAN_T               = 84,
        SCAN_U               = 85,
        SCAN_V               = 86,
        SCAN_W               = 87,
        SCAN_X               = 88,
        SCAN_Y               = 89,
        SCAN_Z               = 90,
        SCAN_LEFT_BRACKET    = 91,     // [
        SCAN_BACKSLASH       = 92,
        SCAN_RIGHT_BRACKET   = 93,     // ]
        SCAN_GRAVE_ACCENT    = 96,     // `
        SCAN_WORLD_1         = 161,    // non-US #1
        SCAN_WORLD_2         = 162,    // non-US #2
        SCAN_ESCAPE          = 256,
        SCAN_ENTER           = 257,
        SCAN_TAB             = 258,
        SCAN_BACKSPACE       = 259,
        SCAN_INSERT          = 260,
        SCAN_DELETE          = 261,
        SCAN_RIGHT           = 262,
        SCAN_LEFT            = 263,
        SCAN_DOWN            = 264,
        SCAN_UP              = 265,
        SCAN_LEFT_SHIFT      = 340,
        SCAN_LEFT_CONTROL    = 341,
        SCAN_LEFT_ALT        = 342,
        SCAN_RIGHT_SHIFT     = 344,
        SCAN_RIGHT_CONTROL   = 345,
        SCAN_RIGHT_ALT       = 346
      };

      enum KEYCODES { /// Key codes of the events
        KEY_NONE            = 0x00,
        KEY_UNKNOW          = 0x01,
        KEY_DELETE          = 0x05,
        KEY_ALT             = 0x06,
        KEY_BACKSPACE       = 0x08,
        KEY_TAB             = 0x09,
        KEY_RETURN          = 0x0D,
        KEY_SHIFT           = 0x0E,
        KEY_CONTROL         = 0x0F,
        KEY_INSERT          = 0x10,
        KEY_ARROW_UP        = 0x12,
        KEY_ARROW_DOWN      = 0x13,
        KEY_ARROW_LEFT      = 0x14,
        KEY_ARROW_RIGHT     = 0x15,
        KEY_ESC             = 0x1B,
        KEY_SPACEBAR        = 0x20
      };

      enum KEY_MODS { /// Key modifier
        MOD_SHIFT           = 0x20,
        MOD_CTRL            = 0x40,
        MOD_ALTGR           = 0x80
      };

      static const size_t BSIZE = 64; /// Internal buffer size
      /**
       * Genertic Keyboard
       * Western / Latin generic keyboard
       */
      class GKeyboardDev : public IDevice {
        public:

          GKeyboardDev () : int_msg(0), do_int(false), mode(KeybMode::KEY) {
            keybuffer.clear();
          }

          virtual ~GKeyboardDev() {
          }

          virtual void Reset () {
            int_msg = 0;
            a = 0;
            b = 0;
            do_int = false;

            mode = KeybMode::KEY;
            keybuffer.clear();
          }

          /**
           * Sends (writes to CMD register) a command to the device
           * @param cmd Command value to send
           */
          virtual void SendCMD (word_t cmd) {
            switch (cmd) {
              case 0x0000: // CLR_BUFFER
                keybuffer.clear();
                break;

              case 0x0002: // PUSH_KEY
                //keybuffer.push((b << 8) | a);
                break;

              case 0x0003: // SET_INT
                int_msg = a;
                break;

              case 0x0005: // SET_MODE
                mode = (KeybMode)(a & 3);
                break;

              default:
                break;
            }
          }

          virtual void A (word_t val) { a = val; }
          virtual void B (word_t val) { b = val; }

          virtual word_t A () { return a; }
          virtual word_t B () { return b; }

          /**
           * Device Type
           */
          virtual byte_t DevType() const {
            return 0x03; // HID
          }

          /**
           * Device SubType
           */
          virtual byte_t DevSubType() const {
            return 0x01; // Western / Latin Keyboard
          }

          /**
           * Device ID
           */
          virtual byte_t DevID() const {
            return 0x01; //
          }

          /**
           * Device Vendor ID
           */
          virtual dword_t DevVendorID() const {
            return 0x00000000; // Generic
          }

          virtual bool DoesInterrupt(word_t& msg) {
            if (do_int && int_msg != 0x0000) {
              msg = int_msg;
              return true;
            }
            return false;
          }

          virtual void IACK () {
            do_int = false; // Acepted, so we can forgot now of sending it again
          }

          virtual void GetState (void* ptr, std::size_t& size) const {
            if (ptr != nullptr && size >= sizeof(GKeyboardState)) {
              auto state = (GKeyboardState*) ptr;
            }
          }

          virtual bool SetState (const void* ptr, std::size_t size) {
            if (ptr != nullptr && size >= sizeof(GKeyboardState)) { // Sanity check
              auto state = (const GKeyboardState*) ptr;

              return true;
            }

            return false;
          }

          /* API exterior to the Virtual Computer (affects or afected by stuff outside of the computer) */

        protected:
          word_t int_msg;
          word_t a, b;
          bool do_int;

          KeybMode mode;                  /// Keyboard mode
          std::deque<word_t> keybuffer;   /// Stores the key events
      };


    } // End of namespace tda
  } // End of namespace dev
} // End of namespace vm

#endif
