#include <assert.h>
#include "Sketch.h"
#include <cmath>

#define DEP 4
#define NOR_WID 131072
#define SAC_WID 262144

int sample_rate = 100;

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

void bytes_to_ipv4(char * dest, const unsigned char * src)
{

    uint32_t ip1, ip2, port1, port2, type;
    ip1 = *((uint32_t *)src);
    port1 = *((uint16_t *)(src + 4));
    ip2 = *((uint32_t *)(src + 6));
    port2 = *((uint16_t *)(src + 10));
    type = *((uint8_t *)(src + 12));
    sprintf(dest, "%u %u %u %u %u", ip1, port1, ip2, port2, type);
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
        sprintf(flows[tot], "%u %u %u %u %u", ip1, port1, ip2, port2, type);
        fscanf(f, "%d", &tmp);
        tot++;
    }

    printf("%s: %d lines.\n", filename, tot);
    fclose(f);
    return tot;
}

int sketch_build(Sketch * sketch, int tot)
{
    record->clear();

    for (int i = 0; i < tot; ++i) {
        unsigned char tmp[13];
        char tmp2[50];
        ipv4_to_bytes(tmp, flows[i]);
//        bytes_to_ipv4(tmp2, tmp);
//        assert(strcmp(tmp2, flows[i]) == 0);
        sketch->Insert(tmp);

        itr = record->find(std::string(flows[i]));
        if(itr!=record->end()){
            ++itr->second;
        }
        else{
            record->insert(std::make_pair(std::string(flows[i]), 1));
        }
    }

    return 0;
}

int sketch_gen_result(Sketch * sketch, char * filename, int tot, bool ml=0)
{
    FILE * fout = fopen(filename, "w");

    fprintf(fout, "%d %lu\n", tot, record->size());
    for(itr = record->begin(); itr != record->end(); ++itr){
        unsigned char tmp[13];
        ipv4_to_bytes(tmp, itr->first.c_str());
        fprintf(fout, "%d %d\n", itr->second, sketch->Query(tmp, ml));
    }

    fclose(fout);

    return 0;
}

void sketch_sample_and_gen_ml_dataset(Sketch * sketch, const char * filename, int tot)
{
    record->clear();
    unsigned char tmp[14];
    for (int i = 0; i < tot; i += sample_rate) {
        itr = record->find(std::string(flows[i]));
        if(itr!=record->end()){
            ++itr->second;
        }
        else{
            record->insert(std::make_pair(std::string(flows[i]), 1));
        }
        ipv4_to_bytes(tmp, flows[i]);
        sketch->Insert(tmp);
    }

    freopen(filename, "w", stdout);
    for(itr = record->begin(); itr != record->end(); ++itr){
        ipv4_to_bytes(tmp, (itr->first).c_str());
        sketch->PrintCounter(tmp, (uint)itr->second);
    }
}


int main(int argc, char ** argv) {

	if (argc < 6) {
		fprintf(stderr, "Need more arguments.");
		return 0;
	}

	char * infile = argv[1]; now_file = infile;
	char * outfile = argv[2];
	int sketch_type = atoi(argv[3]);
	int is_predict = atoi(argv[4]);
    char * data_algo = argv[5];

    double scale_rate;

    // set sample rate
    if (argc >= 7) {
        sample_rate = atoi(argv[6]);

        if (sample_rate <= 0) {
            fprintf(stderr, "invalid sample rate");
            return 0;
        }
    }

    // set scale rate
    switch (sketch_type) {
        case 2:
        case 3:
            scale_rate = 0.1 * sample_rate;
            break;
        default:
            scale_rate = (1.05 - 0.1 * (log(sample_rate) / log(2))) * sample_rate;
    }

    if (argc >= 8) {
        double tmp = atof(argv[7]);
        if (tmp <= 0) {
            fprintf(stderr, "invalid scale rate");
            return 0;
        }
        scale_rate = tmp;
    }

    int tot = read_data(infile);
    Sketch * sketch;
    switch (sketch_type) {
        case 0:
            sketch = new CMSketch(DEP, NOR_WID);
            break;
        case 1:
            sketch = new CMSACSketch(DEP, SAC_WID);
            break;
        case 2:
            sketch = new CUSketch(DEP, NOR_WID);
            break;
        case 3:
            sketch = new CUSACSketch(DEP, SAC_WID);
            break;
        case 4:
            sketch = new PSketch(DEP, NOR_WID);
            break;
        case 5:
            sketch = new PSACSketch(DEP, SAC_WID);
            break;
        default:
            abort();
    }
    sketch_build(sketch, tot);

	if (!is_predict) {
		// normal mode
		// generate normal algorithm sketch result
		// and do sample process
        sketch_gen_result(sketch, outfile, tot);
        delete sketch;

        switch (sketch_type) {
            case 0:
                sketch = new CMSketch(DEP, uint(NOR_WID / scale_rate));
                break;
            case 1:
                sketch = new CMSACSketch(DEP, uint(SAC_WID / scale_rate));
                break;
            case 2:
                sketch = new CUSketch(DEP, uint(NOR_WID / scale_rate));
                break;
            case 3:
                sketch = new CUSACSketch(DEP, uint(SAC_WID / scale_rate));
                break;
            case 4:
                sketch = new PSketch(DEP, uint(NOR_WID / scale_rate));
                break;
            case 5:
                sketch = new PSACSketch(DEP, uint(SAC_WID / scale_rate));
                break;
            default:
                abort();
        }

        char train_data[128];
        sprintf(train_data, "./MLData/%s_sketch.train", data_algo);
        sketch_sample_and_gen_ml_dataset(sketch, train_data, tot);
	} else {
        // predict mode
        char para_data[128];
        sprintf(para_data, "./Para/%s_sketch.para", data_algo);
        sketch->LoadPara((const unsigned char *)para_data);
        sketch_gen_result(sketch, outfile, tot, true);
    }

//	GenMLData(cm);
	return 0;
}