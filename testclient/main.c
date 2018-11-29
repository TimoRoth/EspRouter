#include <stdio.h>

#include <xtimer.h>
#include <shell.h>
#include <msg.h>

static msg_t main_msg_queue[16];

int main(void)
{
    msg_init_queue(main_msg_queue, sizeof(main_msg_queue) / sizeof(main_msg_queue[0]));

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
