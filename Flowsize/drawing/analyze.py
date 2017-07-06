import os, sys
import os.path
import numpy
import multiprocessing
import datetime

cdf_dataset = "20131022_070101"


def analyze_cdf(filename, cdfs):
    f = open(filename)
    dl = map(lambda x: (int(x[0]), int(x[1])), [x.split() for x in f][1:])
    f.close()
    relative_err = []
    cnt = 0
    for d in dl:
        relative_err.append(float(abs(d[1] - d[0])) / d[0])
        if d[1] == d[0]:
            cnt += 1

    obj = dict()
    obj["algo"] = '_'.join((filename.split('/')[-1]).split('_')[2:])
    obj["err"] = numpy.array(relative_err)
    obj["no_err"] = float(cnt) / len(relative_err)

    cdfs.append(obj)

    print "%s analyzed" % name


def write_cdf_results(cdfs):
    tmax = 20.0

    rg = (0, tmax + 1)
    x = numpy.histogram(cdfs[0]["err"], 10000, rg)[1]
    # print len(x)

    results = []
    for data in cdfs:
        hist = numpy.histogram(data["err"], 10000, rg)[0]
        # print hist
        cdf = numpy.cumsum(hist)
        # print cdf
        norm_cdf = cdf / float(len(data["err"]))
        # print norm_cdf
        results.append(norm_cdf)
        # print len(norm_cdf)

    f_out = open('drawing/out/raw_cdf_result', 'w')

    f_out.write("RE")
    for d in cdfs:
        f_out.write(",%s" % d["algo"].replace('_', "\_"))
    f_out.write("\n")

    f_out.write("0")
    for d in cdfs:
        f_out.write(",%lf" % d["no_err"])
    f_out.write("\n")

    for i in range(1, len(x)):
        f_out.write("%lf" % x[i])
        for d in results:
            f_out.write(",%lf" % d[i - 1])
        f_out.write("\n")

    f_out.close()


def analyze_file(filename, lt):
    f = open(filename)
    dl = map(lambda x: (int(x[0]), int(x[1])), [x.split() for x in f][1:])
    f.close()
    relative_err = []
    absolute_err = []
    for d in dl:
        # if d[0] <= 5:
        #     continue
        relative_err.append(float(abs(d[1] - d[0])) / d[0])
        absolute_err.append(abs(d[1] - d[0]))

    if filename.find('zipf') == -1:
        name = filename.split('/')[-1]
    else:
        tmp = filename.split('/')[-1].split('_')
        num = float(tmp[1])
        name = "%.1lf_%s" % (num / 10, '_'.join(tmp[3:]))

    avg_rel_err = reduce(lambda x, y: x + y, relative_err, 0) / float(len(dl))
    avg_abs_err = reduce(lambda x, y: x + y, absolute_err, 0) / float(len(dl))

    lt.append(("%s %d %lf %lf %d\n" % (name, len(dl), avg_rel_err, avg_abs_err, numpy.median(absolute_err))))

    print "%s analyzed" % name


if __name__ == '__main__':
    ip_root = os.path.abspath(sys.argv[1])
    syn_root = os.path.abspath(sys.argv[2])

    ip_out = "drawing/out/raw_data_set_result"
    syn_out = 'drawing/out/syn_data_set_result'

    bits = "111"
    if len(sys.argv) > 3:
        bits = sys.argv[3]

    if len(sys.argv) > 4:
        ip_out = sys.argv[4]

    print ip_root
    print ip_out
    print syn_root
    print syn_out

    start = datetime.datetime.now()

    generate_cdf = (bits[0] == '1')
    if generate_cdf:
        pool = multiprocessing.Pool()
        manager = multiprocessing.Manager()
        cdfs = manager.list()
        for name in os.listdir(ip_root):
            if os.path.isfile(ip_root + '/' + name) and name.find(cdf_dataset) != -1:
                pool.apply_async(analyze_cdf, (ip_root + '/' + name, cdfs))

        pool.close()
        pool.join()
        write_cdf_results(cdfs)

    generate_raw_stat = (bits[1] == '1')
    if generate_raw_stat:
        # generate raw dataset stat info file
        pool = multiprocessing.Pool()
        manager = multiprocessing.Manager()

        results = manager.list()

        for name in os.listdir(ip_root):
            if os.path.isfile(ip_root + '/' + name):
                pool.apply_async(analyze_file, (ip_root + '/' + name, results))
                # analyze_file(root + '/' + name, f_out)

        pool.close()
        pool.join()

        f_out = open(ip_out, 'w')
        f_out.writelines(results)
        f_out.close()

    generate_syn_stat = (bits[2] == '1')
    if generate_syn_stat:
        # generate synthetic dataset stat info file
        pool = multiprocessing.Pool()
        manager = multiprocessing.Manager()

        results = manager.list()

        for name in os.listdir(syn_root):
            if os.path.isfile(syn_root + '/' + name):
                pool.apply_async(analyze_file, (syn_root + '/' + name, results))

        pool.close()
        pool.join()

        f_out = open(syn_out, 'w')
        print len(results)
        f_out.writelines(results)
        f_out.close()

    end = datetime.datetime.now()
    print "Excuted in " + str((end - start).seconds) + "s."
