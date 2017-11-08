以下仅对文件序列化部分（Bench::run_terark 的头两行），将 terark 中的其他序列化全部排除在外。

当注释掉 boost 的模板实例化代码后，ben_boost.obj 是 735k，加上则是 1795k
当注释掉 terark 对文件的模板实例化代码后，ben_terark.obj 是 802k，加上则是 1150k
因此可以算出，terark 的模板实例化代码在 obj 中的尺寸约 350k，boost 则是 1060 k，大了 3 倍左右

在这个测试中，编译时间：ben_terark.cpp 耗时 3 秒，ben_boost.cpp耗时 5 秒
同样的，也仅计算 Bench::run_terark 的前两行，当然，编译时间也有固定开销，并且也不准确，希望能有更准确的测试

