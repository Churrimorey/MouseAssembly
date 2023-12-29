# MouseAssembly
CG project imitating mouse assembly workshop.

参考[ABB机器人IRB 120鼠标装配工作站-ABB13316608765_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1W64y1u7LL/?spm_id_from=333.1007.top_right_bar_window_history.content.click&vd_source=354c3e5aed42e0fa3fff228c9fee5f31)

## 说明！！！

+ 在OBJ导入部分用了一个线程不安全的函数，需要在预编译器重新设置流程如下：

  + 右击项目名称选择属性![./media/image-20231226180910901.png](./media/image-20231226180910901.png)

  + 找C/C++下的预编译器选项![./media/image-20231226180947629.png](./media/image-20231226180947629.png)

  + 在预处理器定义中加入_CRT_SECURE_NO_WARNINGS

    ![./media/image-20231226181034221png](./media/image-20231226181034221.png)

+ 读取OBJ文件因为是相对路径，如果发现模型导入失败，可以检查文件路径是否正确



##文件结构说明

├─build

├─media

└─src

- build：可执行文件，但需要注意涉及到的各类图像资源的路径问题
- media：`README`用到的图像
- src：源代码+纹理图像+模型文件

## 建模部分

> 12.29

### nurbs绘制机械臂

![nurbs](.\media\nurbs.png)

### Obj导入

新加入了ObjLoader类，如果需要导入鼠标意外的OBJ模型可调用这个类，提供了导入OBJ和绘制该OBJ模型的方法

目前绘制鼠标需要调用类Mouse的成员函数，改变了以前直接调用静态函数的方式



最新效果：

鼠标外壳![mouseInLight](./media/OBJ_mouse_tip.png)

鼠标底座![mouseInLight](./media/OBJ_mouse_base.png)

鼠标拼装效果![mouseInLight](./media/OBJ_mouse_whole.png)

## 视角漫游

### 视角漫游方式说明

为了防止混淆视觉，在窗口中心添加中心点表示视角中心；

关于视角漫游各个按键功能如下：

* 按下`w`键，相机向前移动；
* 按下`a`键，相机向左移动；
* 按下`d`键，相机向右移动；
* 按下`s`键，相机向后移动；
* 按下`↑`键，相机向上移动；
* 按下`↓`键，相机向下移动；

当鼠标左键按下时，可以通过移动鼠标进行视角旋转。

### FPS显示

添加在窗口的右上方，在动画实现时可实时监测当前的帧率，做出改进。

下图是完成效果：

![image-20231227144345583](./media/image-20231227144345583.png)
