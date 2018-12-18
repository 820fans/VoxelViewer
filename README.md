# VoxelViewer
View a voxel model. File format is customized by myself.

> 一次偶然的机会,接触了体素相关的内容.
每次一点点改进都让自己开心非常.

这里,基于`CMake`和`OpenGL`,自行编写了一个体素显示软件.
软件写的不是很好,基本上只能算是一个demo吧.

<img src="img/bunny_512_0.jpeg" width="600" />

# 文件格式说明
```
size 12 12 12 
3.5 4.5 5.5 1 0 
3.5 4.5 4.5 0.5 0
```

第一行表示整个模型的大小，格式为`size x y z`，以空格分开。<br>
从第二行开始，表示一个体素，格式为`xi yi zi voxel_size depth`。<br>
由于`OpenGL`中`drawSolidCube`函数的设计，`xi yi zi`表示当前体素的**中心**点。<br>
`voxel_size` 表示当前体素的边长。<br>
`depth` 表示当前体素的层级，因为体素化程序使用到了多分辨率体素化。<br>
`depth`与`voxel_size`大小相关，符合`voxel_size=1/2^depth`。

虽然这种存储方式比较粗糙，而且体素数量多时会导致文件特别大。但是本着够用就好的原则，没有设计二进制的格式。


# 编译要求
## Windows
在windows环境下，使用的dll和头文件全部放在了文件里。
使用Visual Studio 2013打开。
配置好之后可正常运行。


## Linux
在Ubuntu环境下,要求安装`OpenGL`和`GLUT`

目前可在`Clion`下编译运行

# TODO
- 规范化代码
- 规范化ReadMe
- 输出二进制版本
