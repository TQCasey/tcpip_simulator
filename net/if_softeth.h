#ifndef __IF_SOFTETH_H__
#define __IF_SOFTETH_H__

struct se_softc {
	struct arpcom se_ac;
#define se_if	se_ac.ac_if
#define se_addr	se_ac.ac_enaddr
	u_char buf[2048];
};

extern struct se_softc se_softc[NSE];

#endif