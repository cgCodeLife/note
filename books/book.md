##主要资料来源: 来自知乎的分布式系统收藏栏: https://zhuanlan.zhihu.com/p/91431718
###分布式系统CA，因果一致性的介绍相关paper: http://reiddraper.github.io/distreader/
###Google论文：设计一个高可用的全球分布式存储系统: google-globall-distributed-storage-systems.pdf
###微软的分布式存储平台, 除了支持类S3对象存储，还支持表格、队列等数据模型. 主要技术特点：采用Stream/Partition两层设计（类似BigTable）;写错（写满）就封存Extent,使得副本字节一致, 简化了选主和恢复操作; 将S3对象存储、表格、队列、块设备等融入到统一的底层存储架构中.: zure.pfd
###从工程实现角度说明了Paxo在chubby系统的应用， 是理解Paxo协议及其应用场景的必备论文。 主要技术特点： paxo协议， replicated log: paxos工程应用-chubby.pdf
###Amazon设计的高可用的kv系统,主要技术特点：综和运用一致性哈希,vector clock,最终一致性构建一个高可用的kv系统， 可应用于amazon购物车场景: amazon-dynamo.pdf

###分布式存储系统架构: distributed-storage-systems-architecture.pdf
###数据结构与算法C++描述第四版: 链接: https://pan.baidu.com/s/1gy7YheEYAebieV43ym3gVw  密码: fhii
......
内容太多了，后续需要什么从梳理的资料中查

