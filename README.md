# Csas_Engine

#### 介绍

学习Cherno的Hazel系列，已经添加Camera3D,Cube
Spotlight DirectLight
shadow map PCF

#### 建议开发环境

Linux

#### 依赖

GLFW GLAD 等



## **To be done**

particle System 应该管理所有particle（emitter）

particle应该只是持有渲染数据

在editor 界面中，应该显示particle System 这个实体（目前依然当作实体）

其叶子节点可以是一些emitter

**处理逻辑**

用户与particle System 交互其不应访问单个emitter

particle System将创建粒子，位置将由叶子emitter决定

在渲染前将有逻辑帧，system来处理，回收创建等，并将可渲染数据提供给RenderPipeLine



# Issue

https://blog.csdn.net/weixin_42156097/article/details/107036173

vector resize 和 reserve不同



在resize后emplace_back放结构体元素一直是初始值 

**已解决**

## 