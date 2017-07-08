#include <stdio.h>
#include <netinet/in.h>
#include <stddef.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include <rte_common.h>
#include <rte_cycles.h>
#include <rte_memory.h>
#include <rte_mempool.h>
#include <rte_timer.h>
#include <rte_eal.h>
#include <rte_debug.h>
#include <rte_launch.h>
#include <rte_lcore.h>
#include <rte_per_lcore.h>
#include <rte_ethdev.h>

#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_udp.h>
#include <rte_tcp.h>

//hj
#include <rte_spinlock.h>
//jh

#include "pm.h"
#include "tload.h"
#include "tx_mp.h"


//#define GEN_VXLAN

#define NB_MBUF 65535
#define MBUF_CACHE_SIZE 512
#define NB_BURST 32

#define NB_TXQ 1   //The setting of 1 can eliminate dumplated transimitting of 5 Tuples if TIMES == 1.
//设置为0，就不接收包了。
//#define NB_RXQ 1   //DO NOT CHANGE THIS NUMBER
#define NB_RXQ 0   //DO NOT CHANGE THIS NUMBER

#define NB_RXD_DFT 128
#define NB_TXD_DFT 512
const int nb_rxd = NB_RXD_DFT;
const int nb_txd = NB_TXD_DFT;

#define NB_MAX_PM 13000001

#define TX_RATE_DFT 10000

#define PRINT_GAP 2


int start_send_flag = 1;
uint64_t start_tsc,end_tsc,diff_tsc;

//已发送包的个数
uint32_t sended = 0;
uint32_t droped = 0;
//把流量文件里的包发送TIMES遍
#define TIMES_DEFAULT 1

uint32_t pkt_times = TIMES_DEFAULT;

/* global data */
struct 
{
    uint32_t total_trace;
    char trace_file[256];
    uint64_t Mbps;
}ginfo =
{
    .Mbps = TX_RATE_DFT,
};

/* generate mbuf */
struct packet_model pms[NB_MAX_PM];

/* pkt length*/
extern uint32_t pkt_length;

static void usage()
{
    printf("Usage: pkt-sender <EAL options> -- -t <trace_file> -s <Mbps> -L <pkt_length> -T <pkt_times>\n");
    rte_exit(-1, "invalid arguments!\n");
}

static void parse_params(int argc, char **argv)
{
    char opt;
    int accept = 0;
    while((opt = getopt(argc, argv, "t:s:L:T:")) != -1)
    {
        switch(opt)
        {
            case 't': rte_memcpy(ginfo.trace_file, optarg, strlen(optarg)+1); accept = 1; break;
            case 's': ginfo.Mbps = atoi(optarg); 
                      if(ginfo.Mbps <= 0)
                      {
                          rte_exit(EINVAL, "tx rate (Mbps) is invalid!\n");
                      }
                      break;
            case 'L': 
                      pkt_length = atoi(optarg);
                      if(pkt_length < 64)
                      {
                          rte_exit(EINVAL, "pkt_length should be no less than 64!\n");
                      }
                      break;
	    case 'T':
		     pkt_times = atoi(optarg);
		     break;
            default: usage();
        }
    }
    if(!accept)
    {
        usage();
    }

}
/**************************************************************/

/* port init */
struct rte_eth_conf port_conf = 
{
    .rxmode = 
    {
        .max_rx_pkt_len = ETHER_MAX_LEN,
    },
};

/* don't care about NUMA */
static int all_port_setup(struct rte_mempool *mp)
{
    int ret;
    int nb_ports;
    int port_id;
    int queue_id;
    nb_ports = rte_eth_dev_count();

    for(port_id = 0; port_id < nb_ports; port_id++)
    {
        ret = rte_eth_dev_configure(port_id, NB_RXQ, NB_TXQ, &port_conf);
        if(ret < 0)
        {
            rte_exit(-1, "port %d configure failure!\n", port_id);
        }
        for(queue_id = 0; queue_id < NB_TXQ; queue_id++)
        {
            ret = rte_eth_tx_queue_setup(port_id, queue_id, nb_txd, rte_eth_dev_socket_id(port_id), NULL);
            if(ret != 0)
            {
                rte_exit(-1, "port %d tx queue setup failure!\n", port_id);
            }
        }
        for(queue_id = 0; queue_id < NB_RXQ; queue_id++)
        {
            ret = rte_eth_rx_queue_setup(port_id, queue_id, nb_rxd, rte_eth_dev_socket_id(port_id), NULL, mp);
            if(ret != 0)
            {
                rte_exit(-1, "port %d rx queue setup failure!\n", port_id);
            }
        }
        ret = rte_eth_dev_start(port_id);
        if(ret < 0)
        {
            rte_exit(-1, "port %d start failure!\n", port_id);
        }
        rte_eth_promiscuous_enable(port_id);
    }
    return nb_ports;
}


