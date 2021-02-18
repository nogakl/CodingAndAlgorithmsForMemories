import collections
from os import walk
from matplotlib import pyplot as plt
import numpy as np


lines = open("histogramConf.txt", "r").readlines()
outputDir = lines[0].rstrip('\r\n')
size = lines[1].rstrip('\r\n')
missing_masks = lines[2].rstrip('\r\n')
_, _, filenames = next(walk(outputDir))

outfileCounts = open(outputDir + "//" + "outfileCounts.txt", "r").readlines()
outfileDiffs = open(outputDir + "//" + "outfileDiffs.txt", "r").readlines()

outfileCountsLst = [i.rstrip('\r\n') for i in outfileCounts]
outfileDiffsLst = [i.rstrip('\r\n') for i in outfileDiffs]

outfileDiffsLst = ['ERR' if x== str(-1) else x for x in outfileDiffsLst]

labelsX = outfileDiffsLst
sizes = outfileCountsLst
# Plot
'''plt.pie(sizes, labels=labels, autopct='%1.1f%%', shadow=True, startangle=140)

plt.axis('equal')
#plt.show()
plt.tight_layout()
plt.savefig(outputDir + '\\pieChart.png')'''


x = np.array(labelsX)
y = np.array([int(i) for i in sizes])
porcent = 100.*y/y.sum()


patches, texts = plt.pie(y,  startangle=90, radius=1.2)
labels = ['{0} - {1:1.6f} %'.format(i,j) for i,j in zip(x, porcent)]

sort_legend = False
if sort_legend:
    patches, labels, dummy =  zip(*sorted(zip(patches, labels, y),
                                          key=lambda x: x[2],
                                          reverse=True))

plt.legend(patches, labels, fontsize=8)
title = "Size: " + str(size) + "\n" + "Missing Masks: " + str(missing_masks)
plt.title(str(title))
plt.savefig(outputDir + '\\piechart.png', bbox_inches='tight')


'''if 'pieChart.png' in filenames:
    filenames.remove('pieChart.png')
if 'histogram.png' in filenames:
    filenames.remove('histogram.png')


errorsDict = {}
for filename in filenames:
    tmp = filename.split('_')
    filter_number = tmp[3].split('.')[0]
    errors_count = tmp[0]
    errorsDict.update({int(filter_number): int(errors_count)})

val_counts = []
s = set(errorsDict.values())
for val in s:
    val_counts.append(len([k for k,v in errorsDict.items() if v == val]))

labels = list(s)
sizes = val_counts
# Plot
plt.pie(sizes, labels=labels, autopct='%1.1f%%', shadow=True, startangle=140)

plt.axis('equal')
plt.savefig(outputDir + '\\pieChart.png')

od = collections.OrderedDict(sorted(errorsDict.items()))
feature_list = od.keys()
feature_count = od.values()
plt.xlabel('filters id')
plt.ylabel('errors count')
plt.bar(feature_list, feature_count)

plt.savefig(outputDir + '\\histogram.png')
'''
