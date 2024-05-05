# tinykv 🌸 
[![codecov](https://codecov.io/github/Haotian-Yang/JisooDB/branch/main/graph/badge.svg?token=HBA2UV1I5C)](https://codecov.io/github/Haotian-Yang/JisooDB)

A key-value store engine optimized for writing stream of random items, using log-structured approach and write-ahead logging based on [Bitcask](https://riak.com/assets/bitcask-intro.pdf).

## Build & Run
Build:
```
make all
```

Run: `set`, `get`, `del`
```
$./engine set 🔑 🌸
$./engine get 🔑
🌸
$./engine set 🔑 🌹
$./engine get 🔑
🌹
$./engine del 🔑
$./engine get 🔑
$
```

Testing:
```
make all_test
make clean
```

## Design

Disk Design:
- The disk design uses `write-ahead logging` (WAL) to support fast writes, especially when writing an incoming `stream of random items`.

Data Encoding:
```
| key size(4 Bytes) | value size(4 Bytes) | status(1 Bytes) | key(Variable) | value(Variable) | timestamp(23 Bytes)|
```

Index Design: 
- Implement `Skip List` as in-memory indexer to support fast `insert/get/delete` operation.
- Single seek to retrieve data. Index store the direct position to retrieve value for each key.

Limitation:
- While we store all the key and value into disk, but all key need to be keep in the memory. Enough memory is required.


Write Process:
- During the writing process, any update(`set/del`) operation creates a log that is appended to the end of the data file. 
- Then, the corresponding key's index is updated with an updated file ID and offset. If the key has been deleted, it is removed from the index.

Read Process:
- The `get key` involves looking up the index(skip list) to find the `position` (data file ID and offset of the key). If we cannot find it in the index, then it does not exist in our data store. 
- Otherwise, we retrieve its value from disk according to this `position`.

Delete Process:
- As mentioned in the write process, the `del key` operation writes a tombstone to the disk without actually removing the log of the `key`. 
- The data position is then removed from the index. The stale data is cleaned up later during the `merge` process.

Boot Process:
1. Load all the data file with increasing order of the file id
2. Load the data into index
3. While we loading the log data, we remove a key that is stored in the index if the key was marked as deleted.

TODO:
- [ ]: Support Merge operation to merge the data files and cleanup outdated logs
- [ ]: Add CRC into encoded data to support crash recovery and data consistency
- [ ]: Support DB iterator
- [ ]: Task Processing Layer for incoming client request
