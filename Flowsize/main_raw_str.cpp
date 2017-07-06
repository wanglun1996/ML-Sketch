#include <assert.h>
#include "Sketch.h"

#define DEP 4
#define NOR_WID 131072
#define SAC_WID 262144
//#define NOR_WID 262144
//#define SAC_WID 524288

int sample_rate = 100;

const char* in_path = "./FlowData/example.data";
const char* out_path1 = "./MLData/example.train";
const char* out_path2 = "./MLData/example.test";

std::unordered_map<std::string, int> *record = new std::unordered_map<std::string, int>();
std::unordered_map<std::string, int>::iterator itr;

#define MAXN 10000000

char flows[MAXN][40];

uint32_t ipv4_to_int(const char * s)
{
    uint32_t a, b, c, d;
    sscanf(s, "%u.%u.%u.%u", &a, &b, &c, &d);
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
    while (fscanf(f, "%s", flows[tot]) == 1) {
        tot++;
    }

    fclose(f);
    return tot;
}

int sketch_build(Sketch * sketch, int tot)
{
    record->clear();

    for (int i = 0; i < tot; ++i) {
        sketch->Insert((cuc *)flows[i]);

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
        fprintf(fout, "%d %d\n", itr->second, sketch->Query((cuc*)(itr->first.c_str()), ml));
    }

    fclose(fout);

    return 0;
}

void sketch_sample_and_gen_ml_dataset(Sketch * sketch, const char * filename, int tot)
{
    record->clear();
    for (int i = 0; i < tot; i += sample_rate) {
        itr = record->find(std::string(flows[i]));
        if(itr!=record->end()){
            ++itr->second;
        }
        else{
            record->insert(std::make_pair(std::string(flows[i]), 1));
        }
        sketch->Insert((cuc *)flows[i]);
    }

    freopen(filename, "w", stdout);
    for(itr = record->begin(); itr != record->end(); ++itr){
        sketch->PrintCounter((cuc *)(itr->first.c_str()), (uint)itr->second);
    }
}


int main(int argc, char ** argv) {

	if (argc < 6) {
		fprintf(stderr, "Need more arguments.");
		return 0;
	}

	char * infile = argv[1];
	char * outfile = argv[2];
	int sketch_type = atoi(argv[3]);
	int is_predict = atoi(argv[4]);
    char * data_algo = argv[5];

    double scale_rate;
    // set scale rate
    switch (sketch_type) {
        case 2:
        case 3:
            scale_rate = 0.1 * sample_rate;
            break;
        default:
            scale_rate = (1.05 - 0.1 * (log(sample_rate) / log(2))) * sample_rate;
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