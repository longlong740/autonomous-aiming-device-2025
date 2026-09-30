# 简易自行瞄准装置｜2025 电赛

本科电子设计竞赛团队代码整理稿，包含 MSPM0G3507 下位机工程和 K230 视觉端程序。

## 目录

- firmware/oled-software-i2c：CCS 工程、控制模块和 TI SysConfig 生成文件。
- vision/k230：图像识别、阈值调节和菜单程序。

## 运行与构建

下位机工程面向 MSPM0G3507，使用 TI Code Composer Studio 和对应 SDK。视觉端程序运行在 K230/MaixPy 环境。模型文件、实物照片、竞赛报告和运行视频没有放入本仓库。

原始工程引用了 MPU6050/InvenSense 驱动，但本地资料中缺少该驱动声明引用的 License.txt，因此这份驱动没有复制到公开仓库。重新构建前需要通过获准渠道取得兼容驱动。TI 生成的配置文件保留了原有许可证声明。

代码没有在本仓库环境中重新构建，也没有连接实物验证。

## 许可证

团队编写的项目代码按 MIT License 发布。第三方组件继续遵循其自身条款，见 THIRD_PARTY_NOTICES.md。
