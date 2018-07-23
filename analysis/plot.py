import matplotlib.pyplot as plt
import numpy as np

methods = ['super cell+row+col+box(8)', 'super cell(7)', 'fast cell+row+col+box(6)', 'cell+row+col+box(5)', 'stack + empty cell view(4)', 'empty cell view(3)', 'fast backtracking(2)', 'ordinary backtracking(1)']


color1 = (85, 26, 139)
color2 = (0, 191, 255)
d = [(y-x)/(len(methods)-1) for x, y in zip(color1, color2)]
colors = ['#%02x%02x%02x' % tuple(int(x+i*d[k]) for x, k in zip(color1, range(3))) for i in range(len(methods))]

results = (
    ('easy', (0.0119, 0.0101, 0.00474, 0.00469, 0.00492, 0.00456, 0.015, 0.044)),
    ('easy_3_ans', (0.023, 0.0219, 0.0163, 0.0155, 0.0129, 0.0096, 0.0199, 0.0579)),
    ('medium', (0.033, 0.0522, 0.060, 0.051, 0.351, 0.297, 12.6, 30.8)),
    ('hardest_no_ans', (0.37, 0.675, 1.01, 2.08, 9.85, 8.61, 114, 281)),
    ('hardest', (2.24, 4.55, 6.41, 14.0, 47.2, 40.5, 203, 502)),
    # ('hardest_85_ans', (20.5, 44.4, 131.6, 109.8, 383.3, 938)),
)

def plot_per_sudoku(results, colors):
  x = np.arange(len(methods))

  for title, times in results:
    ax1 = plt.subplot2grid((2, 9), (0, 3), colspan=6)
    ax1.set_title(title)
    ax1.barh(x, times, align='center', color=colors)
    ax1.set_yticks(x)
    ax1.set_yticklabels(methods)
    
    ax2 = plt.subplot2grid((2, 9), (1, 3), colspan=6)
    ax2.barh(x[:-2], times[:-2], color=colors[:-2])
    ax2.set_yticks(x[:-2])
    ax2.set_yticklabels(methods[:-2])
    ax2.set_xlabel('time / ms')
    
    plt.savefig('result_' + title + '.png')

def plot_all(results, colors):
  x = np.arange(len(results))

  m_results = tuple(r[1] for r in results)
  m_results = tuple(zip(*m_results)) # transpose
  m_results = tuple((m,r) for m, r in zip(methods, m_results))
  print(m_results)

  plt.title('all')
  plt.ylabel('time / ms')

  plt.xticks(x, tuple(r[0] for r in results))

  for idx, m_result in enumerate(m_results):
    title, times = m_result
    plt.plot(x, times, color=colors[idx])
    
  plt.legend(tuple(m for m in methods))
  # plt.show()
  plt.savefig('result_all.png')

def plot_all_3(results, colors):
  x = np.arange(len(results))

  m_results = tuple(r[1] for r in results)
  m_results = tuple(zip(*m_results)) # transpose
  m_results = tuple((m,r) for m, r in zip(methods, m_results))
  print(m_results)

  ax1 = plt.subplot2grid((3, 5), (0, 1), colspan=4)
  ax2 = plt.subplot2grid((3, 5), (1, 1), colspan=4)
  ax3 = plt.subplot2grid((3, 5), (2, 1), colspan=4)
  ax1.set_title('all')
  ax2.set_ylabel('time / ms')
  ax2.set_ylim(0,m_results[-3][1][-1])
  ax3.set_ylim(0,0.05)

  ax1.set_xticks(x)
  ax1.set_xticklabels(['' for _ in range(5)])
  ax2.set_xticks(x)
  ax2.set_xticklabels(['' for _ in range(5)])
  ax3.set_xticks(x)
  ax3.set_xticklabels([r[0] for r in results])

  for idx, m_result in enumerate(m_results):
    title, times = m_result
    ax1.plot(x, times, color=colors[idx])
    ax2.plot(x, times, color=colors[idx])
    ax3.plot(x, times, color=colors[idx])
    
    # ax2.barh(x[:-2], times[:-2], color=colors[:-2])
    # plt.savefig(title+'.png')
  plt.show()


# plot_per_sudoku(results, colors)
plot_all(results, colors)
  
