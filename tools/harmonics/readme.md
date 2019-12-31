# 球谐光照采样

此工具是采用pyton写的球谐因子生成工具， 实现采用两张方式

* 纯CPU计算
* GPU加速计算

其中gpu的部分实现主要依据是python库-numpy， 利用矩阵并行性质实现得到了

运行main.py 在我的主机（显卡GTX 1060 3GB）快了大概4-5倍的样子

运行结果参见：

```
reading cubemap ...
sampling ...
numpy exec time:1.3972785

100%|██████████| 100000/100000 [00:05<00:00, 19241.61it/s]

cpu exec time:6.490745
```

#### c++实现

除了这里py实现部分， 还有c++实现(纯CPU计算， 实现在engine/cv目录下)， 效率比py里cpu计算要好， 后期可以考虑把c++部分运算移植到cuda里计算。