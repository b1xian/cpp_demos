import numpy as np
import matplotlib.pyplot as plt

def generate_sim_traj_cutin():
    # 生成螺旋部分
    spiral_cnt = 150
    theta = np.linspace(5.0, 3.5 * np.pi, spiral_cnt)  # 角度
    r = theta / 0.3  # 半径
    y_spiral = -r * np.cos(theta) + 60
    x_spiral = r * np.sin(theta)
    y_spiral = y_spiral[-10:-1]
    x_spiral = x_spiral[-10:-1]
    # 生成直线部分
    line_dis = 30
    line_cnt = 15
    y_line = np.linspace(y_spiral[-1], y_spiral[-1] - line_dis, line_cnt)
    x_line = np.ones_like(y_line) * x_spiral[-1]
    # 生成螺旋另一部分
    y_spiral_2 = []
    x_spiral_2 = []
    y_start = y_spiral[0]
    x_start = x_spiral[0]
    for i, (x, y) in enumerate(zip(x_spiral, y_spiral)):
        print(i, x, y)
        y_spiral_2.append(y_start + y_start - y)
        x_spiral_2.append(x_start + x_start - x)
    y_spiral_2.reverse()
    x_spiral_2.reverse()
    print(y_spiral_2)
    y_line_2 = np.linspace(y_spiral_2[0], y_spiral_2[0] + line_dis, line_cnt)
    y_line_2 = y_line_2[::-1]

    print(y_line_2)
    x_line_2 = np.ones_like(y_line) * x_spiral_2[0]

    # 合并螺旋和直线部分
    y_traj = np.concatenate((y_line_2, y_spiral_2, y_spiral, y_line))
    x_traj = np.concatenate((x_line_2, x_spiral_2, x_spiral, x_line))

    return x_traj, y_traj

if __name__ == '__main__':
    x_traj, y_traj = generate_sim_traj_cutin()

    # 设置图形尺寸
    plt.figure(figsize=(3, 30))

    # 绘制轨迹
    plt.plot(x_traj, y_traj, label='Trajectory', color='b', linewidth=2)
    plt.scatter(x_traj, y_traj, color='r', s=10)  # 在轨迹上加上点
    plt.title('Smooth Transition Between Two Lines Using Symmetric Cosine Curves')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.grid(True)
    plt.legend()
    plt.show()
