# whatspic
a picture classification for c++

## train

pytorch

## use

libtorch

> 下载地址 https://pytorch.org/get-started/locally/

```bash
export CUDA_VER=cpu && ./setup-libtorch.sh
```

对于macos中，下载的dylib放到指定目录，第一次打开的时候，会遇到未认证的文件，需要确认权限

```bash
sudo xattr -r -d com.apple.quarantine /usr/local/lib/libtorch
```

无法将依赖libtorch的代码编译成动态或者静态链接库，只能编成可执行文件