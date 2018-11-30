#include <stdio.h>

#include <net/gnrc/ipv6/nib.h>
#include <net/gnrc/ipv6.h>
#include <net/gnrc/netapi.h>
#include <net/gnrc/netif.h>
#ifdef MODULE_GNRC_RPL
#include <net/gnrc/rpl.h>
#endif
#include <net/ipv6/addr.h>
#include <net/netdev.h>
#include <net/netopt.h>
#include <xtimer.h>
#include <shell.h>
#include <msg.h>

static msg_t main_msg_queue[16];

static const gnrc_netif_t *border_interface = NULL;
static const gnrc_netif_t *wireless_interface = NULL;

static int find_interfaces(void)
{
    int tmp;
    gnrc_netif_t *netif = NULL;

    border_interface = wireless_interface = NULL;

    while ((netif = gnrc_netif_iter(netif))) {
        bool is_wired = gnrc_netapi_get(netif->pid, NETOPT_IS_WIRED, 0, &tmp, sizeof(tmp)) == 1;

        if (!border_interface && is_wired) {
            border_interface = netif;
        } else if (!wireless_interface && !is_wired) {
            wireless_interface = netif;
        }

        if (border_interface && wireless_interface)
            break;
    }

    if (!border_interface || !wireless_interface) {
        printf("Unable to find interfaces.\n");
        return -1;
    }

    return 0;
}

static int set_ips(void)
{
    ipv6_addr_t addr, prefix, defroute = IPV6_ADDR_UNSPECIFIED;

    // Add configured outer address
    ipv6_addr_from_str(&addr, BR_IPV6_ADDR);
    if (gnrc_netif_ipv6_addr_add(border_interface, &addr, BR_IPV6_ADDR_LEN, 0) < 0) {
        printf("Failed setting outer address.\n");
        return -1;
    }

    // Add default route
    ipv6_addr_from_str(&addr, BR_IPV6_DEF_RT);
    if (gnrc_ipv6_nib_ft_add(&defroute, 0, &addr, border_interface->pid, 0) < 0) {
        printf("Failed setting default route.\n");
        return -1;
    }

    // Add inner address based on prefix and interface iid
    eui64_t iid;
    ipv6_addr_from_str(&prefix, BR_IPV6_PREFIX);
    if (gnrc_netapi_get(wireless_interface->pid, NETOPT_IPV6_IID, 0, &iid, sizeof(iid)) < 0) {
        printf("Failed getting wireless interface iid.\n");
        return -1;
    }
    ipv6_addr_set_aiid(&prefix, iid.uint8);
    if (gnrc_netif_ipv6_addr_add(wireless_interface, &addr, 64, 0) < 0) {
        printf("Failed setting outer address.\n");
        return -1;
    }

#if GNRC_IPV6_NIB_CONF_MULTIHOP_P6C
    // Add as authoritative border router
    if (gnrc_ipv6_nib_abr_add(&prefix) < 0) {
        printf("Failed adding prefix as authoritative border router.\n");
        return -1;
    }
#endif

#ifdef MODULE_GNRC_RPL
    // Configure rpl
    if (gnrc_rpl_init(wireless_interface->pid) < 0) {
        printf("Failed initializing rpl on wireless interface.\n");
        return -1;
    }
    gnrc_rpl_instance_t *inst = gnrc_rpl_instance_get(GNRC_RPL_DEFAULT_INSTANCE);
    if (inst)
        gnrc_rpl_instance_remove(inst);
    if (!gnrc_rpl_root_init(GNRC_RPL_DEFAULT_INSTANCE, &prefix, false, false)) {
        printf("Failed initializing rpl root.\n");
        return -1;
    }
#endif

    return 0;
}

int main(void)
{
    msg_init_queue(main_msg_queue, sizeof(main_msg_queue) / sizeof(main_msg_queue[0]));

    if (find_interfaces() >= 0)
        set_ips();

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
