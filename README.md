```bash
gcc bloom.c btree.c inc.c sha1.c -fPIC -o inc.so -shared && dotnet run
```