#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtosc.h>

char buffer[256];

void check(int b, const char *msg, int loc)
{
    if(!b) {
        fprintf(stderr, "%s:%d\n", msg, loc);
        exit(1);
    }
}

int main()
{
    //Check the creation of a simple no arguments message
    check(sosc(buffer, 256, "/page/poge", "TIF") == 20,
            "Incorrect message length", __LINE__);

    //                         4     8      12   16
    check(!memcmp(buffer, "/pag""e/po""ge\0\0"",TIF""\0\0\0", 20),
            "Incorrect message contents", __LINE__);

    //printf("%d\n", msg_len(buffer));
    check(msg_len(buffer) == 16,
            "Incorrect detected message length", __LINE__);

    //Verify that it can be read properly
    check(nargs(buffer) == 3,
            "Incorrect number of arguments", __LINE__);

    check(type(buffer, 0) == 'T',
            "Incorrect truth argument", __LINE__);

    check(type(buffer, 1) == 'I',
            "Incorrect infinity argument", __LINE__);

    check(type(buffer, 2) == 'F',
            "Incorrect false argument", __LINE__);

    //Check the creation of a more complex message
    check(sosc(buffer, 256, "/testing", "is", 23, "this string") == 32,
            "Incorrect message length", __LINE__);
    //                       4     8        12      16         20    24    28    32
    check(!memcmp(buffer,"/tes""ting""\0\0\0\0"",is\0""\0\0\0\x17""this"" str""ing", 32),
            "Invalid OSC message", __LINE__);

    //Verify that buffer overflows will not occur
    check(sosc(buffer, 32, "/testing", "is", 23, "this string") == 32,
            "Incorrect message length", __LINE__);

    check(msg_len(buffer) == 32,
            "Invalid detected message length", __LINE__);

    check(sosc(buffer, 31, "/testing", "is", 23, "this string") == 0,
            "Incorrect message length", __LINE__);

    check(!*buffer,
            "Buffer was not cleared on possible overflow", __LINE__);

    return 0;
}
