/**
 * Trillek Virtual Computer - TDA.cpp
 * Nya Elektriska Text Display Adapter
 * @see https://github.com/trillek-team/trillek-computer/blob/master/TDA.md
 */

#include "devices/TDA.hpp"
#include "VSFix.hpp"

#include <algorithm>
#include <cassert>

namespace vm {
	namespace dev {
		namespace tda {

      void TDAtoRGBATexture (const TDAScreen& screen, dword_t* texture) {
        assert(texture != nullptr);

        const byte_t* font = ROM_FONT;
        if (screen.user_font) {
          font = (byte_t*) screen.font_buffer;
        }

        // TODO Rewrite this to be more efficient and cache friendly, as now
        // writes "jumping" in the output texture

        // Reads one by one each character of the text buffer
        for (unsigned row = 0; row < HEIGHT_CHARS; row++) {
          for (unsigned col = 0; col < WIDTH_CHARS; col++) {

            std::size_t addr = col + (WIDTH_CHARS * row);
            byte_t c = screen.txt_buffer[addr]; // character

            // Get Ink (fg) and Paper (bg) colors
            dword_t fg = (screen.txt_buffer[addr] >> 8) & 0x0F; // Bits 8-11
            dword_t bg = (screen.txt_buffer[addr] >> 12)& 0x0F; // bits 12-15

            // Paint the texture
            byte_t pixels;
            for (unsigned y= 0; y < 8 ; y++) {
              pixels = font[c*8 + y];
              for (unsigned x= 0; x < 8 ; x++) {
                addr = x + col*8 + (40*8 * (y + row*8)); // Addres of the pixel in the buffer
                if ((pixels & (1 << (7-x))) != 0) { // Active, uses the Ink (fg)
                  texture[addr] = PALETTE[fg];
                } else {                        // Unactive, uses the Paper (bg)
                  texture[addr] = PALETTE[bg];
                }
              }
            }

          }
        }
			}



      TDADev::TDADev () : buffer_ptr(0), font_ptr(0), vsync_msg(0), do_vsync(false) {
      }

      TDADev::~TDADev() {
      }

      void TDADev::Reset () {
        this->buffer_ptr  = 0;
        this->font_ptr    = 0;
        this->vsync_msg   = 0;
        this->a           = 0;
        this->b           = 0;
        this->do_vsync    = false;
      }

      void TDADev::SendCMD (word_t cmd) {
        dword_t tmp;
        switch (cmd) {
          case 0x0000: // Map Buffer
            tmp = ((b << 16) | a);
            if (tmp + TXT_BUFFER_SIZE < vcomp->RamSize()) {
              buffer_ptr = tmp;
            }
            break;

          case 0x0001: // Map Font
            tmp = ((b << 16) | a);
            if (tmp + FONT_BUFFER_SIZE < vcomp->RamSize()) {
              font_ptr = tmp;
            }
            break;

          case 0x0002: // Set Int
            vsync_msg = a;
            break;

          default:
            break;
        }
      }

      bool TDADev::DoesInterrupt(word_t& msg) {
        if (do_vsync && vsync_msg != 0x0000) {
          msg = vsync_msg;
          return true;
        }
        return false;
      }

      void TDADev::IACK () {
        do_vsync = false; // Acepted, so we can forgot now of sending it again
      }

      void TDADev::GetState (void* ptr, std::size_t& size) const {
        if (ptr != nullptr && size >= sizeof(TDAState)) {
          auto state = (TDAState*) ptr;
          state->buffer_ptr = this->buffer_ptr;
          state->font_ptr   = this->font_ptr;
          state->vsync_msg  = this->vsync_msg;
          state->a          = this->a;
          state->b          = this->b;

          state->do_vsync   = this->do_vsync;
        }
      }

      bool TDADev::SetState (const void* ptr, std::size_t size) {
        if (ptr != nullptr && size >= sizeof(TDAState)) { // Sanity check
          auto state = (const TDAState*) ptr;
          this->buffer_ptr  = state->buffer_ptr;
          this->font_ptr    = state->font_ptr;
          this->vsync_msg   = state->vsync_msg;
          this->a           = state->a;
          this->b           = state->b;

          this->do_vsync    = state->do_vsync;

          return true;
        }

        return false;
      }


		} // End of namespace tda
	} // End of namespace dev
} // End of namespace vm

