# 球谐光照采样

采用pyton写的球谐因子生成工具， 实现采用两种方式

* 纯数组计算
* Numpy加速计算



Numpy加速计算运行main.py 在我的主机（显卡GTX 1060 3GB）快了大概4-5倍的样子

运行结果参见：

```
reading cubemap ...
sampling ...
numpy exec time:1.3972785

100%|██████████| 100000/100000 [00:05<00:00, 19241.61it/s]

cpu exec time:6.490745
```

同样的代码支持[cupy][i1], GPU并行计算， 只需要将代码里的

```py
# 替换 import numpy as np
import cupy as np
```

实际运行下来，由于数据量还是不太大，所以反而还比numpy实现的运行时间更长



#### c++实现

除了这里py实现方式， 还有c++实现(纯CPU计算， 在engine/cv目录下)， 效率比py里cpu计算要好， 后期可以考虑把c++部分运算移植到cuda里计算。


[i1]: https://docs-cupy.chainer.org/en/stable/