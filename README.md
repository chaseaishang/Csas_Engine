# Csas_Engine

#### 介绍

学习Cherno的Hazel系列，已经添加Camera3D,Cube

#### 建议开发环境

Linux

#### 依赖

GLFW GLAD 等

# Issue

如果 glsl 文件包含

 //#include  xxx.glsl

则在 shaderInclude.hpp中，原本用来实现的函数没有考虑到注释，

因此会出错。

**已修改**
