#include <assert.h>
#include "Sketch.h"
#include <cmath>

#define DEP 4
#define NOR_WID 131072
#define SAC_WID 262144

std::unordered_map<std::string, int> *record = new std::unordered_map<std::string, int>();
std::unordered_map<std::string, int>::iterator itr;

#define MAXN 10000000

char flows[MAXN][40];
const char * now_file;

uint32_t ipv4_to_int(const char * s)
{
    uint32_t a, b, c, d;
    int cnt = sscanf(s, "%u.%u.%u.%u", &a, &b, &c, &d);
    if (cnt < 4)
        printf("Invalid IP! %s\n", now_file);
    return (a << 24) + (b << 16) + (c << 8) + d;
}

void ipv4_to_bytes(unsigned char * dest, const char * source)
{
    uint32_t ip1, ip2, port1, port2, type;
    sscanf(source, "%u%u%u%u%u", &ip1, &port1, &ip2, &port2, &type);
    *((uint32_t *)dest) = ip1;
    *((uint16_t *)(dest + 4)) = (uint16_t)port1;
    *((uint32_t *)(dest + 6)) = ip2;
    *((uint16_t *)(dest + 10)) = (uint16_t)port2;
    *((uint8_t *)(dest + 12)) = (uint8_t)type;
}

int read_data(const char * filename)
{
    FILE * f = fopen(filename, "r");

    if (!f) {
        fprintf(stderr, "File not found.\n");
        exit(0);
    }

    int tot = 0;
    int ts;
    int tmp;
    while (fscanf(f, "%d", &ts) == 1) {
        char ip[20];
        uint32_t ip1, port1, ip2, port2, type;
        fscanf(f, "%s %u", ip, &port1);
        ip1 = ipv4_to_int(ip);
        fscanf(f, "%s %u", ip, &port2);
        ip2 = ipv4_to_int(ip);
        fscanf(f, "%u", &type);

        char * dest = flows[tot++];

        *((uint32_t *)dest) = ip1;
        *((uint16_t *)(dest + 4)) = (uint16_t)port1;
        *((uint32_t *)(dest + 6)) = ip2;
        *((uint16_t *)(dest + 10)) = (uint16_t)port2;
        *((uint8_t *)(dest + 12)) = (uint8_t)type;

        fscanf(f, "%d", &tmp);
    }

    printf("%s: %d lines.\n", filename, tot);
    fclose(f);
    return tot;
}

int sketch_build(Sketch * sketch, int tot)
{
    record->clear();

    for (int i = 0; i < tot; ++i) {
        sketch->Insert((cuc *)flows[i]);

        itr = record->find(std::string(flows[i], 13));
        if(itr!=record->end()){
            ++itr->second;
        }
        else{
            record->insert(std::make_pair(std::string(flows[i], 13), 1));
        }
    }

    return 0;
}

int sketch_gen_result(CMSketch * sketch, char * filename)
{
    FILE * fout = fopen(filename, "w");

    for(itr = record->begin(); itr != record->end(); ++itr){
        sketch->PrintCounterFile((cuc *)itr->first.data(), itr->second, fout);
    }

    fclose(fout);

    return 0;
}

int main(int argc, char ** argv) {

	char * infile = argv[1];
    char outfile[] = "out.dat";

    int tot = read_data(infile);
    CMSketch * sketch;

    sketch = new CMSketch(DEP, NOR_WID);
    sketch_build(sketch, tot);

    sketch_gen_result(sketch, outfile);

	return 0;
}
