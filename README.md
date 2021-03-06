# Computer Graphics

本仓库为MegaShow在《计算机图形学》课程中的作业源码、报告存放仓库。

## Report

[Homework 1 Learning about CG](Docs/Homework%201)

[Homework 2 Gui and Draw simple graphics](Docs/Homework%202)

[Homework 3 Draw line](Docs/Homework%203)

[Homework 4 Transformation](Docs/Homework%204)

[Homework 5 Camera](Docs/Homework%205)

[Homework 6 Light and Shading](Docs/Homework%206)

[Homework 7 Shadowing Mapping](Docs/Homework%207)

[Homework 8 Bezier Curve](Docs/Homework%208)

相关博客：

[Game | 使用Vcpkg来构建OpenGL项目](https://icytown.com/game/vcpkg-build-opengl/)

## Required

- Windows
- Visual Studio 2019 (支持Vcpkg，理论上2017也支持)
- Python 3 (支持GLAD源码生成)
- Git

## Quick Start

运行源码需要安装Vcpkg。

```
$ git clone https://github.com/Microsoft/vcpkg.git
$ cd vcpkg
$ .\bootstrap-vcpkg.bat
$ .\vcpkg integrate install --triplet x64-windows
```

然后设置`VCPKG_ROOT`环境变量，为Vcpkg所在目录。

修改GLAD的默认portfile，改为使用Core版本和指定API为4.6。

```
$ vcpkg edit glad
```

```cmake
vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS
        -DGLAD_NO_LOADER=OFF
        -DGLAD_EXPORT=OFF
        -DGLAD_INSTALL=ON
        -DGLAD_REPRODUCIBLE=ON
        -DGLAD_SPEC="gl" # {gl,egl,glx,wgl}
        -DGLAD_PROFILE="core" # {core,compatibility}
        -DGLAD_API="gl=4.6"
    OPTIONS_DEBUG
        -DGLAD_GENERATOR="c-debug"
)
```

安装依赖库。

```
$ vcpkg install glfw3:x64-windows glad:x64-windows glm:x64-windows imgui:x64-windows assimp:x64-windows stb:x64-windows
```

使用Visual Studio启动该仓库内的解决方案，运行即可。

## Specify Triplet

Quick Start是使用`x64-windows`平台编译运行程序，Vcpkg支持以下的平台。

```
$ vcpkg help triplet
Available architecture triplets:
  arm-uwp
  arm-windows
  arm64-uwp
  arm64-windows
  x64-linux
  x64-osx
  x64-uwp
  x64-windows
  x64-windows-static
  x86-uwp
  x86-windows
  x86-windows-static
```

如果需要修改平台，先需要安装相应平台的库，然后修改`OpenGL.vcxproj`和`ImGui.vcxproj`。

```xml
<PropertyGroup Label="Globals">
  <!-- .... -->
  <VcpkgTriplet Condition="'$(Platform)'=='Win32'">x86-windows-static</VcpkgTriplet>
  <VcpkgTriplet Condition="'$(Platform)'=='x64'">x64-windows-static</VcpkgTriplet>
</PropertyGroup>
```

