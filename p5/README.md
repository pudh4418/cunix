作业8：

csv2bin.c rec.{c,h} bincat.c recv.{c,h} 为本次作业的主要源代码。

Usage:
  ./csv2bin [file]
  ./bincat [file]

两程序均仅将数据写入标准输出。

输入文件的文件名不受限，且可由 `gen.sh` 脚本生成。

`bincat` 程序因为需要用到 `mmap` 故仅支持普通文件作为其输入。

运行举例：

```
$ ./gen.sh > data.txt
$ ./csv2bin data.txt > d.dat
$ ./bincat d.dat > d2.txt
```

