/*
 * Copyright (c) 2003 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/*	$NetBSD: ntfs_inode.h,v 1.8 1999/10/31 19:45:26 jdolecek Exp $	*/

/*-
 * Copyright (c) 1998, 1999 Semen Ustimenko
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: src/sys/fs/ntfs/ntfs_inode.h,v 1.10 2001/11/26 23:45:12 jhb Exp $
 */

/* These flags are kept in i_flag. */
#define	IN_ACCESS	0x0001	/* Access time update request. */
#define	IN_CHANGE	0x0002	/* Inode change time update request. */
#define	IN_UPDATE	0x0004	/* Modification time update request. */
#define	IN_MODIFIED	0x0008	/* Inode has been modified. */
#define	IN_RENAME	0x0010	/* Inode is being renamed. */
#define	IN_SHLOCK	0x0020	/* File has shared lock. */
#define	IN_EXLOCK	0x0040	/* File has exclusive lock. */
#define	IN_LAZYMOD	0x0080	/* Modified, but don't write yet. */
#define	IN_HASHED	0x0800	/* Inode is on hash list */
#define IN_ALLOC	0x1000	/* inode is being allocated and initialized */
#define IN_WALLOC	0x2000	/* someone is waiting for alloc/init to finish */
#define	IN_PRELOADED	0x4000	/* loaded from directory entry */
#define	IN_LOADED	0x8000	/* ntvattrs loaded */

struct ntnode {
	lck_attr_t		*i_lock_attr;
	lck_mtx_t		*i_lock;

	vnode_t			i_devvp;	/* vnode of blk dev we live on */
	dev_t           i_dev;		/* Device associated with the inode. */

	LIST_ENTRY(ntnode)	i_hash;
	struct ntfsmount       *i_mp;
	ino_t           i_number;
	u_int32_t       i_flag;

	/* locking */
	int		i_usecount;

	LIST_HEAD(,fnode)	i_fnlist;
	LIST_HEAD(,ntvattr)	i_valist;

	long		i_nlink;	/* MFR */
	ino_t		i_mainrec;	/* MFR */
	u_int32_t	i_frflag;	/* MFR */
};

#define	FN_PRELOADED	0x0001
#define	FN_VALID	0x0002
#define	FN_AATTRNAME	0x0004	/* space allocated for f_attrname */
#define FN_NONRESIDENT	0x0008	/* attribute is non-resident (uses run list) */

struct fnode {
	LIST_ENTRY(fnode) f_fnlist;
	vnode_t			f_vp;		/* Associatied vnode */
	struct ntnode  *f_ip;		/* Associated ntnode */
	u_long		f_flag;

	ntfs_times_t	f_times;	/* $NAME/dirinfo */
	ino_t		f_pnumber;	/* $NAME/dirinfo */
	u_int32_t       f_fflag;	/* $NAME/dirinfo */
	u_int64_t	f_size;		/* defattr/dirinfo: */
	u_int64_t	f_allocated;	/* defattr/dirinfo */

	u_int32_t	f_attrtype;
	char	       *f_attrname;

	/* for ntreaddir */
	u_int32_t       f_lastdattr;
	u_int32_t       f_lastdblnum;
	u_int32_t       f_lastdoff;
	u_int32_t       f_lastdnum;
	caddr_t         f_dirblbuf;
	u_int32_t       f_dirblsz;
	
	size_t			f_compsize;		/* Bytes per compression unit; 0 if not compressed */
};

/* This overlays the fid structure (see <sys/mount.h>) */
struct ntfid {
        u_int16_t ntfid_len;     /* Length of structure. */
        u_int16_t ntfid_pad;     /* Force 32-bit alignment. */
        ino_t     ntfid_ino;     /* File number (ino). */
        int32_t   ntfid_gen;     /* Generation number. */
};