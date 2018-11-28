#include <stdio.h>

#include <shell.h>
#include <msg.h>

#include "uhcpd.h"

static msg_t main_msg_queue[64];

int main(void)
{
    msg_init_queue(main_msg_queue, sizeof(main_msg_queue) / sizeof(main_msg_queue[0]));

    init_uhcpd();

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
