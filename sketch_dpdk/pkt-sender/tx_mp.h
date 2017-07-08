#ifndef TX_MP
#define TX_MP

#include <rte_eal.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include "pm.h"

#define MIN_TX_POOL_SIZE 2048
#define MAX_TX_CACHE_SIZE 512

struct rte_mempool* tx_mempool_create(int n, int lcore_id);

/*int tx_mempool_alloc_bulk(struct rte_mempool *mp, struct rte_mbuf *mbuf[], int n);*/

//void tx_mempool_free_bulk(struct rte_mbuf *mbuf[], int n);

/*static inline void tx_mbuf_init_noreset(struct rte_mbuf *m)
{
    m->next = NULL;
    m->nb_segs = 1;
    m->port = 0xff;
    m->data_off = (RTE_PKTMBUF_HEADROOM <= m->buf_len) ? RTE_PKTMBUF_HEADROOM : m->buf_len;
    RTE_MBUF_ASSERT(rte_mbuf_refcnt_read(m) == 0);
    rte_mbuf_refcnt_set(m, 1);
}*/

struct rte_mbuf* generate_mbuf(struct packet_model pm, struct rte_mempool *mp, uint32_t pkt_length);

#endif
