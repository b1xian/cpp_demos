import rospy
import rosbag
import cv2
import os
import numpy as np
from sensor_msgs.msg import CompressedImage
from cv_bridge import CvBridge

bag = rosbag.Bag('/home/sheldon/work_space/data/vision/mycases/cone_margin/VTI-9667/event_20230922100817/TAD_side_vision_2023-09-22-10-08-34_15.bag')
bridge = CvBridge()

topic_node_dict = {
    "/camera9_10/compressed" : 0,
    "/camera3_2/compressed" : 1,
    "/camera9_7/compressed" : 2,
    "/camera3_5/compressed" : 3,
}

topics = ['/camera3_2/compressed', '/camera3_5/compressed', '/camera9_10/compressed', '/camera9_7/compressed']
node_arr = []
for topic, msg, t in bag.read_messages(topics=topics):
    node_arr.append(topic_node_dict[topic])
bag.close()

arr_len = len(node_arr)
need_node = [0, 1, 2, 3]
for i in range(0, arr_len, 4):
    group = node_arr[i:i+4]
    print(group)
    # for node in need_node:
    #     if group.count(node) == 0:
    #         print(group)