/* lcore main */

struct
{
    struct
    {
        uint64_t tx_total_pkts;
        uint64_t tx_last_total_pkts;
    }txq_stats[NB_TXQ];

    struct
    {
        uint64_t rx_total_pkts;
        uint64_t rx_last_total_pkts;
    }rxq_stats[NB_RXQ];

    uint64_t tx_total;
    uint64_t tx_pps;
    uint64_t tx_mbps;
    uint64_t rx_total;
    uint64_t rx_pps;
    uint64_t rx_mbps;
}port_stats[RTE_MAX_ETHPORTS];

struct lcore_args
{
    uint32_t port_id;
    struct
    {
        struct rte_mbuf *m_table[NB_BURST] __rte_cache_aligned;
        struct rte_mempool *mp;
        uint32_t queue_id;
        struct rte_timer tim;
    }tx;
    struct
    {
        uint8_t is_rx_lcore;
    }rx;
    uint64_t speed;
    uint32_t trace_idx;
};

struct lcore_args lc_args[RTE_MAX_LCORE];

static rte_spinlock_t sended_spinlock;


static void send_pkt_rate(__rte_unused struct rte_timer *timer, void *arg) 
{
    struct lcore_args *largs = (struct lcore_args*)arg;
    struct rte_mempool *mp;
    uint32_t port_id;
    uint32_t queue_id;
    uint32_t count = 0;
    int ret;
    static int index = 0;
    static int open = 0;
    mp = largs->tx.mp;
    port_id = largs->port_id;
    queue_id = largs->tx.queue_id;
    
    /*if (unlikely(open == 0)) {
    	printf("start: %u", largs->trace_idx);
    	fflush(stdout);
    }

*/
    for(; count < NB_BURST; count++)
    {
        if(unlikely(largs->trace_idx == ginfo.total_trace))
        {
	    printf("\nONE circle trace_idx = %u, droped = %u, LCOREID = %u\n", largs->trace_idx, droped, rte_lcore_id());
	    largs->trace_idx = 0;
   	    index++;
	    if (index == pkt_times) {
            	break;
	    }
        }
        largs->tx.m_table[count] = generate_mbuf(pms[largs->trace_idx++], mp, pkt_length);
    }

    rte_spinlock_lock(&sended_spinlock);    
    if(start_send_flag)
    {
	start_tsc = rte_rdtsc();
	start_send_flag = 0;
    }
    rte_spinlock_unlock(&sended_spinlock);


    //发送包
    ret = rte_eth_tx_burst(port_id, queue_id, largs->tx.m_table, count);
    //printf("| Start: %u; End: %u |", largs->trace_idx-count, largs->trace_idx-count+ret);
    //adjust trace_idx:
    largs->trace_idx = (largs->trace_idx + ginfo.total_trace - count + ret)%ginfo.total_trace;

  /*  if (unlikely((open == 0)&&(ret == 0))){
	index++;
    }else if (unlikely(open == 0)){
	open = 1;
	printf("index: %d", index);
	fflush(stdout);
    } 
    */
    rte_spinlock_lock(&sended_spinlock);    
    sended += ret;
    droped += count - ret;
    rte_spinlock_unlock(&sended_spinlock); 


    port_stats[port_id].txq_stats[queue_id].tx_total_pkts += ret;
    while(ret < count)
    {
        rte_pktmbuf_free(largs->tx.m_table[ret++]);
    }
	
    
    rte_spinlock_lock(&sended_spinlock);
    if(ginfo.total_trace * pkt_times <= sended)
    {
	end_tsc = rte_rdtsc();
	diff_tsc = end_tsc - start_tsc;

	uint64_t hz = rte_get_timer_hz();
	double time_consume = (double)diff_tsc/(double)hz;
	
	printf("发送完毕！包数量为 %u ,共发送 %u , 共耗时　%lf秒\n\n",ginfo.total_trace, sended, time_consume);
	printf("\nEND trace_idx = %u, droped = %u, lcoreid = %u\n", largs->trace_idx, droped, rte_lcore_id());
	rte_exit(0, "发送完毕！\n");
    }
    rte_spinlock_unlock(&sended_spinlock);
}

