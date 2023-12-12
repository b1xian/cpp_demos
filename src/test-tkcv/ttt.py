#演示数据集
import pandas as pd
import numpy as np


self_pt1 = [-1.5, -2]
self_pt2 = [1.5, -2]
self_pt3 = [1.5, 6]
self_pt4 = [-1.5, 6]

obj_point1 =  [4.58078, 0.909473]
obj_point2 =  [6.79338, 1.85435]
obj_point3 =  [3.85922, 8.72521]
obj_point4 =  [1.64662, 7.78032]

self_pts = np.array([self_pt1, self_pt2, self_pt3, self_pt4])
obj_pts = np.array([obj_point1, obj_point2, obj_point3, obj_point4])
self_pts = self_pts.transpose()
obj_pts = obj_pts.transpose()

#绘图
import matplotlib.pyplot as plt
#设置图形大小和像素
plt.figure(figsize=(8,9),dpi=130)
plt.scatter(x=self_pts[0],y=self_pts[1],c="black",s=5)#,c="black"表示点为黑色，s=35表示点的大小为35
plt.scatter(x=obj_pts[0],y=obj_pts[1],c="blue",s=5)#,c="black"表示点为黑色，s=35表示点的大小为35


#设置x轴的范围
plt.xlim(xmin = -50,xmax=50)
#设置y轴的范围
plt.ylim(ymin= -50,ymax=50)
#添加水平直线
plt.axhline(y=0,ls="-",color="black",linewidth=2)#y=0表示水平线过y=0，ls设置线条类型，linewidth设置线条粗细
#添加垂直直线
plt.axvline(x=0,ls="-",color="black",linewidth=2)#x=0表示垂线过x=0，其余参数含义同上

plt.axis('on')#不显示散点图原始坐标轴
plt.show()
