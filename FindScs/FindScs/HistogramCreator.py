import collections
from os import walk
from matplotlib import pyplot as plt


outputDir = open("histogramConf.txt", "r").read()
_, _, filenames = next(walk(outputDir))
if 'pieChart.png' in filenames:
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