static uint64_t calc_period(uint64_t speed)
{
    return (uint64_t) (((NB_BURST * (pkt_length + 20) * 8 * rte_get_tsc_hz()) / (double) speed) );
}

static int sender_lcore_main(void *args)
{
    struct rte_mbuf *rx_table[NB_BURST];
    struct lcore_args *largs;
    uint8_t is_rx;
    int ret;

    largs = (struct lcore_args*)args;
    
    is_rx = largs->rx.is_rx_lcore;
    
    int j;
    if(is_rx == 0)
    {
        printf("send packet from port %u - queue %u!\n", largs->port_id, largs->tx.queue_id);

        port_stats[largs->port_id].txq_stats[largs->tx.queue_id].tx_total_pkts = 0;
        port_stats[largs->port_id].txq_stats[largs->tx.queue_id].tx_last_total_pkts = 0;
        rte_timer_init(&largs->tx.tim);

        uint64_t period = calc_period(largs->speed);
        printf("period %lu\n", period);
        rte_timer_reset(&largs->tx.tim, period, PERIODICAL, rte_lcore_id(), send_pkt_rate, largs);
    }
    else
    {
        for(j = 0; j < NB_RXQ; j++)
        {
            port_stats[largs->port_id].rxq_stats[j].rx_total_pkts = 0;
            port_stats[largs->port_id].rxq_stats[j].rx_last_total_pkts = 0;
        }
    }
	
   long int ii = 0;
    for(;;) {
        if(is_rx) {
            for(j = 0; j < NB_RXQ; j++) {
                ret = rte_eth_rx_burst(largs->port_id, j, rx_table, NB_BURST);
                port_stats[largs->port_id].rxq_stats[j].rx_total_pkts += ret;
                while(ret > 0) {
                    rte_pktmbuf_free(rx_table[--ret]);
                }
            }

        }
        else
        {
            rte_timer_manage(); 
        }
    }
}

static void print_stats(int nb_ports)
{
    int i, j;
    uint64_t tx_total;
    uint64_t tx_last_total;
    uint64_t rx_total;
    uint64_t rx_last_total;
    uint64_t last_cyc, cur_cyc;
    uint64_t frame_len;
#ifdef GEN_VXLAN
    frame_len = pkt_length + 20 + 14 + 20 + 8 + 8;
#else
    frame_len = pkt_length + 20;
#endif
    double time_diff;
    last_cyc = rte_get_tsc_cycles();
    for(;;)
    {
        sleep(PRINT_GAP);
        i = system("clear");
        for(i = 0; i < nb_ports; i++)
        {
            tx_total = tx_last_total = 0;
            rx_total = rx_last_total = 0;
            for(j = 0; j < NB_TXQ; j++)
            {
                tx_total += port_stats[i].txq_stats[j].tx_total_pkts;
                tx_last_total += port_stats[i].txq_stats[j].tx_last_total_pkts;
                port_stats[i].txq_stats[j].tx_last_total_pkts = port_stats[i].txq_stats[j].tx_total_pkts;
            }
            for(j = 0; j < NB_RXQ; j++)
            {
                rx_total += port_stats[i].rxq_stats[j].rx_total_pkts;
                rx_last_total += port_stats[i].rxq_stats[j].rx_last_total_pkts;
                port_stats[i].rxq_stats[j].rx_last_total_pkts = port_stats[i].rxq_stats[j].rx_total_pkts;
            }
            cur_cyc = rte_get_tsc_cycles();
            time_diff = (cur_cyc - last_cyc) / (double)rte_get_tsc_hz();
            port_stats[i].tx_total = tx_total;
            port_stats[i].tx_pps = (uint64_t)((tx_total - tx_last_total) / time_diff);
            port_stats[i].tx_mbps = port_stats[i].tx_pps * (frame_len) * 8 / (1000000);
            port_stats[i].rx_total = rx_total;
            port_stats[i].rx_pps = (uint64_t)((rx_total - rx_last_total) / time_diff);
            port_stats[i].rx_mbps = port_stats[i].rx_pps * (frame_len) * 8 / (1000000);
            
        }
        last_cyc = rte_get_tsc_cycles();
        for(i = 0; i < nb_ports; i++)
        {
            printf("Port %d Statistics:\n", i);
            printf(">>>>>>>>>>>tx rate: %llupps\n", (unsigned long long)port_stats[i].tx_pps);
            printf(">>>>>>>>>>>tx rate: %lluMbps\n", (unsigned long long)port_stats[i].tx_mbps);
            printf(">>>>>>>>>>tx total: %llu\n", (unsigned long long)port_stats[i].tx_total);
            printf("\n");
            printf(">>>>>>>>>>>rx rate: %llupps\n", (unsigned long long)port_stats[i].rx_pps);
            printf(">>>>>>>>>>>rx rate: %lluMbps\n", (unsigned long long)port_stats[i].rx_mbps);
            printf(">>>>>>>>>>rx total: %llu\n", (unsigned long long)port_stats[i].rx_total);
            printf("============================\n");

        }
    }
}


