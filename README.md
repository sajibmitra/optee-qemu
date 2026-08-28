# OP-TEE QEMU (AArch64)

Pinned OP-TEE QEMU v8 workspace (`qemu_v8` manifest) as Git submodules.

## Clone

```bash
git clone --recurse-submodules https://github.com/sajibmitra/optee-qemu.git
cd optee-qemu
```

If you already cloned without submodules:

```bash
git submodule update --init --recursive
```

## Preferred upstream setup

Official OP-TEE still uses Google `repo` + the [optee_os manifest](https://github.com/OP-TEE/manifest):

```bash
mkdir -p ~/optee && cd ~/optee
repo init -u https://github.com/OP-TEE/manifest.git -m qemu_v8.xml
repo sync
cd build && make -j$(nproc) toolchains && make -j$(nproc) run
```

## Build (this tree)

```bash
cd build
make -j$(nproc) toolchains   # downloads into ../toolchains (gitignored)
make -j$(nproc) run
```

Build artifacts (`out/`, `out-br/`, `toolchains/`) are intentionally not tracked.
