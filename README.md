```bash
gcc bloom.c btree.c inc.c sha1.c -fPIC -o es.so -shared && dotnet run
```