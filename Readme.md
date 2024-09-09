I took the setup from the book "The little book about OS development". Unfortunately Bochs doesn't work so I had to switch to QEMU.

**Setup**:

OS: Ubuntu 22.04 LTS

Toolchain:

```bash
sudo apt-get install build-essential nasm genisoimage

sudo apt-get qemu qemu-system-x86
```

**How to run**:
- Use `make run` to run the OS
- Use `make serial` to run with serial port output capturing
- Use `make debug` to run with debugging symbols and opens a local "remote" port 1234. Use a second window (and go to the same folder) and run `make gdb` to hook gdb to the local "remote" server.

**GDB notes**
`c`:                continue
`b <function>`:     break at a function
`layout src`:       brings a second window showing source code
`layout asm`:       brings a second window showing assembly code
`p \x <var name>`:  print variable value in hex