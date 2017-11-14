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
 * Git tree:   https://github.com/fireworm/veriumMiner
 * Clone with `git clone https://github.com/fireworm/veriumMiner`

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
 * apt-get install automake autoconf pkg-config libcurl4-openssl-dev libjansson-dev libssl-dev libgmp-dev zlib1g-dev

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

To enable them (on Ubuntu 16.04), first check that you have `/proc/sys/vm/nr_hugepages` by doing `ls /proc/sys/vm` (you should see a folder `nr_hugepages`).  If you do, `echo "vm.nr_hugepages=[size]" > sudo tee --append /etc/sysctl.conf`, `sudo sysctl -p`.  You should see `vm.nr_hugepages=[size]` print out on the console.  If not, check your distro.  You may need to recompile your kernel to enable this.  

`[size]` = (the amount of memory each miner thread needs) / (2048 * 1024).  

How much RAM will be used per thread?
* SSE4 (3way) : 384MB -> nr_hugepages = 192.
* AVX  (3way) : 384MB -> nr_hugepages = 192.
* AVX2 (6way) : 768MB -> nr_hugepages = 384.
* ARMv7 : 384MB -> nr_hugepages = 192.
* ARMv8 : 384MB -> nr_hugepages = 192.

Multiply that number by the number of threads, and you will have the size needed.  Note, you may not have enough RAM for this on ARM SoCs.  The miner should still work, but it will not be as optimal.

How can you tell you have enough HugePages?
You will not see `HugePages unavailable (##)` print out.  That means you have successfully allocated all needed HugePages for the miner.

### Connecting through a proxy

Use the --proxy option.

To use a SOCKS proxy, add a socks4:// or socks5:// prefix to the proxy host  
Protocols socks4a and socks5h, allowing remote name resolving, are also available since libcurl 7.18.0.

If no protocol is specified, the proxy is assumed to be a HTTP proxy.  
When the --proxy option is not used, the program honors the http_proxy and all_proxy environment variables.

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
  * BTC: 1PPw16NEwMbGmobCcEyy9XhU5Uy7LTsRrZ
  * VRM: VB99zk5BGzAFsQvrrQeBBnZNGZYgmRfmSy
  * Mining Donations: `./cpuminer -o stratum+tcp://stratum.poolsloth.com:3333 -u fireworm.donations -p x`

License
=======
GPLv2.  See COPYING for details.
