#include "tload.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <rte_memory.h>
#include <netinet/in.h>
#include "pm.h"

uint32_t pkt_length = 64;

#if 0
static void debug_pm(struct packet_model pm)
{
    printf("0x0000: %04X %04X %04X %04X %04X %04X %04X %04X\n",
            ((uint16_t)pm.eth.d_addr.addr_bytes[0] << 8) | pm.eth.d_addr.addr_bytes[1],
            ((uint16_t)pm.eth.d_addr.addr_bytes[2] << 8) | pm.eth.d_addr.addr_bytes[3],
            ((uint16_t)pm.eth.d_addr.addr_bytes[4] << 8) | pm.eth.d_addr.addr_bytes[5],
            ((uint16_t)pm.eth.s_addr.addr_bytes[0] << 8) | pm.eth.s_addr.addr_bytes[1],
            ((uint16_t)pm.eth.s_addr.addr_bytes[2] << 8) | pm.eth.s_addr.addr_bytes[3],
            ((uint16_t)pm.eth.s_addr.addr_bytes[4] << 8) | pm.eth.s_addr.addr_bytes[5],
            ((uint16_t)ntohs(pm.eth.ether_type)),
            ((uint16_t)pm.ip.version_ihl << 8) | pm.ip.type_of_service);
    printf("0x0010: %04X %04X %04X %04X %04X %04X %04X %04X\n",
            ntohs(pm.ip.total_length),
            ntohs(pm.ip.packet_id),
            ntohs(pm.ip.fragment_offset),
            ((uint16_t)pm.ip.time_to_live << 8) | pm.ip.next_proto_id,
            ntohs(pm.ip.hdr_checksum),
            (uint16_t)(ntohl(pm.ip.src_addr) >> 16),
            (uint16_t)(ntohl(pm.ip.src_addr) & 0xffff),
            (uint16_t)(ntohl(pm.ip.dst_addr) >> 16));
    printf("0x0020: %04X ", (uint16_t)(ntohl(pm.ip.dst_addr) & 0xffff));
    if(pm.ip.next_proto_id == 6)
    {
        printf("%04X %04X %04X %04X %04X %04X %04X\n",
                ntohs(pm.l4.tcp.hdr.src_port),
                ntohs(pm.l4.tcp.hdr.dst_port),
                ntohl(pm.l4.tcp.hdr.sent_seq) >> 16,
                ntohl(pm.l4.tcp.hdr.sent_seq) & 0xffff,
                ntohl(pm.l4.tcp.hdr.recv_ack) >> 16,
                ntohl(pm.l4.tcp.hdr.recv_ack) & 0xffff,
                ((uint16_t)pm.l4.tcp.hdr.data_off << 8) | pm.l4.tcp.hdr.tcp_flags);
        
        printf("0x0030: %04X %04X %04X\n",
                ntohs(pm.l4.tcp.hdr.rx_win),
                ntohs(pm.l4.tcp.hdr.cksum),
                ntohs(pm.l4.tcp.hdr.tcp_urp));
    }
    else
    {
        printf("%04X %04X %04X %04X\n",
                ntohs(pm.l4.udp.hdr.src_port),
                ntohs(pm.l4.udp.hdr.dst_port),
                ntohs(pm.l4.udp.hdr.dgram_len),
                ntohs(pm.l4.udp.hdr.dgram_cksum));
    }
}
#endif

