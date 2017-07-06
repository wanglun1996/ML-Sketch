from run import read_list_file


# draw figure #1->#3 re_cm, re_cu, re_csm

class Result(object):
    def __init__(self, line):
        d = line.split()
        self.dataset = d[0].split('_')[0]
        self.algo = '_'.join(d[0].split('_')[1:])
        self.flow_num = int(d[1])
        self.avg_rel_err = float(d[2])
        self.avg_abs_err = float(d[3])

# draw sample rate figure

f = open('drawing/out/raw_seq_result')
raw_dl = [Result(x) for x in f]
f.close()

f = open('drawing/out/gfs_ae_seq', 'w')

ds = set()
for d in raw_dl:
    ds.add(d.dataset)

ds = sorted(list(ds))

# raw_dl = filter(lambda x: x.algo.find('raw') == -1, raw_dl)
# raw_dl = filter(lambda x: x.algo.find('ML') != -1, raw_dl)
raw_dl = filter(lambda x: x.algo.find('CM') != -1, raw_dl)

# write header
f.write("Dataset ID, Training once, Training always\n")#", CSM, CSMML, CU, CUML\n")
# f.write("Dataset, CM, CMML, CSM, CSMML, CU, CUML\n")

cnt = 0

for d in ds:
    dt = filter(lambda x: x.dataset == d, raw_dl)
    cnt += 1
    if cnt == 24:
        break
    f.write("%d" % cnt)
    dt = sorted(dt, key=lambda x: x.algo)
    a = dt[1].avg_abs_err / dt[0].avg_abs_err
    b = dt[2].avg_abs_err / dt[0].avg_abs_err
    f.write(",%lf,%lf" % (a, b))
    f.write("\n")

f.close()

read_list_file('drawing/list_seq_predict.json')
