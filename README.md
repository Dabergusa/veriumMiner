veriumMiner
==============

This is a multi-threaded CPU miner for Verium using scrypt²,
fork of [tpruvot](//github.com/tpruvot)'s cpuminer-multi (see AUTHORS for list of contributors).

#### Table of contents

* [Dependencies](#dependencies)
* [Download](#download)
* [Build](#build)
* [Usage instructions](#usage-instructions)
* [Donations](#donations)
* [GCC 7.2](#gcc)
* [Credits](#credits)
* [License](#license)


Dependencies
============
 * libcurl http://curl.haxx.se/libcurl/
 * jansson http://www.digip.org/jansson/ (jansson source is included in-tree)
 * openssl libcrypto https://www.openssl.org/
 * pthreads
 * zlib (for curl/ssl)

Download
========
 * Git tree:   https://github.com/fireworm71/veriumMiner
 * Clone with `git clone https://github.com/fireworm71/veriumMiner`

Build
=====

#### Basic *nix build instructions:
 * just use ./build.sh
_OR_
 * ./autogen.sh	# only needed if building from git repo
 * ./nomacro.pl	# only needed if building on Mac OS X or with Clang
 * ./configure CFLAGS="*-march=native*" --with-crypto --with-curl
   * # Use -march=native if building for a single machine
 * make

#### Note for Debian/Ubuntu users:
 * `apt-get install automake autoconf pkg-config libcurl4-openssl-dev libjansson-dev libssl-dev libgmp-dev zlib1g-dev`

#### Notes for FreeBSD users:
 * Install Dependencies: 
`pkg install automake autoconf git`

 * Build libcurl by compiling and installing the curl port
`cd /usr/ports/ftp/curl; make install`

 * Now checkout and build the cpuminer binary
```
   git clone https://github.com/fireworm71/veriumMiner.git
   cd veriumMiner
   export C_INCLUDE_PATH=$INCLUDE_PATH:/usr/local/include
   export CPLUS_INCLUDE_PATH=$INCLUDE_PATH:/usr/local/include
   export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/lib
   ./autogen.sh
   /usr/local/bin/perl ./nomacro.pl
   ./configure CFLAGS="-march=native" --with-crypto --with-curl
   make
```

#### Notes for AIX users:
 * To build a 64-bit binary, export OBJECT_MODE=64
 * GNU-style long options are not supported, but are accessible via configuration file

#### Basic Windows build with Visual Studio 2013
 * All the required .lib files are now included in tree (windows only)
 * AVX enabled by default for x64 platform (AVX2 and XOP could also be used)

#### Basic Windows build instructions, using MinGW64:
 * Install MinGW64 and the MSYS Developer Tool Kit (http://www.mingw.org/)
   * Make sure you have mstcpip.h in MinGW\include
 * install pthreads-w64
 * Install libcurl devel (http://curl.haxx.se/download.html)
   * Make sure you have libcurl.m4 in MinGW\share\aclocal
   * Make sure you have curl-config in MinGW\bin
 * Install openssl devel (https://www.openssl.org/related/binaries.html)
 * In the MSYS shell, run:
   * for 64bit, you can use ./mingw64.sh else :
     ./autogen.sh	# only needed if building from git repo
   * LIBCURL="-lcurldll" ./configure CFLAGS="*-march=native*"
     * # Use -march=native if building for a single machine
   * make

#### Architecture-specific notes:
 * ARMv8:
   * Neon is enabled by default, ./build.sh should work fine.
 * ARMv7:
   * No runtime CPU detection. The miner can take advantage of some instructions specific to ARMv5E and later processors, but the decision whether to use them is made at compile time, based on compiler-defined macros.
   * To use NEON instructions, add "-mfpu=neon" to CFLAGS.
 * x86:
   * The miner checks for SSE2 instructions support at runtime, and uses them if they are available.
 * x86-64:	
   * The miner can take advantage of AVX, AVX2 and XOP instructions, but only if both the CPU and the operating system support them.
     * Linux supports AVX starting from kernel version 2.6.30.
     * FreeBSD supports AVX starting with 9.1-RELEASE.
     * Mac OS X added AVX support in the 10.6.8 update.
     * Windows supports AVX starting from Windows 7 SP1 and Windows Server 2008 R2 SP1.
   * The configure script outputs a warning if the assembler doesn't support some instruction sets. In that case, the miner can still be built, but unavailable optimizations are left off.

Usage instructions
==================
Run "cpuminer --help" to see options.

### HugePages (Linux only)
Linux provides a nice optimization (+10% - +50% gains) that enables faster memory lookups.  These are 'HugePages'.  HugePages preallocate a bunch of memory for 'a specific use', in this case, for the miner.  

To enable them (on Ubuntu 16.04), first check that you have `/proc/sys/vm/nr_hugepages` by doing `ls /proc/sys/vm` (you should see `nr_hugepages` in the print out).  

You can allocate huge pages by doing one of two things:
1) `sudo nano /etc/sysctl.conf`, scroll to the bottom, and type in `vm.nr_hugepages=size`, then `Ctrl+O`, then `[Enter]`, then `Ctrl+X`.
2) `echo "vm.nr_hugepages=size" > sudo tee --append /etc/sysctl.conf`.

After either, do `sudo sysctl -p`.  You should see `vm.nr_hugepages=size` print out on the console.  If not, check your distro.  You may need to recompile your kernel to enable this.  You can also verify that memory is allocated by running `free` and seeing that you now have a ton of memory allocated, but aren't running anything that's using it.

`size` = (the amount of memory each miner thread needs) / (2048 * 1024).  

How much memory is be used per thread?
* 1way : 128MB -> nr_hugepages = 65.
* SSE4 (3way) : 384MB -> nr_hugepages = 193.
* AVX  (3way) : 384MB -> nr_hugepages = 193.
* AVX2 (6way) : 768MB -> nr_hugepages = 386.
* ARMv7 (3way) : 384MB -> nr_hugepages = 193.
* ARMv8 (3way) : 384MB -> nr_hugepages = 193.

For example, 4 threads on an SSE4, you'd type `echo "vm.nr_hugepages=772" > sudo tee --append /etc/sysctl.conf`

Multiply that number by the number of threads, and you will have the size needed.  Note, you may not have enough RAM for this on ARM SoCs.  The miner should still work, but it will not be as optimal. 

How can you tell you have enough HugePages?
You will not see `HugePages unavailable (##)` print out.  That means you have successfully allocated all needed HugePages for the miner.

If for some reason you want to remove HugePages (or adjust the size):

`sudo nano /etc/sysctl.conf`, scroll to the bottom, and remove / edit the line `vm.nr_hugepages=size`, `Ctrl+O`, `[Enter]`, `Ctrl+X`.  Then, like before, `sudo sysctl -p`.  Note that you can also reboot and this will cause HugePages to allocate / deallocate.

### Oneways, cpu affinity.

You can now specify a number of 'oneway' threads to acompany your 'default way' threads.  Default way is determined by your CPU instruction set.

'-1 n' will specify the number of oneway threads to spawn.  You can also use `--oneways n`.  Some folks (on arm especially) see perf gains due to the implementation.  Additionally, there are new options to help control affinity for these threads and 'default' way threads too.  `--cpu-affinity-stride N`, `--cpu-affinity-default-index N`, `--cpu-affinity-oneway-index N`, and `--cpu-priority-oneway 0-5`

Affinity stride works by saying how many cpus should be skipped before assigning the thread to it.  So, if you have 8 cpu's and set 'stride' to 3, you will start with CPU (0 + 3 * 0), then CPU (0 + 3 * 1), then CPU (0 + 3 * 2),  aka  CPU's 0, 3, 6.  'default' index is the starting index ('0' in the example) for the number of `-t` threads.  'oneway-index' is the starting index for oneway `-1` threads.

Play around with them, and pass `-D` to get some debug output.

e.g, 'Bind oneways to odd threads, 'defaults' to even threads: `./cpuminer ... -t 4 -1 4 --cpu-affinity-stride 2 --cpu-affinity-default-index 0 --cpu-affinity-oneway-index 1`
e.g.2, 'Bind oneways to the last 4 cpu's after the defaults' `./cpuminer ... -t 4 -1 4 --cpu-affinity-stride 1 --cpu-affinity-default-index 0 --cpu-affinity-oneway-index 4` (edited)

# RYZEN USERS pass `--ryzen`.

Ryzen's implementation of AVX2 is ... subpar.  Please pass `--ryzen` on the commandline to default to the AVX implementation.  Users reported ~25% gains.

### Connecting through a proxy

Use the --proxy option.

To use a SOCKS proxy, add a socks4:// or socks5:// prefix to the proxy host  
Protocols socks4a and socks5h, allowing remote name resolving, are also available since libcurl 7.18.0.

If no protocol is specified, the proxy is assumed to be a HTTP proxy.  
When the --proxy option is not used, the program honors the http_proxy and all_proxy environment variables.

GCC
=======
Some people have reported increases by using GCC 7.2.  Please note, this will replace your existing GCC installation, which may become unrecoverable if any errors occur in the `make` process.

To build and install GCC 7.2 on Ubuntu do the following:
* `apt-get -y install unzip flex`
* `wget https://github.com/gcc-mirror/gcc/archive/gcc-7_2_0-release.zip`
* `unzip gcc-7_2_0-release.zip`
* `cd gcc-gcc-7_2_0-release`
* `sudo ./contrib/download_prerequisites`
* `mkdir build && cd build`
* `../configure --enable-languages=c,c++ --disable-multilib`
* `make -j 8`
* `make install`


Credits
=======
CPUMiner-multi was forked from pooler's CPUMiner, and has been started by Lucas Jones.
* [tpruvot](https://github.com/tpruvot) added all the recent features and newer algorythmns
* [Wolf9466](https://github.com/wolf9466) helped with Intel AES-NI support for CryptoNight
* [FireWorm71](https://github.com/Fireworm71) helped with ARMv8 implementation, and memory optimizations.

Donations
=======
This miner does not include any automatic donation code.  Please donate if you find this code profitable.  A suggestion is about 3.5 days of mining time per year (1%).  If you prefer to mine on my behalf, please run your miner with the miner args below.
* FireWorm71:
  * Mining Donations: `./cpuminer -o stratum+tcp://stratum.poolsloth.com:3333 -u fireworm.donations -p x`
  * BTC: 1PPw16NEwMbGmobCcEyy9XhU5Uy7LTsRrZ
  * VRM: VB99zk5BGzAFsQvrrQeBBnZNGZYgmRfmSy
  * VRC: VYy4v95EfkJy7QnPSwfK7fhgWPM4RujbPf

License
=======
GPLv2.  See COPYING for details.
