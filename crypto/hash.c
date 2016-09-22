// Copyright (c) 2012-2013 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "hash-ops.h"

void cn_fast_hash(const void *data, int len, char *hash) {
  union hash_state state;
  hash_process(&state, data, len);
  memcpy(hash, &state, HASH_SIZE);
}
