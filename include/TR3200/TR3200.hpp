#pragma once
/**
 * Trillek Virtual Computer - TR3200.hpp
 * Implementation of the TR3200 CPU
 */

#include "ICPU.hpp"
#include "VComputer.hpp"

namespace vm {
  namespace cpu {

    static unsigned const TR3200_NGPRS = 16;


    class TR3200 : public ICPU {
      public:

        /**
         * Builds a TR3200 CPU
         * @param clock CPU clock speed
         */
        TR3200(unsigned clock = 100000);

        virtual ~TR3200();

        /**
         * Returns CPU clock speed in Hz
         */
        virtual unsigned Clock() {
          return this->cpu_clock;
        }

        /**
         * Resets the CPU state
         */
        virtual void Reset();

        /**
         * Executes a singe instrucction of the CPU
         * @return Number of CPU cycles used
         */
        unsigned Step();

        /**
         * Executes one or more CPU clock cycles
         * @param n Number of cycles (default=1)
         */
        void Tick(unsigned n=1);

        /**
         * Sends an interrupt to the CPU.
         * @param msg Interrupt message
         * @return True if the CPU accepts the interrupt
         */
        bool SendInterrupt (word_t msg);

        /**
         * Writes a copy of CPU state in a chunk of memory pointer by ptr.
         * @param ptr Pointer were to write
         * @param size Size of the chunk of memory were can write. If is
         * sucesfull, it will be set to the size of the write data.
         */
        virtual void GetState (void* ptr, std::size_t& size) const;

        /**
         * Sets the CPU state.
         * @param ptr Pointer were read the state information
         * @param size Size of the chunk of memory were will read.
         * @return True if can read the State data from the pointer.
         */
        virtual bool SetState (const void* ptr, std::size_t size);

      protected:

        unsigned cpu_clock;       /// CPU clock speed

        dword_t r[TR3200_NGPRS];  /// Registers
        dword_t pc;               /// Program Counter

        unsigned wait_cycles;     /// Nº of cycles that need to finish the actual instruction

        word_t int_msg;           /// Interrupt message

        bool interrupt;           /// Is atending an interrupt ?
        bool step_mode;           /// Is in step mode execution ?
        bool skiping;             /// Is skiping an instruction ?
        bool sleeping;            /// Is sleping the CPU ?

        /**
         * Does the real work of executing a instrucction
         * @param Numvber of cycles tha requires to execute an instrucction
         */
        virtual unsigned RealStep();

        /**
         * Process if an interrupt is waiting
         */
        void ProcessInterrupt();

    };

    /**
     * Structure that stores the TR3200 CPU state in any moment
     */
    struct TR3200State {
      dword_t r[TR3200_NGPRS];  /// Registers
      dword_t pc;               /// Program Counter

      unsigned wait_cycles;     /// Nº of cycles that need to finish the actual instruction

      word_t int_msg;           /// Interrupt message

      bool interrupt;           /// Is atending an interrupt ?
      bool step_mode;           /// Is in step mode execution ?
      bool skiping;             /// Is skiping an instruction ?
      bool sleeping;            /// Is sleping the CPU ?
    };

  } // End of namespace cpu
} // End of namespace vm

