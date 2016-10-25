#!/bin/bash

set -e

rm -f ./PiccoloPageRank.bin

mpic++ PiccoloPageRank.cpp -lpthread -std=c++14 -O2 -o PiccoloPageRank.bin

echo "Finish compiling, starting running programs --"
mpiexec -hosts localhost -n 2 bash -c "ulimit -s unlimited ; ./PiccoloPageRank.bin"

rm -f ./PiccoloPageRank.bin

# 初始方式：1）数据格式：发送格式；2） 发送线程、接收线程；3）分布式加载图、分布式保存图；
# 分区方式划分组别：O（1）/O（1）分区、O（1）/O（PARITION）分组；
# 弱一致性优化：1）发送格式：调整缓存区大小、溢出或同步时flush；
# _checkpoint：1）保存检查点：对等方分布式快照算法收集Marker、并进行一次数据表+元数据的状态保存；2）起始恢复：加载文件中状态并恢复到数据表，从指定的迭代步开始计算；
# MapReduce迭代方式：./make_mapred.sh 单节点、多节点运行时间差异；
# 硬盘数据交换：Swap_Out(), Swap_In(id, part)

