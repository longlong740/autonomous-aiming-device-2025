import time, os, gc
import image
import struct
import math
from media.sensor import Sensor, CAM_CHN_ID_0
from media.display import *
from media.media import MediaManager
from machine import TOUCH

#分辨率
WIDTH, HEIGHT = 800, 480

# 阈值持久化存储函数
def read_threshold():
    """读取阈值"""
    try:
        f = open('/data/threshold.txt', 'r')  # 以只读方式打开文件
        threshold = f.read().strip()  # 读取内容
        f.close()
        # 解析阈值数据
        threshold_data = eval(threshold)
        print("成功从文件加载阈值配置")
        return threshold_data
    except Exception as e:
        print(f"读取阈值失败: {e}，使用默认值")
        # 返回默认阈值
        return [
            [82, 100, -28, 28, -37, 34],
            [0, 100, 0, 100, 0, 100]
        ]

def write_threshold(value):
    """写入阈值"""
    try:
        f = open('/data/threshold.txt', 'w')  # 以写入方式打开文件
        f.write(str(value))  # 将阈值数据转换为字符串后写入
        f.close()
        print("阈值已保存到文件")
        return True
    except Exception as e:
        print(f"保存阈值失败: {e}")
        return False

def draw_touch_btn(img, tp,
                   x, y, w, h,
                   text, text_color=(0, 0, 0),
                   bg_color=(0, 255, 255), thickness=5,
                   font_size=25,
                   text_dx=0, text_dy=0):

    img.draw_rectangle(int(x), int(y), int(w), int(h),
                       color=bg_color, thickness=int(thickness))
    img.draw_string_advanced(
        int(x + (w - len(text) * font_size * 0.6) // 2 + text_dx),
        int(y + (h - font_size) // 2 + text_dy),
        int(font_size),
        text,
        color=text_color
    )

    points = tp.read(1)
    touch = points[0] if points else None
    if touch and x <= touch.x <= x + w and y <= touch.y <= y + h:
        return True
        #返回 True 表示按钮被按下
    return False

# ==================== 阈值调节功能 ====================
def run_threshold_ui(sensor, tp, thresholds,
                     WIDTH=800, HEIGHT=480):

    original_thresholds = [row[:] for row in thresholds]
    current_group = 0
    if not thresholds or len(thresholds) < 2:
        thresholds[:] = [
            [0, 100, -128, 127, -128, 127],
            [0, 100, -128, 127, -128, 127]
        ]

    value_labels = ["Lmin", "Lmax", "Amin", "Amax", "Bmin", "Bmax"]
    threshold_ranges = [
        (0, 100),
        (0, 100),
        (-128, 127),
        (-128, 127),
        (-128, 127),
        (-128, 127),
    ]

    buttons = []
    BTN_W, BTN_H = 120, 70  # 适配800x480，按钮稍大一些

    button_definitions = {
        "Lmin+": (0, 1), "Lmin-": (0, -1),
        "Lmax+": (1, 1), "Lmax-": (1, -1),
        "Amin+": (2, 1), "Amin-": (2, -1),
        "Amax+": (3, 1), "Amax-": (3, -1),
        "Bmin+": (4, 1), "Bmin-": (4, -1),
        "Bmax+": (5, 1), "Bmax-": (5, -1),
    }

    left_texts = ["Lmin+", "Lmin-", "Lmax+", "Lmax-", "Amin+", "Amin-"]
    right_texts = ["Amax+", "Amax-", "Bmin+", "Bmin-", "Bmax+", "Bmax-"]

    # 左侧按钮布局优化
    for i, text in enumerate(left_texts):
        buttons.append({
            'rect': (10, i * (BTN_H + 5) + 10, BTN_W, BTN_H),  # 增加间距
            'text': text,
            'action': button_definitions[text],
            'color': (70, 130, 180),      # 更现代的蓝色
            'text_color': (255, 255, 255), # 白色文字
            'pressed_color': (0, 255, 100)
        })

    # 右侧按钮布局优化
    for i, text in enumerate(right_texts):
        buttons.append({
            'rect': (WIDTH - BTN_W - 10, i * (BTN_H + 5) + 10, BTN_W, BTN_H),
            'text': text,
            'action': button_definitions[text],
            'color': (70, 130, 180),
            'text_color': (255, 255, 255),
            'pressed_color': (0, 255, 100)
        })

    # 中央控制按钮区域 - 利用800px宽度
    center_x = WIDTH // 2 - 60  # 居中位置

    switch_btn = {
        'rect': (center_x, 10, 120, 60),
        'text': "切换组",
        'action': "switch",
        'color': (255, 140, 0),        # 橙色
        'text_color': (255, 255, 255),
        'pressed_color': (255, 200, 0)
    }
    buttons.append(switch_btn)

    toggle_display_btn = {
        'rect': (center_x, 80, 120, 60),
        'text': "切换显示",
        'action': "toggle_display",
        'color': (34, 139, 34),        # 绿色
        'text_color': (255, 255, 255),
        'pressed_color': (50, 205, 50)
    }
    buttons.append(toggle_display_btn)

    exit_btn = {
        'rect': (center_x, 150, 120, 60),
        'text': "退出",
        'action': "exit",
        'color': (220, 20, 60),        # 红色
        'text_color': (255, 255, 255),
        'pressed_color': (255, 69, 0)
    }
    buttons.append(exit_btn)

    def is_point_in_button(x, y, btn_x, btn_y, btn_w, btn_h):
        return btn_x <= x <= btn_x + btn_w and btn_y <= y <= btn_y + btn_h

    def draw_botton(img, x, y, w, h, color, text="", text_color=(255,0,0), font_size=20, center_text=False):
        img.draw_rectangle(x, y, w, h, color=color, thickness=2)
        if text:
            if center_text:
                text_width = int(len(text) * font_size * 0.6)
                tx = x - 15 + (w - text_width) // 2
            else:
                tx = x + 25 + (w - len(text) * font_size) // 2
            ty = y + (h - font_size) // 2
            img.draw_string_advanced(tx, ty, font_size, text, color=text_color)

    show_binary = False
    last_touch = False
    save_hint_counter = 0

    while True:
        img = sensor.snapshot(chn=CAM_CHN_ID_0)

        if show_binary:
            threshold_tuple = tuple(thresholds[current_group])
            img.binary([threshold_tuple])

        points = tp.read(1)
        touch_point = points[0] if points else None

        if touch_point and not last_touch:
            for btn in buttons:
                action = btn.get('action')
                x, y, w, h = btn['rect']
                if is_point_in_button(touch_point.x, touch_point.y, x, y, w, h):
                    if action == "switch":
                        current_group = (current_group + 1) % len(thresholds)
                        print(f"切换到第{current_group+1}组")
                        time.sleep_ms(100)
                    elif action == "toggle_display":
                        show_binary = not show_binary
                        print(f"切换显示模式，当前为{'阈值处理' if show_binary else '原始画面'}")
                        time.sleep_ms(100)
                    elif action == "exit":
                        return thresholds
                    break

        if touch_point:
            for btn in buttons:
                if btn.get('action') not in ["switch", "toggle_display", "exit"]:
                    x, y, w, h = btn['rect']
                    color = btn['color']
                    if is_point_in_button(touch_point.x, touch_point.y, x, y, w, h):
                        color = btn['pressed_color']
                        index, change = btn['action']
                        min_val, max_val = threshold_ranges[index]
                        new_val = thresholds[current_group][index] + change
                        new_val = max(min_val, min(max_val, new_val))
                        thresholds[current_group][index] = new_val
                        print(f"阈值更新: {value_labels[index]} = {thresholds[current_group][index]}")
                        time.sleep_ms(100)

        for btn in buttons:
            x, y, w, h = btn['rect']
            color = btn['color']
            if btn.get('action') in ["switch", "toggle_display", "exit"]:
                draw_botton(img, x, y, w, h, color, btn['text'], btn['text_color'], 30, center_text=True)
            else:
                draw_botton(img, x, y, w, h, color, btn['text'], btn['text_color'], 30)

        # 优化阈值显示区域 - 利用800px宽度
        # 顶部状态栏
        img.draw_rectangle(150, 5, 500, 30, color=(50, 50, 50), fill=True)
        img.draw_string_advanced(160, 10, 20, f"当前组: 第{current_group+1}组", color=(255, 255, 255))
        img.draw_string_advanced(300, 10, 20, f"模式: {'阈值处理' if show_binary else '原始画面'}", color=(255, 255, 255))

        # 阈值数值显示区域 - 更大更清晰
        display_y = HEIGHT - 80
        start_x = 150
        gap_x = 100  # 增加间距，适配800px宽度

        # 绘制阈值背景
        img.draw_rectangle(140, display_y - 10, 520, 70, color=(30, 30, 30), fill=True)
        img.draw_rectangle(140, display_y - 10, 520, 70, color=(100, 100, 100), thickness=2)

        for i, label in enumerate(value_labels):
            x_pos = start_x + i * gap_x
            # 当前值 - 更大字体
            display_text = f"{label}: {thresholds[current_group][i]}"
            img.draw_string_advanced(x_pos, display_y, 18, display_text, color=(0, 255, 255))
            # 原始值 - 小字体参考
            orig_text = f"({original_thresholds[current_group][i]})"
            img.draw_string_advanced(x_pos, display_y + 25, 14, orig_text, color=(255, 150, 150))
            # 标签说明
            img.draw_string_advanced(x_pos, display_y + 45, 12, label, color=(200, 200, 200))

        if save_hint_counter > 0:
            img.draw_string_advanced(WIDTH // 2 - 60, HEIGHT // 2 - 20, 25, "阈值已保存!", color=(255, 0, 0))
            save_hint_counter -= 1

        Display.show_image(img)
        gc.collect()
        time.sleep_ms(5)

        last_touch = bool(touch_point)

# ==================== 功能模板函数 ====================
def run_custom_function(sensor, MY_THRESHOLDS, tp):

    """
    自定义功能函数模板
    参数:
        sensor: 摄像头对象
        MY_THRESHOLDS: 阈值列表，可通过阈值调节界面修改
        tp: 触摸屏对象
    """

    # ========== 初始化部分（通常不需要修改） ==========

    # 选择使用的阈值（0或1）
    color_index = 0
    threshold = MY_THRESHOLDS[color_index]

    # 基础变量
    clock = time.clock()
    last_touch = False

    # 退出按钮位置和尺寸 - 适配800x480
    exit_btn_x, exit_btn_y = WIDTH - 120, 10
    exit_btn_w, exit_btn_h = 110, 70

    def is_point_in_button(x, y, btn_x, btn_y, btn_w, btn_h):
        """检查点是否在按钮范围内"""
        return btn_x <= x <= btn_x + btn_w and btn_y <= y <= btn_y + btn_h

    def draw_exit_button(img, x, y, w, h, text="退出",
                        bg_color=(200, 200, 255), text_color=(0, 0, 0),
                        font_size=30, pressed=False):
        """绘制退出按钮"""
        color = (0, 255, 255) if pressed else bg_color
        img.draw_rectangle(x, y, w, h, color=color, thickness=2)
        # 居中显示文字
        text_width = int(len(text) * font_size * 0.6)
        tx = x + (w - text_width) // 2
        ty = y + (h - font_size) // 2
        img.draw_string_advanced(tx, ty, font_size, text, color=text_color)

    try:
        # ========== 您的自定义变量（在这里添加） ==========
        # 例如：
        # your_variable = 0
        # your_data_list = []

        while True:
            clock.tick()
            os.exitpoint()

            img = sensor.snapshot(chn=CAM_CHN_ID_0)

            # 检查触摸屏输入
            points = tp.read(1)
            touch_point = points[0] if points else None

            # ========== 退出按钮处理（通常不需要修改） ==========
            exit_pressed = False
            if touch_point and not last_touch:
                if is_point_in_button(touch_point.x, touch_point.y,
                                    exit_btn_x, exit_btn_y, exit_btn_w, exit_btn_h):
                    print("退出自定义功能")
                    return  # 退出函数，返回主界面

            draw_exit_button(img, exit_btn_x, exit_btn_y, exit_btn_w, exit_btn_h,
                           pressed=exit_pressed)

            # ========== 您的功能代码（在这里编写） ==========
            """
            在这里编写您的具体功能代码，例如：

            # 1. 图像处理
            blobs = img.find_blobs([threshold], merge=True)

            # 2. 检测结果处理
            if blobs:
                for blob in blobs:
                    # 您的处理逻辑
                    pass

            # 3. 按键响应
            if key.is_pressed():
                # 您的按键处理逻辑
                pass

            # 4. 串口通信
            data = uart.read(decode=True)
            if data:
                # 处理接收到的数据
                pass

            # 5. 显示信息
            img.draw_string_advanced(0, 70, 30, "您的状态信息", color=(255,255,255))
            """

            # ========== 基础显示（透明模式） ==========
            # 顶部状态信息 - 透明背景，直接叠加在图像上
            img.draw_string_advanced(10, 10, 25, f"FPS: {clock.fps():.1f}", color=(0, 255, 0))
            img.draw_string_advanced(150, 10, 25, "自定义功能模式", color=(255, 255, 255))
            img.draw_string_advanced(400, 10, 20, f"分辨率: {WIDTH}x{HEIGHT}", color=(200, 200, 200))
            Display.show_image(img)

            # 更新触摸状态
            last_touch = bool(touch_point)

    except KeyboardInterrupt as e:
        print("用户中断: ", e)
    except Exception as e:
        print(f"发生错误: {e}")
    finally:
        # 不需要释放Display和MediaManager，因为它们在主程序中管理
        print("退出模式1")
        time.sleep(0.1)

# ==================== 主程序 ====================

#初始化
sensor = Sensor(width=1920, height=1080)
sensor.reset()
sensor.set_framesize(chn=CAM_CHN_ID_0, width=WIDTH, height=HEIGHT)
sensor.set_pixformat(Sensor.RGB565, chn=CAM_CHN_ID_0)
Display.init(Display.ST7701, width=WIDTH, height=HEIGHT, to_ide=True)
MediaManager.init()
sensor.run()

#触屏初始化
tp  = TOUCH(0)

#自定义阈值 - 从文件加载
print("正在加载阈值配置...")
MY_THRESHOLDS = read_threshold()
print(f"当前阈值: {MY_THRESHOLDS}")

#主界面
while True:
    os.exitpoint()
    img = sensor.snapshot(chn=CAM_CHN_ID_0)
    img.clear()
    img.draw_rectangle(0, 0, WIDTH, HEIGHT, color=(255,255,255), fill=True)

    # 优化主界面布局 - 适配800x480
    # 标题区域
    img.draw_rectangle(0, 0, WIDTH, 80, color=(70, 130, 180), fill=True)
    img.draw_string_advanced(WIDTH//2 - 100, 20, 35, "K230 控制面板", color=(255, 255, 255))
    img.draw_string_advanced(WIDTH//2 - 80, 50, 20, "分辨率: 800x480", color=(200, 220, 255))

    # 功能按钮区域 - 更大更美观
    btn_width, btn_height = 250, 150
    btn_y = 120
    spacing = 50

    # LAB设置按钮 - 添加详细说明
    btn1_x = (WIDTH - btn_width * 2 - spacing) // 2
    # 绘制按钮背景
    img.draw_rectangle(btn1_x, btn_y, btn_width, btn_height, color=(255, 140, 0), fill=True)
    img.draw_rectangle(btn1_x, btn_y, btn_width, btn_height, color=(200, 100, 0), thickness=3)
    # 主标题
    img.draw_string_advanced(btn1_x + 50, btn_y + 30, 32, "LAB阈值设置", color=(255, 255, 255))
    # 副标题说明
    img.draw_string_advanced(btn1_x + 30, btn_y + 70, 20, "调整颜色检测参数", color=(255, 255, 200))
    img.draw_string_advanced(btn1_x + 60, btn_y + 95, 18, "Color Detection", color=(255, 255, 200))

    # 检测按钮点击
    points = tp.read(1)
    touch = points[0] if points else None
    if touch and btn1_x <= touch.x <= btn1_x + btn_width and btn_y <= touch.y <= btn_y + btn_height:
        print("进入阈值调节")
        run_threshold_ui(sensor, tp, MY_THRESHOLDS, WIDTH, HEIGHT)
        print("修改后的阈值:", MY_THRESHOLDS)
        write_threshold(MY_THRESHOLDS)

    # 模式1按钮 - 添加详细说明
    btn2_x = btn1_x + btn_width + spacing
    # 绘制按钮背景
    img.draw_rectangle(btn2_x, btn_y, btn_width, btn_height, color=(34, 139, 34), fill=True)
    img.draw_rectangle(btn2_x, btn_y, btn_width, btn_height, color=(20, 100, 20), thickness=3)
    # 主标题
    img.draw_string_advanced(btn2_x + 60, btn_y + 30, 32, "自定义功能", color=(255, 255, 255))
    # 副标题说明
    img.draw_string_advanced(btn2_x + 40, btn_y + 70, 20, "开发您的视觉算法", color=(200, 255, 200))
    img.draw_string_advanced(btn2_x + 50, btn_y + 95, 18, "Custom Function", color=(200, 255, 200))

    # 检测按钮点击
    if touch and btn2_x <= touch.x <= btn2_x + btn_width and btn_y <= touch.y <= btn_y + btn_height:
        print("进入自定义功能")
        run_custom_function(sensor, MY_THRESHOLDS, tp)

    # 底部信息栏
    info_y = HEIGHT - 60
    img.draw_rectangle(0, info_y, WIDTH, 60, color=(50, 50, 50), fill=True)
    img.draw_string_advanced(20, info_y + 10, 20, f"当前阈值组数: {len(MY_THRESHOLDS)}", color=(255, 255, 255))
    img.draw_string_advanced(20, info_y + 35, 16, "触摸按钮进入对应功能", color=(200, 200, 200))
    img.draw_string_advanced(WIDTH - 200, info_y + 20, 18, "K230 Vision System", color=(100, 200, 255))

    Display.show_image(img)
    gc.collect()
    time.sleep_ms(50)