int main(int argc, char **argv)
{
    int ret;
    ret = rte_eal_init(argc, argv);
    if(ret < 0)
    {
        rte_exit(-1, "rte_eal_init failure!\n");
    }
    
    struct rte_mempool *mbuf_pool;
    mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL", NB_MBUF, MBUF_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());
    if(mbuf_pool == NULL)
    {
        rte_exit(-1, "create pktmbuf pool failure!\n");
    }
    int nb_ports;
    nb_ports = all_port_setup(mbuf_pool);
    if(nb_ports <= 0)
    {
        rte_exit(-1, "not detect any DPDK devices!\n");
    }

    int lcore_nb;
    lcore_nb = rte_lcore_count();
    printf("\nlcore_nb = %d", lcore_nb);
    printf("\nnb_ports = %d\n", nb_ports);

    rte_spinlock_init(&sended_spinlock);

// the "1" added for count is specified for master lcore. Each slave lcore works for only one queue (TXQ or RXQ). Each port is assigned with NB_TXQ HW ring(s) for transmitting and with NB_RXQ HW ring(s) for receiving.  
    if(lcore_nb < nb_ports * (NB_TXQ + NB_RXQ) + 1)
    {
        rte_exit(-1, "lcore is less than needed! (should be %d)\n", nb_ports * (NB_TXQ + NB_RXQ) + 1);
    }
    if(lcore_nb > nb_ports * (NB_TXQ + NB_RXQ) + 1)
    {
        rte_exit(-1, "lcore is too much! (should be %d)\n", nb_ports * (NB_TXQ + NB_RXQ) + 1);
    }


    parse_params(argc - ret, argv + ret);

    rte_timer_subsystem_init();

#ifndef GEN_VXLAN
    ret = load_trace(ginfo.trace_file, pms);   
#else
    ret = load_vxlan_trace(ginfo.trace_file, pms);
#endif
    ginfo.total_trace = ret;
    if(ret <= 0)
    {
        rte_exit(-1, "no invalid trace!\n");
    }

//    int nb_rx_lcore = nb_ports;
    int nb_rx_lcore = nb_ports * NB_RXQ;


    int nb_tx_lcore = nb_ports * NB_TXQ;
    int lcore_idx = 0;
    uint32_t lcore_id;
    uint32_t port_id;
    uint32_t queue_id;
    port_id = queue_id = 0;

    RTE_LCORE_FOREACH_SLAVE(lcore_id)
    {
        if(lcore_idx < nb_rx_lcore)
        {
            lc_args[lcore_id].port_id = port_id++;
            lc_args[lcore_id].rx.is_rx_lcore = 1;
            printf("\nRECEIVE lcore: lcore_id = %u", lcore_id);
        }
        else
        {
            if(lcore_idx == nb_rx_lcore)
            {
                port_id = queue_id = 0;
            }
            lc_args[lcore_id].tx.mp = mbuf_pool;
            lc_args[lcore_id].tx.queue_id = queue_id % NB_TXQ;
            lc_args[lcore_id].rx.is_rx_lcore = 0;
            lc_args[lcore_id].port_id = queue_id++ / NB_TXQ;
            lc_args[lcore_id].speed = ginfo.Mbps * 1024 * 1024 / NB_TXQ;
            lc_args[lcore_id].trace_idx = 0;
            printf("\nTRANSMIT lcore: lcore_id = %u", lcore_id);
        }
        rte_eal_remote_launch(sender_lcore_main, (void*)&lc_args[lcore_id], lcore_id);
        lcore_idx++;
        printf("\nlcore_id = %u", lcore_id);
	//break;
    }
    print_stats(nb_ports);
    rte_eal_mp_wait_lcore();
    return 0;
}
