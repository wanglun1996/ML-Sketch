from run import read_list_file


# draw figure #1->#3 re_cm, re_cu, re_csm

f = open('drawing/out/raw_cdf_result')
dl = [x.strip().split(',') for x in f]
f.close()
dl = zip(*dl)
x = dl[0]
dl = sorted(dl[1:], key=lambda x: x[0])

for ago in ['CM', 'CU', 'CSM']:
    f = open('drawing/out/gfs_re_' + str.lower(ago), 'w')
    results = filter(lambda x: x[0].find(ago) != -1, dl)
    for i in range(0, len(x)):
        f.write(x[i])
        for r in results:
            f.write("," + r[i])
        f.write("\n")
    f.close()


class Result(object):
    def __init__(self, line):
        d = line.split()
        self.dataset = '_'.join(d[0].split('_')[0:2])
        self.algo = '_'.join(d[0].split('_')[2:])
        self.flow_num = int(d[1])
        self.avg_rel_err = float(d[2])
        self.avg_abs_err = float(d[3])


class ResultSyn(object):
    def __init__(self, line):
        d = line.split()
        self.dataset = float(d[0].split('_')[0])
        self.algo = '_'.join(d[0].split('_')[1:])
        self.flow_num = int(d[1])
        self.avg_rel_err = float(d[2])
        self.avg_abs_err = float(d[3])

# draw figure #4->#6 ae_cm, ae_cu, ae_csm

f = open('drawing/out/raw_data_set_result')
raw_dl = [Result(x) for x in f]
f.close()

for ago in ['CM', 'CU', 'CSM']:
    results = filter(lambda x: x.algo.find(ago) != -1, raw_dl)
    print len(results)
    f = open('drawing/out/gfs_ae_' + str.lower(ago), 'w')
    f.write("Dataset,%s,%s\_ML,%s\_SAC,%s\_SAC\_ML\n" % (ago, ago, ago, ago))
    ds = set()
    for d in results:
        ds.add(d.dataset)
    cnt = 0
    for s in list(ds)[:16]:
        cnt += 1
        dt = filter(lambda x: x.dataset == s, results)
        f.write("%d" % cnt)
        dt = sorted(dt, key=lambda x: x.algo)
        for d in dt:
            f.write(",%lf" % d.avg_abs_err)
        f.write("\n")

    f.close()


# draw figure #7->#9 ae_cm, ae_cu, ae_csm
f = open('drawing/out/syn_data_set_result')
raw_dl = [ResultSyn(x) for x in f]
# raw_dl = sorted(raw_dl, key=lambda x: x.dataset)
f.close()

for ago in ['CM', 'CU', 'CSM']:
    results = filter(lambda x: x.algo.find(ago) != -1, raw_dl)
    print len(results)
    f = open('drawing/out/gfs_skew_' + str.lower(ago), 'w')
    f.write("Skewness,%s,%s\_ML,%s\_SAC,%s\_SAC\_ML\n" % (ago, ago, ago, ago))
    ds = set()
    for d in results:
        ds.add(d.dataset)
    ds = sorted(list(ds))
    for s in ds:
        dt = filter(lambda x: x.dataset == s, results)
        f.write("%.1lf" % dt[0].dataset)
        dt = sorted(dt, key=lambda x: x.algo)
        for d in dt:
            f.write(",%lf" % d.avg_abs_err)
        f.write("\n")

    f.close()


read_list_file('drawing/list.json')
