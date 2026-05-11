# ConcurrentMemoryPool 说明文档

## 1. 项目简介

`ConcurrentMemoryPool` 是一个参考 **TCMalloc 分层思想**实现的高并发内存池（Windows/VC++ 环境），目标是降低多线程频繁 `malloc/free` 的锁竞争与系统调用开销。

核心思路：
- **ThreadCache（线程私有）**：优先满足小对象分配，避免线程间竞争。
- **CentralCache（进程共享）**：在线程缓存不足时提供批量对象。
- **PageCache（页级管理）**：管理大块页内存，负责切分与合并。

---

## 2. 项目结构

主要代码位于 `ConcurrentMemoryPool/`：

- `Common.h`：全局常量、基础工具、`FreeList`、`SizeClass`、`Span/SpanList`
- `ThreadCache.h/.cpp`：线程本地缓存与小对象分配逻辑
- `CentralCache.h/.cpp`：中心缓存，向 `ThreadCache` 批量供给对象
- `PageCache.h/.cpp`：页缓存，向系统申请/归还页并进行相邻合并
- `PageMap.h`：页号到 `Span*` 的映射结构（当前使用 `TCMalloc_PageMap1`）
- `ObjectPool.h`：用于 `Span`/`ThreadCache` 等对象的定长对象池
- `ConcurrentAlloc.h`：统一分配/释放接口
- `benchmark.cpp`：性能对比基准（`ConcurrentAlloc` vs `malloc`）
- `UnitTest.cpp`：功能验证代码（多数为注释示例）
- `test.cpp`：对象池实验代码

工程文件：
- `ConcurrentMemoryPool.sln`
- `ConcurrentMemoryPool/ConcurrentMemoryPool.vcxproj`

---

## 3. 内存分级与关键参数

定义见 `Common.h`：

- `NFREELIST = 208`：自由链表桶数
- `MAX_BYTES = 256 * 1024`：ThreadCache 单次处理上限（256KB）
- `PAGE_SHIFT = 13`：页大小 `1 << 13 = 8KB`
- `PAGE_NUM = 129`：PageCache 最大桶页数边界（1~128 页）

### 小对象与大对象边界
- `size <= 256KB`：走 `ThreadCache -> CentralCache -> PageCache`
- `size > 256KB`：按页对齐后直接从 `PageCache` 申请 Span

---

## 4. 设计总览

### 4.1 ThreadCache（线程级）
每个线程持有一组 `FreeList` 桶：
- 分配时：
  1. 根据 `size` 做对齐（`SizeClass::RoundUp`）
  2. 计算桶下标（`SizeClass::Index`）
  3. 桶非空直接弹出
  4. 桶为空则向 `CentralCache` 批量拉取
- 释放时：
  1. 回收到对应桶
  2. 若桶长度超过慢启动阈值，批量归还 `CentralCache`

### 4.2 CentralCache（中心级）
按对象大小维护 `SpanList` 桶（共享，需要加锁）：
- 向 `ThreadCache` 提供 `[start, end]` 对象区间
- 若当前桶无可用 Span，则向 `PageCache` 申请新 Span 并切分为对象链
- 回收 `ThreadCache` 归还对象，若某 Span 全部归还（`_useCount == 0`），则返还 `PageCache`

### 4.3 PageCache（页级）
按页数维护 Span 桶，并管理页号映射：
- `NewSpan(k)`：优先从 k 桶取；否则拆更大桶；再不行向系统申请 128 页
- `ReleaseSpanToPageCache(span)`：尝试左右相邻合并，最终挂回对应桶
- 超大 Span（>128 页）直接 `SystemFree`

---

## 5. 核心数据结构

### 5.1 FreeList
单链表管理同尺寸对象：
- `Push/Pop`
- `PushRange/PopRange`（批量操作）
- `MaxSize` 用于慢启动控制

### 5.2 SizeClass
负责尺寸与桶映射策略：
- `RoundUp(size)`：按分区粒度对齐
- `Index(size)`：映射到 208 个桶
- `NumMoveSize(size)`：每次批量搬运对象数（2~512）
- `NumMovePage(size)`：估算一次搬运对应页数

### 5.3 Span / SpanList
- `Span`：描述连续页区间及其切分对象状态
- `SpanList`：带哨兵的双向循环链表，桶内管理 Span

### 5.4 PageMap
通过页号快速找到所属 `Span`，用于 `ConcurrentFree` 时从对象地址反查 Span。

---

## 6. 分配与释放流程

### 6.1 分配（`ConcurrentAlloc(size)`）
1. 若 `size > 256KB`：
   - 页对齐
   - 从 `PageCache::NewSpan` 获取 Span
   - 返回页起始地址
2. 若 `size <= 256KB`：
   - 获取/初始化线程本地 `ThreadCache`
   - 调用 `ThreadCache::Allocate(size)`

### 6.2 释放（`ConcurrentFree(obj)`）
1. 通过 `PageCache::MapObjectToSpan(obj)` 找到 Span
2. 根据 `span->_objSize` 判断对象类别：
   - 大对象：直接归还 `PageCache`
   - 小对象：回收到当前线程 `ThreadCache`，必要时再批量回收至 `CentralCache`

---

## 7. 线程安全策略

- `ThreadCache`：线程私有，常规路径无锁
- `CentralCache`：每个 `SpanList` 桶独立锁（`SpanList::_mtx`）
- `PageCache`：全局页锁（`_pageMtx`）
- `ObjectPool`：使用 `_poolMtx` 保护并发申请

---

## 8. 编译与运行（Windows / Visual Studio）

### 8.1 环境要求
- Visual Studio 2022（工具集 `v143`）
- Windows SDK 10.x

### 8.2 打开工程
- 打开：`ConcurrentMemoryPool.sln`
- 选择配置：`Debug|x64` 或 `Release|x64`
- 构建解决方案后运行

### 8.3 当前入口
`benchmark.cpp` 含 `main()`，默认执行多线程性能对比：
- `BenchmarkConcurrentMalloc(...)`
- `BenchmarkMalloc(...)`

控制参数（在 `main` 中）：
- `n`：每轮每线程分配/释放次数
- `nworks`：线程数
- `rounds`：轮数

---

## 9. 性能测试说明

`benchmark.cpp` 通过相同线程数和轮次比较两种方式：
- 系统 `malloc/free`
- `ConcurrentAlloc/ConcurrentFree`

测试包含：
- 分配耗时
- 释放耗时
- 总耗时

建议在 `Release|x64` 下测试，并固定机器负载后多次取平均值。

---

## 10. 已知问题与检查建议

项目根目录提供了 `逻辑检查清单.md`，其中列出若干需要重点关注的问题（如并发一致性、ODR 风险、页映射生命周期等）。

建议阅读顺序：
1. 并发读写一致性
2. 头文件定义与链接语义
3. 页数换算边界语义
4. 映射表生命周期完整性

---

## 11. 后续可扩展方向

- 补充单元测试与压力测试（不同对象分布、线程数、生命周期模型）
- 增加统计面板（命中率、回收率、桶分布、系统申请次数）
- 增强跨平台支持（Linux `mmap/munmap` 路径）
- 完善超大对象策略与碎片监控

---

## 12. 术语对照

- **TC**：ThreadCache
- **CC**：CentralCache
- **PC**：PageCache
- **Span**：按页连续内存段管理单元
- **PageID**：页号（地址右移 `PAGE_SHIFT`）
