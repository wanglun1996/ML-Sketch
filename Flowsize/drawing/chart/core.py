#coding=utf-8
import matplotlib.pyplot as plt
import json
import os
import numpy as np
from matplotlib.ticker import FuncFormatter
import re

class DrawingCore:

    def __init__(self, filename, settings):
        print filename,
        self.filename = filename
        self.x_points = []
        self.y_points_arr = []
        self.x_label = None
        self.y_label = settings['y_label']
        self.categories = []
        self.legends = []
        # self.point_types = ['o-', 's-', 'p-', '^-', '*-', 'H-', 'd-', 'h-', '<-', '2-', 'v-']
        self.colors = ['g', 'r', 'b', 'k', 'w', 'y', 'c']

        self.point_types = ['gs-', 'r^-', 'bo-', '^-', '*-', 'H-', 'd-', 'h-', '<-', '2-', 'v-']
        # self.point_types = ['bo-', 'gs-', 'r^-', '^-', '*-', 'H-', 'd-', 'h-', '<-', '2-', 'v-']

        # self.colors = ['g', 'g', 'g', 'g', 'g', 'g', 'g']

        self.output_file = self.filename.split('.')[0] + '.pdf'
        if settings.has_key('output'):
            self.output_file = settings['output']
        self.settings = {
            'x_label': None,
            'chart.type': 'line',
            'style': 1,
            'marker': True,
            'separator': None,
            'legend.loc': 'best',
            'legend.ncol': '1',
            'legend.mode': None,
            'legend.bbox_to_anchor': None,
            'legend.handlelength': None,
        }
        self.settings.update(settings)
        self.x_label = self.settings['x_label']
        self.rcParams = {}
        if settings.has_key('rcParams'):
            self.rcParams = settings['rcParams']

        self.init_plt()
        self.init_data()

        self.draw()

    def init_plt(self):
        f = file(os.path.split(os.path.realpath(__file__))[0] + '/rcParams_' + str(self.settings['style']) + '.json')
        params = json.load(f)
        plt.rcParams.update(params)
        plt.clf()
        f.close()

    def init_data(self):
        f = file(self.filename)
        data = f.readlines()
        f.close()
        # split line
        split_func = lambda x: x.strip().split()
        if self.settings['separator']:
            split_func = lambda x: x.strip().split(self.settings['separator'])

        data = map(split_func, data)
        m = len(data)
        n = len(data[0])
        if not self.x_label:
            self.x_label = data[0][0]
        for i in range(1, n):
            self.y_points_arr.append([])
            self.categories.append(data[0][i])
        for i in range(1, m):
            self.x_points.append(data[i][0])
            for j in range(1, n):
                self.y_points_arr[j - 1].append(data[i][j])

    def draw(self):

        if self.settings['chart.type'] == 'line':
            self.draw_line()
        elif self.settings['chart.type'] == 'bar':
            self.draw_bar()

        ax = plt.gca()
        if self.settings.has_key('xtick.lim'):
            plt.xlim(self.settings['xtick.lim'])
        if self.settings.has_key('ytick.lim'):
            plt.ylim(self.settings['ytick.lim'])
        if self.settings.has_key('xtick.interval'):
            start, end = ax.get_xlim()
            ax.xaxis.set_ticks(np.arange(start, end + self.settings['xtick.interval'], self.settings['xtick.interval']))
        if self.settings.has_key('ytick.interval'):
            start, end = ax.get_ylim()
            ax.yaxis.set_ticks(np.arange(start, end + self.settings['ytick.interval'], self.settings['ytick.interval']))
        if self.settings.has_key('xtick.use_k'):
            ax.xaxis.set_major_formatter(FuncFormatter(lambda x, y: str(int(x / 1000)) + 'k'))

        ax.xaxis.set_ticks_position('bottom')
        ax.yaxis.set_ticks_position('left')

        plt.xlabel(self.x_label)
        plt.ylabel(self.y_label)
        # plt.legend(loc='best', ncol=3, mode='expand')
        
        plt.legend(loc=self.settings['legend.loc'], ncol=int(self.settings['legend.ncol']),
                   mode=self.settings['legend.mode'], bbox_to_anchor=self.settings['legend.bbox_to_anchor'],
                   handlelength=self.settings['legend.handlelength'])
        # plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,
        #            ncol=2, mode="expand", borderaxespad=0.)
        # plt.legend(loc='lower right')

        plt.savefig(self.output_file)
        plt.close()
        print 'done.'

    def draw_line(self):
        x_points = self.x_points
        is_digit = reduce(lambda x, y: x and not re.match(r'[+-]?\d+(.\d+)?$', y) == None, x_points, True)
        if not is_digit:
            x_ticks = x_points
            x_points = range(0, len(x_points))
            plt.xticks(range(0, len(x_points)), x_ticks)
            plt.xlim((-1, len(x_points)))

        for i in range(0, len(self.categories)):
            points_type = self.point_types[i]
            if not self.settings['marker']:
                points_type = points_type[0:0]
            plt.plot(x_points, self.y_points_arr[i], points_type, label=self.categories[i], alpha=0.7)

    def draw_bar(self):
        tot_width = 0.7
        xs = range(0, len(self.x_points))
        width = tot_width / len(self.categories)
        for i in range(0, len(self.categories)):
            plt.bar(map(lambda x: x + i * width, xs), map(lambda x: float(x), self.y_points_arr[i]), width, color=self.colors[i], label=self.categories[i])
        plt.xlim(-0.5, len(self.x_points) + 0.2)
        plt.xticks(map(lambda x: x + tot_width / 2, range(0, len(self.x_points))), self.x_points)
