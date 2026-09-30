/***
  This file is part of PulseAudio.

  Copyright 2004-2006 Lennart Poettering
  Copyright 2006 Pierre Ossman <ossman@cendio.se> for Cendio AB

  PulseAudio is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  PulseAudio is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with PulseAudio; if not, see <http://www.gnu.org/licenses/>.
***/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <sys/types.h>
#include <string.h>

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_NETINET_IN_SYSTM_H
#include <netinet/in_systm.h>
#endif
#ifdef HAVE_NETINET_IP_H
#include <netinet/ip.h>
#endif

#include <pulse/xmalloc.h>

#include <pulsecore/core-util.h>
#include <pulsecore/llist.h>
#include <pulsecore/log.h>
#include <pulsecore/macro.h>
#include <pulsecore/socket.h>
#include <pulsecore/arpa-inet.h>

#include "ipacl.h"

#define IPS_STATIC static
#include "../../../xorg-server/os/ipv6scan.c"
#undef IPS_STATIC

struct pa_ip_acl {
    struct iprange_list *iprange_list;
};

pa_ip_acl* pa_ip_acl_new(const char *s) {
    pa_ip_acl *acl;

    pa_assert(s);

    acl = pa_xnew(pa_ip_acl, 1);
    acl->iprange_list = iprange_parse(s, NULL);
    if (!acl->iprange_list) {
        pa_xfree(acl);
        return NULL;
    }

    return acl;
}

void pa_ip_acl_free(pa_ip_acl *acl) {
    pa_assert(acl);

    iprange_free (acl->iprange_list);

    pa_xfree(acl);
}

int pa_ip_acl_check(pa_ip_acl *acl, int fd) {
    struct sockaddr_storage sa;
    socklen_t salen;
    int r = 0;

    pa_assert(acl);
    pa_assert(fd >= 0);

    salen = sizeof(sa);
    if (getpeername(fd, (struct sockaddr*) &sa, &salen) < 0)
        return -1;

#ifdef HAVE_IPV6
    if (sa.ss_family != AF_INET && sa.ss_family != AF_INET6)
#else
    if (sa.ss_family != AF_INET)
#endif
        return -1;

    if (sa.ss_family == AF_INET) {
        struct sockaddr_in *sai = (struct sockaddr_in*) &sa;
        if (salen != sizeof(struct sockaddr_in))
            return -1;
        r = iprange_match(acl->iprange_list, &sai->sin_addr, sizeof(sai->sin_addr));
    }
#ifdef HAVE_IPV6
    else {
        struct sockaddr_in6 *sai = (struct sockaddr_in6*) &sa;
        if (salen != sizeof(struct sockaddr_in6))
            return -1;
        r = iprange_match(acl->iprange_list, &sai->sin6_addr, sizeof(sai->sin6_addr));
    }
#endif

    return r != 0;
}

