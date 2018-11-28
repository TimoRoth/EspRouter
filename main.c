#include <stdio.h>

#include <net/gnrc/ipv6/nib.h>
#include <net/gnrc/ipv6.h>
#include <net/gnrc/netapi.h>
#include <net/gnrc/netif.h>
#include <net/ipv6/addr.h>
#include <net/netdev.h>
#include <net/netopt.h>

#include <shell.h>
#include <msg.h>

static msg_t main_msg_queue[16];

#define EXTERNAL_SUFFIX "80:2"
#define INTERNAL_SUFFIX "80:1"

static const gnrc_netif_t *border_interface = NULL;
static const gnrc_netif_t *wireless_interface = NULL;

static void set_ips(void)
{
    gnrc_netif_t *netif = NULL;

    border_interface = wireless_interface = NULL;

    while ((netif = gnrc_netif_iter(netif))) {
        bool is_wired = gnrc_netapi_get(netif->pid, NETOPT_IS_WIRED, 0, NULL, 0) == 1;
        ipv6_addr_t addr;

        if (!border_interface && is_wired) {
            ipv6_addr_from_str(&addr, BR_IPV6_PREFIX EXTERNAL_SUFFIX);
            gnrc_netif_ipv6_addr_add(netif, &addr, BR_IPV6_PREFIX_LEN, 0);
            gnrc_ipv6_nib_change_rtr_adv_iface(netif, false);
            border_interface = netif;
        } else if (!wireless_interface && !is_wired) {
            ipv6_addr_from_str(&addr, BR_IPV6_PREFIX INTERNAL_SUFFIX);
            gnrc_netif_ipv6_addr_add(netif, &addr, BR_IPV6_PREFIX_LEN, 0);
            gnrc_ipv6_nib_change_rtr_adv_iface(netif, true);
            wireless_interface = netif;
        }

        if (border_interface && wireless_interface)
            break;
    }
}

int main(void)
{
    msg_init_queue(main_msg_queue, sizeof(main_msg_queue) / sizeof(main_msg_queue[0]));

    set_ips();

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
