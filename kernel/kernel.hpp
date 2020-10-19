#ifndef KERNEL_HPP
#define KERNEL_HPP

#include "kernel/display.hpp"

class Kernel
{
  private:
    Display *display;

  public:

    // Initialize kernel services and run kernel
    Kernel();

    // Shutdown kernel services and stop kernel
    ~Kernel();

    // Open kernel shell that allows for user interaction
    void openShell();
};

// Global pointer to the kernel object
Kernel *theKernel;

#endif // KERNEL_HPP
