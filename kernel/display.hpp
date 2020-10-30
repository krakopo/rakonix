#ifndef DISPLAY_HPP
#define DISPLAY_HPP

class Display
{
  public:
    virtual void print(const char *string) = 0;
    virtual void print(const char *string, int bgcolour, int fgcolour) = 0;
    virtual void set_text_colour(int bgcolour, int fgcolour) = 0;
    virtual void reset_text_colour() = 0;
};

class DisplayService
{
  public:
    static Display *getDisplay();
};

#endif // DISPLAY_HPP
