This is a log-structured fast key-value database that has been implemented based on[Bitcask](https://riak.com/assets/bitcask-intro.pdf)

Disk Design;
- The disk design uses `write-ahead logging` (WAL) to support fast writes, especially when writing an incoming `stream of random items`.

Data Encoding:
```
| key size(4 Bytes) | value size(4 Bytes) | status(1 Bytes) | key | value | timestamp(23 Bytes)|
```

Index Design: 
- Implement `Skip List` as in-memory indexer to support fast `insert/get/delete` operation.
- Single seek to retrieve data. Index store the direct position to retrieve value for each key.

Limitation:
- While we store all the key and value into disk, but all key need to be keep in the memory. Enough memory is required.


Write Process:
- During the write process, any update or delete operation creates a log that is appended to the end of the data file. 
- Then, the corresponding key's index is updated with an updated file ID and offset. If the key has been deleted, it is removed from the index.

Read Process:
- The read process involves looking up the index to find the `position` (data file ID and offset of the key). If we cannot find it in the index, then it does not exist in our data store. 
- Otherwise, we retrieve its value from disk according to this `position`.


Boot Process:
1. Load all the data file with increasing order of the file id
2. Load the data into index
3. While we loading the log data, we remove a key that is stored in the index if the key was marked as deleted.

TODO:
- [ ]: Support Merge operation to merge the data files and cleanup outdated logs
- [ ]: Add CRC into encoded data to support crash recovery and data consistency
- [ ]: Support DB iterator
