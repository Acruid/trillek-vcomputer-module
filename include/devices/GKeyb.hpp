#pragma once
/**
 * Trillek Virtual Computer - GKeyb.hpp
 * Generic Western/Latin aeyboard
 * @see https://github.com/trillek-team/trillek-computer/blob/master/Keyboard.md
 */

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
          word_t a, b, c;

          std::deque<dword_t> keybuffer;    /// Stores the key events

          word_t int_msg;
          bool do_int;
      };

      enum SCANCODES { /// Scan codes of the events
        SCAN_UNKNOWN         = 0xFFFF,
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
        MOD_NONE            = 0x0,
        MOD_SHIFT           = 0x1,
        MOD_CTRL            = 0x2,
        MOD_ALTGR           = 0x4
      };

      static const size_t BSIZE = 64; /// Internal buffer size
      /**
       * Genertic Keyboard
       * Western / Latin generic keyboard
       */
      class GKeyboardDev : public IDevice {
        protected:
          word_t a, b, c;

          std::deque<dword_t> keybuffer;    /// Stores the key events

          word_t int_msg;
          bool do_int;

        public:

          GKeyboardDev ();

          virtual ~GKeyboardDev ();

          virtual void Reset ();

          /**
           * Sends (writes to CMD register) a command to the device
           * @param cmd Command value to send
           */
          virtual void SendCMD (word_t cmd);

          virtual void A (word_t val) { a = val; }
          virtual void B (word_t val) { b = val; }
          virtual void C (word_t val) { a = val; }

          virtual word_t A () { return a; }
          virtual word_t B () { return b; }
          virtual word_t C () { return c; }
          virtual word_t E () { return keybuffer.size(); }

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

          virtual bool DoesInterrupt(word_t& msg);

          virtual void IACK ();

          virtual void GetState (void* ptr, std::size_t& size) const;

          virtual bool SetState (const void* ptr, std::size_t size);

          /* API exterior to the Virtual Computer (affects or afected by stuff outside of the computer) */

          /**
           * Push a new key event to the keyboard buffer.
           * @param scancode
           * @param keycode
           * @param status Status bits
           * @return False if the buffer is full
           */
          bool SendKeyEvent (word_t scancode, unsigned char keycode, byte_t status) {
            if (keybuffer.size() >= BSIZE) {
              return false;
            }

            dword_t keyevent = ((status & 7) << 24) | (keycode << 16) | scancode;
            keybuffer.push_back(keyevent);

            return true;
          }

          /**
           * Enforces a Push a new key event to the keyboard buffer, droping
           * the most old event stored if is full.
           * @param scancode
           * @param keycode
           * @param status Status bits
           */
          void EnforceSendKeyEvent (word_t scancode, unsigned char keycode, byte_t status) {
            if (keybuffer.size() >= BSIZE) {
              keybuffer.pop_front();
            }

            dword_t keyevent = ((status & 7) << 24) | (keycode << 16) | scancode;
            keybuffer.push_back(keyevent);
          }

      };


    } // End of namespace tda
  } // End of namespace dev
} // End of namespace vm

