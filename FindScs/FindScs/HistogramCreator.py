import collections
from os import walk
from matplotlib import pyplot as plt


outputDir = open("histogramConf.txt", "r").read()
_, _, filenames = next(walk(outputDir))
if 'histogram.png' in filenames:
    filenames.remove('histogram.png')
errorsDict = {}
for filename in filenames:
    tmp = filename.split('_')
    filter_number = tmp[3].split('.')[0]
    errors_count = tmp[0]
    errorsDict.update({int(filter_number): int(errors_count)})


od = collections.OrderedDict(sorted(errorsDict.items()))
feature_list = od.keys()
feature_count = od.values()
plt.xlabel('filters id')
plt.ylabel('errors count')
plt.bar(feature_list, feature_count)

plt.savefig(outputDir + '\\histogram.png')

