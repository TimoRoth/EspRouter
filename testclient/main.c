#include <stdio.h>

#include <net/gnrc/ipv6/nib.h>
#include <net/gnrc/ipv6.h>
#include <net/gnrc/netapi.h>
#include <net/gnrc/netif.h>
#include <net/ipv6/addr.h>
#include <net/netdev.h>
#include <net/netopt.h>

#include <xtimer.h>
#include <shell.h>
#include <msg.h>

static msg_t main_msg_queue[16];

int init_ip(void)
{
#if defined(BR_IPV6_PREFIX) && defined(BR_IPV6_DEFRTR)
    // Assuming client with single network interface, so blindly using first (and only).
    gnrc_netif_t *netif = gnrc_netif_iter(NULL);

    // Add configured outer address
    ipv6_addr_t addr;
    eui64_t iid;
    ipv6_addr_from_str(&addr, BR_IPV6_PREFIX);
    if (gnrc_netapi_get(netif->pid, NETOPT_IPV6_IID, 0, &iid, sizeof(iid)) < 0) {
        printf("Failed getting wireless interface iid.\n");
        return -1;
    }
    ipv6_addr_set_aiid(&addr, iid.uint8);
    if (gnrc_netif_ipv6_addr_add(netif, &addr, 64, 0) < 0) {
        printf("Failed setting address.\n");
        return -1;
    }

    gnrc_ipv6_nib_change_rtr_adv_iface(netif, false);

    // Add default route
    ipv6_addr_t defroute = IPV6_ADDR_UNSPECIFIED;
    ipv6_addr_from_str(&addr, BR_IPV6_DEFRTR);
    if (gnrc_ipv6_nib_ft_add(&defroute, 0, &addr, netif->pid, 0) < 0) {
        printf("Failed setting default route.\n");
        return -1;
    }
#endif

    return 0;
}

int main(void)
{
    msg_init_queue(main_msg_queue, sizeof(main_msg_queue) / sizeof(main_msg_queue[0]));

    init_ip();

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