int load_trace_line(FILE *fp, struct packet_model *pm)
{
    int i;
    char buff[256];
    char *tok[7], *s, *sp;
    if(fgets(buff, 256, fp) == NULL)
    {
        return END_LINE;
    }
	
    for(i = 0, s = buff; i < NB_FIELD; i++, s = NULL)
    {
        tok[i] = strtok_r(s, " \t\n", &sp);
    }

    uint8_t proto;
    proto = (uint8_t)strtoul(tok[4], NULL, 0);
    if(proto != 6 && proto != 17)
    {
        return INVALID_LINE;
    }
    
    if(proto == 6)
    {
        //ether header
        memset(&(pm->tcp.eth.d_addr), 0, sizeof(pm->tcp.eth.d_addr));
        memset(&(pm->tcp.eth.s_addr), 0, sizeof(pm->tcp.eth.s_addr));
        pm->tcp.eth.d_addr.addr_bytes[5] = (uint8_t)0x02;
        pm->tcp.eth.s_addr.addr_bytes[5] = (uint8_t)0x01;
        pm->tcp.eth.ether_type = htons((uint16_t)0x0800);

        //ipv4 header
        pm->tcp.ip.next_proto_id = proto;
        pm->tcp.ip.version_ihl = (uint8_t)0x45;
        pm->tcp.ip.type_of_service = (uint8_t)0;
        pm->tcp.ip.total_length = htons((uint16_t)(pkt_length - 18));
        pm->tcp.ip.packet_id = 0;
        pm->tcp.ip.fragment_offset = 0x0040;//DF
        pm->tcp.ip.time_to_live = 0xff;
        pm->tcp.ip.hdr_checksum = 0;
        pm->tcp.ip.src_addr = htonl(strtoul(tok[0], NULL, 0));
        pm->tcp.ip.dst_addr = htonl(strtoul(tok[1], NULL, 0));
        pm->tcp.ip.hdr_checksum = rte_ipv4_cksum(&(pm->tcp.ip));

        //l4 header
        pm->tcp.tcp.src_port = htons((uint16_t)strtoul(tok[2], NULL, 0));
        pm->tcp.tcp.dst_port = htons((uint16_t)strtoul(tok[3], NULL, 0));
        pm->tcp.tcp.sent_seq = htonl(1);
        pm->tcp.tcp.recv_ack = htonl(2);
        pm->tcp.tcp.data_off = (uint8_t)(sizeof(struct tcp_hdr)>>2)<<4;
        pm->tcp.tcp.tcp_flags = (uint8_t)0x10;
        pm->tcp.tcp.rx_win = htons(0xffff);
        pm->tcp.tcp.cksum = 0;
        pm->tcp.tcp.tcp_urp = 0;
        pm->tcp.tcp.cksum = rte_ipv4_udptcp_cksum(&(pm->tcp.ip), (void*)&(pm->tcp.tcp));
        pm->is_udp = 0;
    }
    else
    {
        //ether header
        memset(&(pm->udp.eth.d_addr), 0, sizeof(pm->udp.eth.d_addr));
        memset(&(pm->udp.eth.s_addr), 0, sizeof(pm->udp.eth.s_addr));
        pm->udp.eth.d_addr.addr_bytes[5] = (uint8_t)0x02;
        pm->udp.eth.s_addr.addr_bytes[5] = (uint8_t)0x01;
        pm->udp.eth.ether_type = htons((uint16_t)0x0800);

        //ipv4 header
        pm->udp.ip.next_proto_id = proto;
        pm->udp.ip.version_ihl = (uint8_t)0x45;
        pm->udp.ip.type_of_service = (uint8_t)0;
        pm->udp.ip.total_length = htons((uint16_t)(pkt_length - 18));
        pm->udp.ip.packet_id = 0;
        pm->udp.ip.fragment_offset = 0x0040;//DF
        pm->udp.ip.time_to_live = 0xff;
        pm->udp.ip.hdr_checksum = 0;
        pm->udp.ip.src_addr = htonl(strtoul(tok[0], NULL, 0));
        pm->udp.ip.dst_addr = htonl(strtoul(tok[1], NULL, 0));
        pm->udp.ip.hdr_checksum = rte_ipv4_cksum(&(pm->udp.ip));


        pm->udp.udp.src_port = htons((uint16_t)strtoul(tok[2], NULL, 0));
        pm->udp.udp.dst_port = htons((uint16_t)strtoul(tok[3], NULL, 0));
        pm->udp.udp.dgram_len = htons((uint16_t)(pkt_length - 18 - sizeof(struct ipv4_hdr)));
        pm->udp.udp.dgram_cksum = 0;
        pm->udp.udp.dgram_cksum = rte_ipv4_udptcp_cksum(&(pm->udp.ip), (void*)&(pm->udp.udp));
        pm->is_udp = 1;
    }
    return VALID_LINE;
}

int load_trace(const char *file, struct packet_model pms[])
{

    FILE *fp = fopen(file, "rb");
    int ret = 0;
    int count = 0;
    if(fp == NULL)
    {
        rte_exit(-1, "open trace file failure!\n");
    }
    while((ret = load_trace_line(fp, &pms[count])) != END_LINE)
    {
        if(ret == VALID_LINE)
        {
            count++;
        }
    }
    printf("total trace %d\n", count);
    return count;
}


int load_vxlan_trace_line(FILE *fp, struct packet_model *pm)
{
    uint32_t vni;
    if(fscanf(fp, "%u%*c", &vni) == EOF)
    {
        return END_LINE;    
    }
    memset(pm, 0, sizeof(*pm));
    pm->vxlan.vx.vx_flags = htonl(0x08000000ULL);
    pm->vxlan.vx.vx_vni = htonl(vni << 8);
    //add udp hdr
    struct udp_hdr *vx_udp_hdr = (struct udp_hdr*)(&(pm->vxlan.udp));
    vx_udp_hdr->dst_port = htons(4789);
    vx_udp_hdr->src_port = htons(9999);
    vx_udp_hdr->dgram_len = htons(sizeof(struct vxlan_hdr) + sizeof(struct udp_hdr) + pkt_length - 4);
    //add ip hdr
    struct ipv4_hdr *vx_ip_hdr = (struct ipv4_hdr*)(&(pm->vxlan.ip));
    vx_ip_hdr->version_ihl = 0x45;
    vx_ip_hdr->total_length = htons(ntohs(vx_udp_hdr->dgram_len) + sizeof(struct ipv4_hdr));
    vx_ip_hdr->fragment_offset = htons(0x4000);
    vx_ip_hdr->next_proto_id = 17;
    vx_ip_hdr->hdr_checksum = rte_ipv4_cksum(vx_ip_hdr);

    vx_udp_hdr->dgram_cksum = rte_ipv4_udptcp_cksum(vx_ip_hdr, (void*)vx_udp_hdr);
    //add ether hdr
    //
    struct vlan_hdr *vl_hdr;
    vl_hdr = (struct vlan_hdr*)(&(pm->vxlan.vlan));
    vl_hdr->vlan_tci = 0;
    vl_hdr->eth_proto = htons(0x0800);

    struct ether_hdr *vx_eth_hdr = (struct ether_hdr*)(&(pm->vxlan.eth));
    vx_eth_hdr->ether_type = htons(0x8100);

    pm->is_vxlan = 1;
    return load_trace_line(fp, pm);
}

int load_vxlan_trace(const char *file, struct packet_model pms[])
{
    FILE *fp = fopen(file, "rb");
    int ret = 0;
    int count = 0;
    if(fp == NULL)
    {
        rte_exit(-1, "open trace file failure!\n");
    }
    while((ret = load_vxlan_trace_line(fp, &pms[count])) != END_LINE)
    {
        if(ret == VALID_LINE)
        {
            count++;
        }
    }
    printf("total trace %d\n", count);
    return count;

}

