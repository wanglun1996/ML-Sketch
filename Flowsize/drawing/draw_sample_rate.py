from run import read_list_file


# draw figure #1->#3 re_cm, re_cu, re_csm

class Result(object):
    def __init__(self, line):
        d = line.split()
        self.dataset = '_'.join(d[0].split('_')[0:2])
        self.sample_rate = int(d[0].split('_')[2])
        self.algo = '_'.join(d[0].split('_')[3:])
        self.flow_num = int(d[1])
        self.avg_rel_err = float(d[2])
        self.avg_abs_err = float(d[3])

# draw sample rate figure

f = open('drawing/out/raw_sample_rate_result')
raw_dl = [Result(x) for x in f]
f.close()

f = open('drawing/out/ae_sample_rate', 'w')

datasets = set()
for d in raw_dl:
    datasets.add(d.dataset)

ds = list(datasets)[0]
print "select dataset: " + ds

dl = filter(lambda x: x.dataset == ds, raw_dl)
srs = sorted(list(set([x.sample_rate for x in dl])))

f.write("Sample rate, CM, CSM, CU\n")

for s_rate in srs:
    tmp_ds = filter(lambda x: x.sample_rate == s_rate and x.algo.find('ML') != -1, dl)
    print len(tmp_ds)
    tmp_ds = sorted(tmp_ds, key=lambda x: x.algo)

    f.write("%d" % s_rate)
    for d in tmp_ds:
        f.write(",%lf" % d.avg_abs_err)
    f.write("\n")

f.close()

read_list_file("drawing/list_sample_rate.json")