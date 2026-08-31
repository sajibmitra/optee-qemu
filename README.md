# OP-TEE QEMU (AArch64)

Pinned OP-TEE QEMU v8 workspace (`qemu_v8` manifest) as Git submodules: OP-TEE OS, Linux, TF-A, QEMU, Buildroot, and related components for developing and testing Trusted Applications on an emulated Armv8-A platform.

Official docs: [optee.readthedocs.io](https://optee.readthedocs.io/) · [QEMU guide](https://optee.readthedocs.io/en/latest/building/devices/qemu.html)

---

## Prerequisites

| Requirement | Notes |
|---|---|
| Host OS | Linux (x86_64 or aarch64 recommended) |
| Disk | ~20–40 GB free after toolchains + full build |
| RAM | ≥8 GB recommended for parallel builds |
| Packages | `git`, `make`, `python3`, `python3-pyelftools`, `cmake`, `ninja-build`, `pkg-config`, `libssl-dev`, `libglib2.0-dev`, `libpixman-1-dev`, `flex`, `bison`, `wget`, `cpio`, `unzip`, `rsync`, `bc`, `device-tree-compiler`, `acpica-tools`, `uuid-dev` |
| Terminal | One of `tmux`, `gnome-terminal`, `konsole`, or `xterm` (needed by `make run`) |

Example on Debian/Ubuntu:

```bash
sudo apt update
sudo apt install -y git make python3 python3-pyelftools cmake ninja-build \
  pkg-config libssl-dev libglib2.0-dev libpixman-1-dev flex bison wget \
  cpio unzip rsync bc device-tree-compiler acpica-tools uuid-dev \
  libncurses-dev xz-utils
```

---

## 1. Clone

```bash
git clone --recurse-submodules https://github.com/sajibmitra/optee-qemu.git
cd optee-qemu
```

If you already cloned without submodules:

```bash
git submodule update --init --recursive
```

Create the platform Makefile (normally created by Google `repo`; not stored in the `build` submodule):

```bash
ln -sf qemu_v8.mk build/Makefile
```

---

## 2. Download toolchains (once)

From the repo root:

```bash
cd build
make -j"$(nproc)" toolchains
```

This downloads Arm GNU toolchains (and Rust on x86_64 hosts) into `../toolchains/` (gitignored). Re-run only if you wipe that directory.

---

## 3. Build and boot QEMU

Still in `build/`:

```bash
make -j"$(nproc)" run
```

What happens:

1. Builds TF-A, OP-TEE OS, Linux, Buildroot rootfs, and QEMU.
2. Opens **Normal World** and **Secure World** UART terminals.
3. Leaves you at the QEMU monitor prompt.

Continue boot in the QEMU console:

```text
(qemu) c
```

First full build can take a long time. Later rebuilds are incremental.

### Login

On the **Normal World** terminal, log in as `root` (no password by default on the Buildroot image).

---

## 4. Run OP-TEE tests

In the **Normal World** shell:

```bash
xtest          # full regression suite
xtest -h       # help / list options
xtest 1001     # run a single test case
```

Secure World logs appear in the Secure World terminal (`CFG_TEE_CORE_LOG_LEVEL` defaults to `3`).

### Automated check (host)

From `build/` (builds if needed, then runs xtest non-interactively):

```bash
make -j"$(nproc)" check
```

---

## 5. Day-to-day workflow

| Goal | Command (from `build/`) |
|---|---|
| Rebuild everything that changed, then boot | `make -j$(nproc) run` |
| Boot without rebuilding | `make run-only` |
| Rebuild only OP-TEE OS | `make optee-os` |
| Rebuild only Linux | `make linux` |
| Rebuild only TF-A | `make arm-tf` |
| Rebuild only Buildroot / rootfs | `make buildroot` |
| Clean one component | `make optee-os-clean` (also `linux-clean`, `arm-tf-clean`, …) |
| Clean Buildroot | `make buildroot-clean` |

After editing Trusted Applications or client apps under `optee_examples/` / `optee_test/`, rebuild Buildroot (or the relevant package) so the rootfs picks up the new binaries, then `make run`.

### Useful knobs

```bash
# Share the workspace into the guest at /mnt/host
make QEMU_VIRTFS_AUTOMOUNT=y run

# User networking is on by default (SLiRP). VirtFS + usernet:
make QEMU_VIRTFS_AUTOMOUNT=y QEMU_USERNET_ENABLE=y run

# Quiet TEE core logs
make CFG_TEE_CORE_LOG_LEVEL=1 run

# Disable TEE ASLR for GDB (secure world)
make CFG_CORE_ASLR=n run
```

Mount a host folder manually if you enabled VirtFS without automount:

```bash
mkdir -p /mnt/host
mount -t 9p -o trans=virtio host /mnt/host
```

---

## 6. Repository layout

| Path | Role |
|---|---|
| `build/` | Top-level makefiles (`qemu_v8.mk`) — start here |
| `optee_os/` | Trusted Execution Environment (secure OS) |
| `optee_client/` | Normal-world TEE client library / `tee-supplicant` |
| `optee_test/` | `xtest` and related TAs |
| `optee_examples/` | Sample client apps + TAs |
| `linux/` | Normal-world kernel |
| `trusted-firmware-a/` | TF-A (BL1/BL2/BL31, SPMD) |
| `qemu/` | Emulator |
| `buildroot/` | Root filesystem |
| `u-boot/`, `hafnium/`, `mbedtls/`, … | Platform / feature dependencies |
| `toolchains/` | Downloaded cross toolchains (**not** in git) |
| `out/` | Firmware / boot binaries (**not** in git) |
| `out-br/` | Buildroot output (**not** in git) |

---

## 7. Developing a Trusted Application (quick path)

1. Add or edit sources under `optee_examples/` (or your own tree).
2. Ensure the example is enabled in Buildroot / OP-TEE packages for this platform.
3. Rebuild and boot:

```bash
cd build
make -j"$(nproc)" buildroot
make run-only   # or: make run
```

4. In Normal World, run your CA (client application). Secure World traces show up on the Secure World UART.

For TA/CA packaging details, see [OP-TEE Trusted Applications](https://optee.readthedocs.io/en/latest/building/trusted_applications.html).

---

## 8. Debugging tips

**Normal-world GDB** (example): build with `GDBSERVER=y`, boot, then in the guest:

```bash
gdbserver :12345 xtest 4002
```

On the host, use the Buildroot cross-gdb under `out-br/host/bin/` and `target remote :12345`.

**Secure-world GDB**: start with `make run CFG_CORE_ASLR=n`, connect to QEMU’s GDB stub (`localhost:1234`), then:

```text
(gdb) symbol-file ../optee_os/out/arm/core/tee.elf
(gdb) b tee_entry_std
(gdb) c
```

Use the aarch64 toolchain GDB from `toolchains/aarch64/bin/`.

---

## 9. Upstream `repo` workflow (optional)

If you prefer the official Google `repo` checkout instead of this submodule mirror:

```bash
mkdir -p ~/optee && cd ~/optee
repo init -u https://github.com/OP-TEE/manifest.git -m qemu_v8.xml
repo sync -j"$(nproc)"
cd build
make -j"$(nproc)" toolchains
make -j"$(nproc)" run
```

---

## What is not tracked in git

`out/`, `out-br/`, `toolchains/`, and `.repo/` are build/download artifacts and are listed in `.gitignore`. Do not commit them — they exceed GitHub size limits.
