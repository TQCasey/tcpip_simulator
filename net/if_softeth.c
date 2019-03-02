#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/mbuf.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <machine/cpu.h>

#include <net/if.h>
#include <net/if_types.h>
#include <net/netisr.h>
#include <net/route.h>
#include <net/bpf.h>
#include <net/netisr.h>

#ifdef	INET
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/in_var.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#endif

#ifdef NS
#include <netns/ns.h>
#include <netns/ns_if.h>
#endif

#ifdef ISO
#include <netiso/iso.h>
#include <netiso/iso_var.h>
#endif

#include "if_softeth.h"

struct se_softc se_softc[NSE];

void
seattach(n)
int n;
{
	register struct se_softc *se = &se_softc[n];
	register struct ifnet *ifp = &se->se_if;
	
	se->se_addr[0] = 0x00;
	se->se_addr[1] = 0x19;
	se->se_addr[2] = 0x72;
	se->se_addr[3] = 0x09;
	se->se_addr[4] = 0x14;
	se->se_addr[5] = n;

	ifp->if_name = "se";
	ifp->if_unit = n;
	ifp->if_flags = IFF_BROADCAST | IFF_SIMPLEX | IFF_NOTRAILERS;
	ifp->if_output = ether_output;
	ifp->if_start = sestart;
	ifp->if_ioctl = seioctl;
//	ifp->if_init = seinit;
	ifp->if_snd.ifq_maxlen = 50;

	ifp->if_flags |= IFF_RUNNING;

	if_attach(ifp);
#if NBPFILTER > 0
	bpfattach(&ifp->if_bpf, ifp, DLT_NULL, sizeof(u_int));
#endif
}

//int 
//seinit(unit)
//int unit;
//{
//	
//}

int se_pcap_send(char *pkt, int len);

int
sestart(ifp)
struct ifnet *ifp;
{
	register struct se_softc *se = &se_softc[ifp->if_unit];
	struct mbuf *m0, *m;
//	int buffer;
	int len = 0, i, total,t;
	char *buf = se->buf;
	int offset = 0;

	if ((se->se_if.if_flags & IFF_RUNNING) == 0)
		return 0;

	IF_DEQUEUE(&se->se_if.if_snd, m);
	
	if (m == 0)
		return 0;

	len = i = 0;
	t = 0;
	for (m0 = m; m != 0; m = m->m_next)
		t += m->m_len;
	
	m = m0;
	total = t;
	for (m0 = m; m != 0; ) {
		bcopy(mtod(m, caddr_t), buf+offset, m->m_len);
		offset += m->m_len;
		t -= m->m_len;
		MFREE(m, m0);
		m = m0;
	}

	len = total;
	if (len < 64) len = 64;	// 以太网最小包长

		
	se_pcap_send(buf, len);

	return 0;
}

int
seioctl(ifp, cmd, data)
register struct ifnet *ifp;
int cmd;
caddr_t data;
{
	register struct ifaddr *ifa = (struct ifaddr *)data;
	struct se_softc *se = &se_softc[ifp->if_unit];
	struct ifreq *ifr = (struct ifreq *)data;
	int s = splimp(), error = 0;
	
	switch (cmd) {
		
	case SIOCSIFADDR:
		ifp->if_flags |= IFF_UP;
		
		switch (ifa->ifa_addr->sa_family) {
#ifdef INET
		case AF_INET:
//			((struct arpcom *)ifp)->ac_ipaddr.s_addr = ((struct sockaddr_in *) &ifr->ifr_addr)->sin_addr.s_addr;
//			arpwhohas((struct arpcom *)ifp, &((struct arpcom *)ifp)->ac_ipaddr);
			((struct arpcom *)ifp)->ac_ipaddr =
				IA_SIN(ifa)->sin_addr;
			arpwhohas((struct arpcom *)ifp, &IA_SIN(ifa)->sin_addr);
			break;
#endif
		default:
			break;
		}
		break;
		
	default:
		error = EINVAL;
		break;
	}
	splx(s);
	return (error);
}