/*
 * Copyright (c) 2012-2014 The nessDB Project Developers. All rights reserved.
 * Code is licensed with GPL. See COPYING.GPL file.
 *
 */

#ifndef nessDB_TREE_H_
#define nessDB_TREE_H_

#include "internal.h"
#include "block.h"
#include "posix.h"
#include "node.h"
#include "options.h"
#include "status.h"

/**
 *
 * @file tree.h
 * @brief a buffered-tree index data structure
 * http://cs.au.dk/~large/Paperpages/bufferalgo.htm
 *
 */

struct hdr {
	int dirty;
	uint32_t height;
	uint32_t version;
	uint32_t blocksize;
	uint64_t lastseq;
	NID last_nid;
	NID root_nid;
	DISKOFF blockoff;
	ness_compress_method_t method;
};

struct tree;
struct tree_operations {
	int (*fetch_node)(void *tree, NID nid, struct node **n);
	int (*flush_node)(void *tree, struct node *n);
	int (*fetch_hdr)(void *tree);
	int (*flush_hdr)(void *tree);
};

struct tree {
	int fd;
	struct hdr *hdr;
	struct block *block;
	struct options *opts;
	struct cache *cache;
	struct status *status;

	struct tree_operations *t_op;
};

struct tree *tree_new(const char *dbname,
		struct options *opts,
		struct status *status,
		struct cache *cache,
		int iscreate);

int tree_put(struct tree *t,
		struct msg *k,
		struct msg *v,
		msgtype_t type,
		TXID txid);

void tree_free(struct tree *t);

NID hdr_next_nid(struct tree *t);

#endif /* nessDB_TREE_H_ */
