#include "terminal.hpp"

void Terminal::output_callback(const char* s, size_t len, void* user)
{
    write(*(int*)user, s, len);
}

int Terminal::damage(VTermRect rect, void *user)
{
    return ((Terminal*)user)->damage(rect.start_row, rect.start_col, rect.end_row, rect.end_col);
}

int Terminal::moverect(VTermRect dest, VTermRect src, void *user)
{
    return ((Terminal*)user)->moverect(dest, src);
}

int Terminal::movecursor(VTermPos pos, VTermPos oldpos, int visible, void *user)
{
    return ((Terminal*)user)->movecursor(pos, oldpos, visible);
}

int Terminal::settermprop(VTermProp prop, VTermValue *val, void *user)
{
    return ((Terminal*)user)->settermprop(prop, val);
}

int Terminal::bell(void *user)
{
    return ((Terminal*)user)->bell();
}

int Terminal::resize(int rows, int cols, void *user)
{
    return ((Terminal*)user)->resize(rows, cols);
}

int Terminal::sb_pushline(int cols, const VTermScreenCell *cells, void *user)
{
    return ((Terminal*)user)->sb_pushline(cols, cells);
}

int Terminal::sb_popline(int cols, VTermScreenCell *cells, void *user)
{
    return ((Terminal*)user)->sb_popline(cols, cells);
}
