from run import read_list_file


# draw figure #1->#3 re_cm, re_cu, re_csm

class Result(object):
    def __init__(self, line):
        d = line.split()
        self.dataset = '_'.join(d[0].split('_')[0:2])
        self.sample_rate = int(d[0].split('_')[2])
        self.scale_rate = int(d[0].split('_')[3])
        self.algo = '_'.join(d[0].split('_')[4:])
        self.flow_num = int(d[1])
        self.avg_rel_err = float(d[2])
        self.avg_abs_err = float(d[3])

# draw sample rate figure

f = open('drawing/out/raw_scale_rate_result')
raw_dl = [Result(x) for x in f]
f.close()

f = open('drawing/out/ae_scale_rate', 'w')

ds = set()
for d in raw_dl:
    ds.add(d.dataset)

# write dataset
f.write("Scale rate")
for i in range(0, len(ds)):
    f.write(",Dataset %d" % i)
f.write("\n")

ds = list(ds)

raw_dl = sorted(raw_dl, key=lambda x: x.scale_rate)

srs = sorted(list(set([x.scale_rate for x in raw_dl])))


dt_noml = filter(lambda x: x.algo.find('ML') == -1 and x.scale_rate == srs[0], raw_dl)
dt_noml = sorted(dt_noml, key=lambda x: x.dataset + x.algo)
f.write("nonML")
for d in dt_noml:
    f.write(",%lf" % d.avg_abs_err)
f.write("\n")


for s in srs:
    dt = filter(lambda x: x.scale_rate == s and x.algo.find('ML') != -1, raw_dl)
    f.write("%d" % s)
    dt = sorted(dt, key=lambda x: x.dataset + x.algo)
    for d in dt:
        print d.dataset + d.algo + ", %d" % s
        f.write(",%lf" % d.avg_abs_err)
    f.write("\n")

f.close()


read_list_file('drawing/list_scale_rate.json')
