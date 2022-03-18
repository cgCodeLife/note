简介
bblot是golang编写的开源的轻量级kv数据库，支持简单的事务，接口语义只有put/get/delete等操作。适合简单的单机kv存储。支持持久化，可选支持数据安全。可用作kv数据库，也可用在缓存服务中。对比同类如leveldb等会更轻量，更简单，没有复杂的实现，没有更复杂的事务。
理念：简单，快速
场景：适合读多写少的场景。(个人理解，不适合大的key-value),随机读写。如小规模元数据存储引擎，支持持久化的缓存索引服务等。
特性：可以做到纯内存操作。
架构
[图片]
用法
bblot是基于文件的db,一个文件代表一个db,所以，使用之前需要关联一个文件。这个文件可以是本地文件，也可以是任意满足golang定义的os.File接口语义的文件描述符。
// 打开一个bblot库
db, err := bbolt.Open(fname, 0600, &bbolt.Options{Timeout: 10 * time.Second})
if err != nil {
   return nil, err
}

// 创建一个bucket,如果没有的话
err = db.Update(func(tx *bbolt.Tx) error {
   _, err := tx.CreateBucketIfNotExists(bucketname)
   if err != nil {
      return err
   }
   return nil
})
if err != nil {
   return nil, err
}

// 写操作
err = db.Upate(func(tx *bbolt.Tx) error {
    // get bucket
    // tx operation
    ...
}

// write transaction failed
if err != nil {
}

// 读操作
err = db.View(func(tx *bbolt.Tx) error {
    // get bucket
    // tx operation
    ...
}

// read transaction failed
if err != nil {
}
任何操作都是在事务里进行的，读写不并发，安全性由db保证。
概念
DB: bbolt kv数据库
Bucket: bbolt的一个存储集合，类似关系数据库的table。可以有多个，支持嵌套。任何数据存储都要基于Bucket。
cursor: key查询器，封装B+树的查找算法并记录查询路径。
Tx: 封装的读写事务。
Node: 内存结构，Page的反序列化。任何写操作都是通过Node进行的。
Page:db的硬盘layout。page大小和操作系统设定的page自动对齐。
数据结构
Page
[图片]
                                                                          图-1 page layout
page是数据在硬盘上的物理结构，此page不是操作系统中内存page的概念，是bblot对硬盘layout的一个抽象。任何数据:bucket,key,value,都是以page为layout的。Page是一个扁平结构，没有嵌套，有四种类型:分支类型，叶子类型，meta数据，空闲页集合。
id:代表该page的id
flags: 代表着四种类型的page中的哪一种
count: 代表的是content中有多少个flags表示的页元素。如果超过了u16表示的大小，会设置该位置的值为0xFFFF,然后在content的前sizeof(Page)中存储实际的count值。如果count溢出，新申请的页也一定是连续存储的，如果存不下，继续标记overflow进行溢出。在分配页的时候，必须一次性分配成功。不支持非连续存储。
overflow: 代表额外申请的页数。只在leaf类型上发生。即便发生了溢出，那也必须是连续的，也就是当前page后连续的page上。这样做是为了简单，不做复杂的空间管理。
分支类型(branch)：因为是基于B+树进行的数据布局，所以page中可能是B+数的内部节点。
[图片]
                                                                             图-2 branch layout
pos: 指的是pos所在位置与key的偏移量
ksize: key的长度。
pgid: 指的是key所在的页id。
其实这里虽然是B+树的结构，但是不完全符合B+树的定义，这里的子节点数量和key的数量是相等的。不过这不影响B+数算法的使用。
叶子类型(leaf): 
[图片]
                                                                            图-3 leaf layout
字段含义同上。但是注意一个flags。这个flags有两个值，0x00,0x01, 0x00代表是普通的leaf，0x01代表这是一个Bucket,是个嵌套Bucket。Bucket稍微有些复杂一些，Bucket环节可以看到。
meta类型：
[图片]
                                                                         图-4 meta layout
meta：总是在固定的位置，只在第0,1号page上，所以page head里的count,overflow都不会用到。
magic：是个固定的数组，0xED0CDAED
version：是该数据结构定义的版本，现在固定是2。
pagesize: 对齐的操作系统页大小，一般是4096bytes。
flags: 无用。
root: 类型是bucket{root: pgid(type uint64), sequence: uint64},用来作为bucket的跟节点。就是B+数的跟节点。
pgid: 记录当前使用过的最大的pgid号，用来作为是否需要扩容DB的依据。
txid：DB历史最大事务id。
checksum: meta结构的crc32。
meta有两个，分别是0,1，固定在0,1页面上，没有其它备份。0,1其实就是一种备份，那决定用哪个呢，就是根据txid最大的那个就是最新的，如果无法启动，数据损坏，可以选择回退到使用另一个meta,那么此时上次写入的数据是被丢弃的。要做到这一点，其实有两个地方要保证，一是回收的COW的旧页还没有overwrite，这一点肯定能做到，二是DB只能扩容，不能缩容。
[图片]
                                                                       图-5 freelist layout
count,overflow都可能用上，count还有可能溢出。这个通常发生freelist总是不满足空闲页的申请，不断的从DB文件里获取新的更大的连续页的情况。pgid是升序存储，每个pgid代表的都是可复用的page。
Node
纯内存结构，对Page的写操作，在内存中都是对Node的操作，Node和Page会通过序列化和反序列化相互转换。Node存在的目的是为了方便进行B+树的调节操作。这样利用这些Node会在新的Page上构建新的视图。
type node struct {
   bucket     *Bucket  // 对应的Bucket
   isLeaf     bool // 对应的page是否是叶子节点
   unbalanced bool  // 是否未进行B+树的合并
   spilled    bool  // 是否已经进行过B+树的分裂
   key        []byte  // branch/leaf中的key们
   pgid       pgid  //对应的页id
   parent     *node  // 父节点
   children   nodes  // 子节点
   inodes     inodes  // 兄弟节点
}

type inode struct {
   flags uint32
   pgid  pgid
   key   []byte
   value []byte
}

type inodes []inode
Page->Node:
通过node::read(*page)进行反序列化，按照page的格式，会初始化pgid, isLeaf。然后通过解析content，按照isLeaf解析出对应leaf/branch类型的信息，将解析的node存放在inodes中。
inode其实就是对leaf类型的page的一个封装。如果遇到branch类型的，value就是nil，其它不变。这里即便是leaf，也有可能是Bucket。
Node->Page:
通过接口node::write(*page)将信息写到page上。和read刚好相反，计算好offset,然后循环遍历inodes数组，逐一写到page里即可。逐一这里不涉及实际Page的申请，次Page只是基于内存数组按照Page的layout写入的数据，在最后事务提交的时候会看到如何将数据写到实际的Page上。
但是可以看到，实际这里进行了数据的copy,所以如果确实是大块的key-value,其实bblot并不适合你的场景。而且B+树在做分裂和合并的时候也要realloc,进行必要的数据copy操作。
bblot本身也也对此作了限制，限制该page大小不能超过32GB，是个固定上限。不过这个限制个人感觉太宽松了，实际你单次写了上GB的数据，一定很慢。
Bucket
你可以直接理解，它就是table,关系数据库的table,但是它支持嵌套。为什么要这玩意呢，主要还是为了做数据分类的，无论有多少个平行的Bucket还是嵌套多少个Bucket,它们在B+树上都是普通leaf类型的节点，但是嵌套Bucket是它长得那么完全符合B+树的形式，但是并不妨碍基于B+树的逻辑进行数据管理，而且它对业务有用，业务根据它可以做数据分类。
// 内存结构
type Bucket struct {
   *bucket
   tx       *Tx                // the associated transaction
   buckets  map[string]*Bucket // subbucket cache
   page     *page              // inline page reference
   rootNode *node              // materialized node for the root page.
   nodes    map[pgid]*node     // node cache

   // Sets the threshold for filling nodes when they split. By default,
   // the bucket will fill to 50% but it can be useful to increase this
   // amount if you know that your write workloads are mostly append-only.
   //
   // This is non-persisted across transactions so it must be set in every Tx.
   FillPercent float64
}

// bucket represents the on-file representation of a bucket.
// This is stored as the "value" of a bucket key. If the bucket is small enough,
// then its root page can be stored inline in the "value", after the bucket
// header. In the case of inline buckets, the "root" will be 0.
// 持久化结构
type bucket struct {
   root     pgid   // page id of the bucket's root-level page
   sequence uint64 // monotonically incrementing, used by NextSequence()
}
对于一个新建的Bucket,它是一个inline布局的，类似Redis的数据压缩一样，当数据量比较少的时候采用的layout和数据量多的时候是不一样的，这里inline和常规B+数的差别就是Bucket会在一个page内存储所有的信息。
[图片]
                                                              图-6 inline Bucket layout
其中，bucket后边的内嵌了一个完整的Page结构，但是这个是个假的Page，没有实际对应一个独立的Page,id是固定值，为0。
注意FillPercent，这个会决定一个Page里装多大的数据，能决定空闲率。
DB
DB对应的持久化结构就是一个文件。通过mmap对应到内存中一个非常大的数组中，任何page的访问都是以一个操作系统的page大小为单位进行的。
// DB represents a collection of buckets persisted to a file on disk.
// All data access is performed through transactions which can be obtained through the DB.
// All the functions on DB will return a ErrDatabaseNotOpen if accessed before Open() is called.
type DB struct {
   // When enabled, the database will perform a Check() after every commit.
   // A panic is issued if the database is in an inconsistent state. This
   // flag has a large performance impact so it should only be used for
   // debugging purposes.
   StrictMode bool

   // Setting the NoSync flag will cause the database to skip fsync()
   // calls after each commit. This can be useful when bulk loading data
   // into a database and you can restart the bulk load in the event of
   // a system failure or database corruption. Do not set this flag for
   // normal use.
   //
   // If the package global IgnoreNoSync constant is true, this value is
   // ignored.  See the comment on that constant for more details.
   //
   // THIS IS UNSAFE. PLEASE USE WITH CAUTION.
   NoSync bool

   // When true, skips syncing freelist to disk. This improves the database
   // write performance under normal operation, but requires a full database
   // re-sync during recovery.
   NoFreelistSync bool

   // FreelistType sets the backend freelist type. There are two options. Array which is simple but endures
   // dramatic performance degradation if database is large and framentation in freelist is common.
   // The alternative one is using hashmap, it is faster in almost all circumstances
   // but it doesn't guarantee that it offers the smallest page id available. In normal case it is safe.
   // The default type is array
   FreelistType FreelistType

   // When true, skips the truncate call when growing the database.
   // Setting this to true is only safe on non-ext3/ext4 systems.
   // Skipping truncation avoids preallocation of hard drive space and
   // bypasses a truncate() and fsync() syscall on remapping.
   //
   // https://github.com/boltdb/bolt/issues/284
   NoGrowSync bool

   // If you want to read the entire database fast, you can set MmapFlag to
   // syscall.MAP_POPULATE on Linux 2.6.23+ for sequential read-ahead.
   MmapFlags int

   // MaxBatchSize is the maximum size of a batch. Default value is
   // copied from DefaultMaxBatchSize in Open.
   //
   // If <=0, disables batching.
   //
   // Do not change concurrently with calls to Batch.
   MaxBatchSize int

   // MaxBatchDelay is the maximum delay before a batch starts.
   // Default value is copied from DefaultMaxBatchDelay in Open.
   //
   // If <=0, effectively disables batching.
   //
   // Do not change concurrently with calls to Batch.
   MaxBatchDelay time.Duration

   // AllocSize is the amount of space allocated when the database
   // needs to create new pages. This is done to amortize the cost
   // of truncate() and fsync() when growing the data file.
   AllocSize int

   // Mlock locks database file in memory when set to true.
   // It prevents major page faults, however used memory can't be reclaimed.
   //
   // Supported only on Unix via mlock/munlock syscalls.
   Mlock bool

   path     string
   openFile func(string, int, os.FileMode) (*os.File, error)
   file     *os.File
   dataref  []byte // mmap'ed readonly, write throws SEGV
   data     *[maxMapSize]byte
   datasz   int
   filesz   int // current on disk file size
   meta0    *meta
   meta1    *meta
   pageSize int
   opened   bool
   rwtx     *Tx
   txs      []*Tx
   stats    Stats

   freelist     *freelist
   freelistLoad sync.Once

   pagePool sync.Pool

   batchMu sync.Mutex
   batch   *batch

   rwlock   sync.Mutex   // Allows only one writer at a time.
   metalock sync.Mutex   // Protects meta page access.
   mmaplock sync.RWMutex // Protects mmap access during remapping.
   statlock sync.RWMutex // Protects stats access.

   ops struct {
      writeAt func(b []byte, off int64) (n int, err error)
   }

   // Read only mode.
   // When true, Update() and Begin(true) return ErrDatabaseReadOnly immediately.
   readOnly bool
}
cursor
读写操作都通过cursor进行定位。
type Cursor struct {
   bucket *Bucket
   stack  []elemRef
}
主要流程
DB加载
区分新库还是旧库，通过文件是否存在或者文件大小是否为0判断，如果满足以上任意条件 ，则认为是新库，会进行DB初始化。加载的主要流程如下：
1. 如果是新库
  1. 构建两个metaPage 因为meta的位置总是固定为0,1所以直接构造Page内存并设置pgid分别为0,1，freelist地址设置为2，root地址设置为3，pgid(当前已经使用的page的最大值)4。
  meta地址永远不变，其它都会变。
  2. 构建freelistPage
  一般情况下，对于做过释放操作的页，都会记录在freelist中，未曾使用过的页，也算free page但是不会记录在freelist中，在分配可用空间的时候配合meta中记录的pgid，即最大使用过的pgid来从这个页开始之后进行申请。
  非一般情况指的是如果你不对freelist做同步持久化的话，需要全盘扫描B+树来构建freelist。
  3. 构建leafPage
  leaf没啥好构建的，设置pgid为3。代表整个DB的根。这个位置可能会发生变化。
2. 否则，直接读取第一个page size大小的数据进行有效性校验。主要校验magic, version等。
3. db做mmap
  1. 加载两个meta
  2. 做文件mmap
4. 重建freelist 从meta信息拿到freelist页地址，加载freelist页并反序列化。freelist支持两种实现，hash和数组。
写流程
1. 创建读写事务对象
  1. meta
  2. freelist
2. 回调业务handler
3. 提交/回滚
  1. 均衡
  2. freelist commit (alloc more free space)
读流程
1. 创建只读事务
2. 回调业务handler
3. 回滚
事务
bbolt支持的事务比较简单，没有像关系数据库那样的详细的事务隔离级别的划分。它保证的是读写线程安全和原子操作。读写事务不是完全串行的，而是可以并发的。基于COW机制，读写看到的B+树的视图不同。
读、写事务开始的时候都会将meta信息copy一次，在各自的事务视图上没有内存交集，而且每次事务都需要重新从Page上加载数据。
读事务
读事务只做二分查找或者遍历，对Page的访问是只读的，可以和写事务并发执行，但是读事务会在整个事务声明周期内一直持有mmap的读锁，目的是为了防止写操作触发mmap重新映射的情况。所以需要注意的是，不能让读写操作在同一个goroutine并发执行。
这里个人感觉读写并发可能会存在数据安全问题：
场景：tx1:读事务begin{foreatch整个DB}-->tx2:写事务{}->....txn:写事务{复用了之前释放的页pgid}->tx1{遍历到pgid,但是这个pgid可能已经不属于要遍历的bucket了。}。
不过这个我们实际生产环境没遇到过。
写事务
写事务不允许写写并发，所以一开始会直接拿一把读写锁，防止多写，和读事务不同的地方如下：
1. Node的缓存
读事务不使用Node,直接通过cursor进行二分查找，写事务要将访问过的Page反序列化出来到Node上。
2. mmap的管理
在写的过程中，可能会出现当前映射的大小太小的情况，所以bblot是运行自行进行扩容重新映射的，但是这个映射要拿mmap锁，这一点会和读事务冲突。使用不当可能会死锁。
3. 旧空间的释放
基于COW,对原先的page可能需要放在freelist中等待释放。
4. 数据更新
通过fdatasync直接刷新，但是这个是可选项。
rollback
读写事务都可能发生，其中读事务主要是将自己的tx记录从db事务列表删除。其实主要是给新的写事务告知可释放的pending page列表是哪些。
写事务要做的是释放pending中的页表并从db重新加载一份freelist视图。
空间管理
layout
[图片]
                                                                           图-7 文件上的page layout
[图片]
                                                                      图-8 单一inline Bucket layout
[图片]
                                                                           图-9 多个inline Bucket layout
图-9只是画了其中一种layout,在B+树的结构中，嵌套的bucket也可以是inline。每个Bucket都有自己的根，inline就是图中的inline Page所在的Page。

[图片]
                                                                      图-10 DB layout 示例
页申请
在业务回调的handler中，不存在申请页面的情况，变更操作都是先通过cursor找到一个Node节点，在该节点进行数据更新。等到提交事务的时候，会通过分裂的方式将旧的页面放在freelist的pending列表中，等待回收，然后copy一份数据到新申请的连续页空间中。申请步骤：
1. alloc from freelist first
2. alloc from data[tx.meta.pgid:count]
空闲页管理
freelist+used != file size可能成立  (个人理解随着长时间的反复擦写之后可能没啥用)。
分两种管理结构：
hashmap:
[图片]
                                                                                     图-11 hashmap结构的freelist
分配特点：尽可能精确匹配size，找不到就全局遍历hashmap找更大连续的并做切分。
释放：前后合并 (tip: 有两个额外的map记录forward和backward)
如图-11举例，如果p19释放了，那么p15-p23将会合并为一个更大的freelist page.
优点：相比array分配快，无论分配size,碎片多少，可用空间如何。
缺点：序列化和反序列化比较复杂，因为最终在disk上的layout都是按照freelist page的格式放置的。
array:
[图片]
                                                                 图-12
分配特点：直接全局遍历，精确匹配，否则分配失败。
释放：将待释放的pagid集合先按照pgid进行排序，然后插入到array中，array保持是一个有序数组。
优点：序列化和反序列化快
缺点：分配相对慢，特别是data比较大或者申请的size比较大但是碎片比较多的时候。

个人理解这里其实不会有太大的碎片问题，主要通过分裂和合并解决。但copy数据比较多，不太适合写太大的key-value。
分裂
分裂原则：
1. 是inline bucket,但是出现以下任意两种情况的：
  1. 创建了子bucket
  2. page使用率超过1/4 操作系统页大小
2. page使用率超过设定的最大阈值，默认50%(Tips:注意大value的处理)
3. 先递归分裂所有子bucket 
4. 再递归分裂所有子节点
合并
合并原则：
1. key-value个数小于最小阈值
  1. leaf:1
  2. branch:2
2. 或者page使用率不足1/4
附件
项目地址：https://github.com/etcd-io/bbolt
一些测试对比：https://blog.csdn.net/huxinglixing/article/details/116156322
cmd工具：https://learn.hashicorp.com/tutorials/vault/inspect-data-boltdb?in=vault/monitoring
