#include "tx_mp.h"
#include <rte_lcore.h>
#include <stdio.h>
#include <string.h>
#include <rte_eal.h>
#include <rte_common.h>
#include <rte_mempool.h>
#include <rte_memory.h>
#include <rte_mbuf.h>

inline struct rte_mbuf* generate_mbuf(struct packet_model pm, struct rte_mempool *mp, uint32_t pkt_length)
{
    struct rte_mbuf *m;

    m = rte_pktmbuf_alloc(mp);
    if(m == NULL)
    {
        rte_exit(-1, "mempool is empty!\n");
    }
    char *data;
    if(pm.is_vxlan)
    {
        data = rte_pktmbuf_append(m, sizeof(pm.vxlan));
        if(data == NULL)
        {
            rte_exit(-1, "mbuf append vxlan hdr failed!\n");
        }
        rte_memcpy(data, (char*)&(pm.vxlan), sizeof(pm.vxlan));
    }
    if(pm.is_udp)
    {
        data = rte_pktmbuf_append(m, sizeof(pm.udp));
        if(data == NULL)
        {
            rte_exit(-1, "mbuf append udp hdr failed!\n");
        }
        rte_memcpy(data, &(pm.udp), sizeof(pm.udp));
        data = rte_pktmbuf_append(m, pkt_length - 46);
        if(data == NULL)
        {
            rte_exit(-1, "mbuf append udp data failed!\n");
        }
    }
    else
    {
        data = rte_pktmbuf_append(m, sizeof(pm.tcp));
        if(data == NULL)
        {
            rte_exit(-1, "mbuf append tcp hdr failed!\n");
        }
        rte_memcpy(data, &(pm.tcp), sizeof(pm.tcp));
        data = rte_pktmbuf_append(m, pkt_length - 58);
        if(data == NULL)
        {
            rte_exit(-1, "mbuf append tcp data failed!\n");
        }
    }
    return m;
}

struct rte_mempool* tx_mempool_create(int n, int lcore_id)
{
    char name[64];
    struct rte_mempool *mp;
    struct rte_mbuf *m;
    int poolsz, i, pmi;
    snprintf(name, 64, "tx_mempool_lcore(%d)", lcore_id);
    mp = rte_pktmbuf_pool_create(name, n, MAX_TX_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_DATAROOM, rte_lcore_to_socket_id(lcore_id));
    if(mp == NULL)
    {
        rte_exit(EINVAL, "create mempool for lcore %d failed\n", lcore_id);
    }
    rte_mempool_dump(stdout, mp);
    return mp;
}
/*
int tx_mempool_alloc_bulk(struct rte_mempool *mp, struct rte_mbuf *mbuf[], int n)
{
    int ret, i;
    ret = rte_mempool_get_bulk(mp, (void**)mbuf, n);
    if(ret >= 0)
    {
        for(i = 0; i < n; i++)
        {
            tx_mbuf_init_noreset(mbuf[i]);
        }
    }
    return ret;
}

void tx_mempool_free_bulk(struct rte_mbuf *mbuf[], int n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        rte_pktmbuf_free(mbuf[i]);
    }
}*/
