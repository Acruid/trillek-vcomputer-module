/**
 * Trillek Virtual Computer - Timer.cpp
 * Implementation of Timer embed device
 */

#include "Timer.hpp"

namespace vm {

  Timer::Timer () {
  }

  Timer::~Timer () {
  }

  void Timer::Reset() {
    tmr0 = 0;
    tmr1 = 0;

    re0 = 0;
    re1 = 0;

    cfg = 0;

    do_int_tmr0 = false;
    do_int_tmr1 = false;
  }

  void Timer::Tick (unsigned n, const double delta) {
    dword_t tmp;
    if ((cfg & 1) != 0) {
      tmp = tmr0;
      tmr0 -= n;
      if (tmr0 > tmp) { // Underflow of TMR0
        tmr0 = re0 - (0xFFFFFFFF - tmr0);
        do_int_tmr0 = (cfg & 2) != 0;
      }
    }

    if ((cfg & 8) != 0) {
      tmp = tmr1;
      tmr1 -= n;
      if (tmr1 > tmp) { // Underflow of TMR1
        tmr1 = re1 - (0xFFFFFFFF - tmr1);
        do_int_tmr1 = (cfg & 16) != 0;
      }
    }
  }

  bool Timer::DoesInterrupt(word_t& msg) {
    if ( ((cfg & 2) != 0) && do_int_tmr0) { // TMR0 does an interrupt
      msg = 0x0001;
      return true;

    } else if (((cfg & 16) != 0) && do_int_tmr1) { // TMR1 does an interrupt
      msg = 0x1001;
      return true;
    }
    return false;
  }

  void Timer::IACK () {
    // TODO This could raise potential problems...
    if (do_int_tmr0) {
      do_int_tmr0 = false;
    } else {
      do_int_tmr1 = false;
    }
  }

  byte_t Timer::ReadB (dword_t addr) {
    switch (addr) {
      case 0x11E000:
        return tmr0;
        break;

      case 0x11E001:
        return tmr0 >> 8;
        break;

      case 0x11E002:
        return tmr0 >> 16;
        break;

      case 0x11E003:
        return tmr0 >> 24;
        break;


      case 0x11E004:
        return re0;
        break;

      case 0x11E005:
        return re0 >> 8;
        break;

      case 0x11E006:
        return re0 >> 16;
        break;

      case 0x11E007:
        return re0 >> 24;
        break;


      case 0x11E008:
        return tmr1;
        break;

      case 0x11E009:
        return tmr1 >> 8;
        break;

      case 0x11E00A:
        return tmr1 >> 16;
        break;

      case 0x11E00B:
        return tmr1 >> 24;
        break;


      case 0x11E00C:
        return re1;
        break;

      case 0x11E00D:
        return re1 >> 8;
        break;

      case 0x11E00E:
        return re1 >> 16;
        break;

      case 0x11E00F:
        return re1 >> 24;
        break;

      case 0x11E010:
        return cfg;
        break;

      default:
        return 0;
    }
  }

  word_t Timer::ReadW (dword_t addr) {
    switch (addr) {
      case 0x11E000:
        return tmr0;
        break;

      case 0x11E002:
        return tmr0 >> 16;
        break;

      case 0x11E004:
        return re0;
        break;

      case 0x11E006:
        return re0 >> 16;
        break;

      case 0x11E008:
        return tmr1;
        break;

      case 0x11E00A:
        return tmr1 >> 16;
        break;

      case 0x11E00C:
        return re1;
        break;

      case 0x11E00E:
        return re1 >> 16;
        break;

      case 0x11E010:
        return cfg;
        break;

      default:
        return this->ReadB(addr) | (this->ReadB(addr+1) << 8);
    }
  }

  dword_t Timer::ReadDW (dword_t addr) {
    switch (addr) {
      case 0x11E000:
        return tmr0;
        break;

      case 0x11E004:
        return re0;
        break;

      case 0x11E008:
        return tmr1;
        break;

      case 0x11E00C:
        return re1;
        break;

      case 0x11E010:
        return cfg;
        break;

      default:
        return this->ReadW(addr) | (this->ReadW(addr+2) << 16);
    }
  }

  void Timer::WriteB (dword_t addr, byte_t val) {
    switch (addr) {
      case 0x11E000:
        tmr0 = (tmr0 & 0xFFFFFF00) | val;
        break;

      case 0x11E001:
        tmr0 = (tmr0 & 0xFFFF00FF) | val << 8;
        break;

      case 0x11E002:
        tmr0 = (tmr0 & 0xFF00FFFF) | val << 16;
        break;

      case 0x11E003:
        tmr0 = (tmr0 & 0x00FFFFFF) | val << 24;
        break;


      case 0x11E004:
        re0 = (re0 & 0xFFFFFF00) | val;
        break;

      case 0x11E005:
        re0 = (re0 & 0xFFFF00FF) | val << 8;
        break;

      case 0x11E006:
        re0 = (re0 & 0xFF00FFFF) | val << 16;
        break;

      case 0x11E007:
        re0 = (re0 & 0x00FFFFFF) | val << 24;
        break;


      case 0x11E008:
        tmr1 = (tmr1 & 0xFFFFFF00) | val;
        break;

      case 0x11E009:
        tmr1 = (tmr1 & 0xFFFF00FF) | val << 8;
        break;

      case 0x11E00A:
        tmr1 = (tmr1 & 0xFF00FFFF) | val << 16;
        break;

      case 0x11E00B:
        tmr1 = (tmr1 & 0x00FFFFFF) | val << 24;
        break;


      case 0x11E00C:
        re1 = (re1 & 0xFFFFFF00) | val;
        break;

      case 0x11E00D:
        re1 = (re1 & 0xFFFF00FF) | val << 8;
        break;

      case 0x11E00E:
        re1 = (re1 & 0xFF00FFFF) | val << 16;
        break;

      case 0x11E00F:
        re1 = (re1 & 0x00FFFFFF) | val << 24;
        break;

      case 0x11E010:
        cfg = val;
        break;

      default:
        break;
    }
  }

  void Timer::WriteW (dword_t addr, word_t val) {
    switch (addr) {
      case 0x11E000:
        tmr0 = (tmr0 & 0xFFFF0000) | val;
        break;

      case 0x11E002:
        tmr0 = (tmr0 & 0x0000FFFF) | val << 16;
        break;

      case 0x11E004:
        re0 = (re0 & 0xFFFF0000) | val;
        break;

      case 0x11E006:
        re0 = (re0 & 0x0000FFFF) | val << 16;
        break;

      case 0x11E008:
        tmr1 = (tmr1 & 0xFFFF0000) | val;
        break;

      case 0x11E00A:
        tmr1 = (tmr1 & 0x0000FFFF) | val << 16;
        break;

      case 0x11E00C:
        re1 = (re1 & 0xFFFF0000) | val;
        break;

      case 0x11E00E:
        re1 = (re1 & 0x0000FFFF) | val << 16;
        break;

      case 0x11E010:
        cfg = val;
        break;

      default:
        this->WriteB(addr, val);
        this->WriteB(addr+1, val >> 8);
    }
  }

  void Timer::WriteDW (dword_t addr, dword_t val) {
    switch (addr) {
      case 0x11E000:
        tmr0 = val;
        break;

      case 0x11E004:
        re0 = val;
        break;

      case 0x11E008:
        tmr1 = tmr1;
        break;

      case 0x11E00C:
        re1 = re1;
        break;

      case 0x11E010:
        cfg = val;
        break;

      default:
        this->WriteW(addr, val);
        this->WriteW(addr+2, val >> 16);
    }
  }


} // End of namespace vm

