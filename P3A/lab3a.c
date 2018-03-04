// NAME: Quentin Truong
// EMAIL: quentintruong@gmail.com
// ID: 404782322

#include <errno.h>          // number of last error
#include <string.h>         
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "ext2_fs.h"


// Statics
static int img_fd = -1;

// Process cmd line args
void process_args(int argc, char** argv){
    if (argc != 2){
        fprintf(stderr, "Error: wrong number of arguments\n");
        exit(1);
    }
    else{
        img_fd = open(argv[1], O_RDONLY);
        if (img_fd < 0){
            fprintf(stderr, "Error: open failed\n");
            exit(2);
        }
    }
}

// Exit handler
void exit_handler(){
    close(img_fd);
}

/*
    SUPERBLOCK
    total number of blocks (decimal)
    total number of i-nodes (decimal)
    block size (in bytes, decimal)
    i-node size (in bytes, decimal)
    blocks per group (decimal)
    i-nodes per group (decimal)
    first non-reserved i-node (decimal)
*/
void summarize_superblock(struct ext2_super_block* super_block){
    fprintf(stdout, "%s,%u,%u,%u,%u,%u,%u,%u\n", 
        "SUPERBLOCK",                       super_block->s_blocks_count, 
        super_block->s_inodes_count,        EXT2_MIN_BLOCK_SIZE << super_block->s_log_block_size, 
        super_block->s_inode_size,          super_block->s_blocks_per_group, 
        super_block->s_inodes_per_group,    super_block->s_first_ino);
}

/*
    GROUP
    group number (decimal, starting from zero)
    total number of blocks in this group (decimal)
    total number of i-nodes in this group (decimal)
    number of free blocks (decimal)
    number of free i-nodes (decimal)
    block number of free block bitmap for this group (decimal)
    block number of free i-node bitmap for this group (decimal)
    block number of first block of i-nodes in this group (decimal)
*/
void summarize_group(struct ext2_super_block* super_block, struct ext2_group_desc* group_desc){
    fprintf(stdout, "%s,%u,%u,%u,%u,%u,%u,%u,%u\n", 
        "GROUP", 0,
        super_block->s_blocks_count, super_block->s_inodes_count,
        group_desc->bg_free_blocks_count, group_desc->bg_free_inodes_count, 
        group_desc->bg_block_bitmap, group_desc->bg_inode_bitmap, 
        group_desc->bg_inode_table);
}

/*
Scan the free block bitmap for each group. For each free block, produce a new-line terminated line, with two comma-separated fields (with no white space).
    BFREE
    number of the free block (decimal)
*/
void summarize_free_block_entries(struct ext2_super_block* super_block, struct ext2_group_desc* group_desc){
    // Useful variables
    __u32 bitmap_block = group_desc->bg_block_bitmap;
    __u32 block_count = super_block->s_blocks_count;
    __u32 block_size = EXT2_MIN_BLOCK_SIZE << super_block->s_log_block_size;

    // Malloc and pread
    uint8_t *bitmap = malloc(block_size);
    if ((pread(img_fd, bitmap, block_size, bitmap_block * block_size)) == -1) {
        fprintf(stderr, "Error: pread failed\n");
        exit(2);
    }

    // Print out
    for (__u32 i = 0; i < block_count; i++){
        uint8_t byte = bitmap[i / 8];
        uint8_t mask = 1 << (i % 8);
        if ((byte & mask) == 0)
            fprintf(stdout, "%s,%u\n", "BFREE", i + 1);
    }

    free(bitmap);
}

/*
Scan the free I-node bitmap for each group. For each free I-node, produce a new-line terminated line, with two comma-separated fields (with no white space).
    IFREE
    number of the free I-node (decimal)
*/
void summarize_free_inode_entries(struct ext2_super_block* super_block, struct ext2_group_desc* group_desc){
    // Useful variables
    __u32 bitmap_inode = group_desc->bg_inode_bitmap;
    __u32 inode_count = super_block->s_inodes_count;
    __u32 block_size = EXT2_MIN_BLOCK_SIZE << super_block->s_log_block_size;

    // Malloc and pread
    uint8_t *bitmap = malloc(block_size);
    if ((pread(img_fd, bitmap, block_size, bitmap_inode * block_size)) == -1) {
        fprintf(stderr, "Error: pread failed\n");
        exit(2);
    }

    // Print out
    for (__u32 i = 0; i < inode_count; i++){
        uint8_t byte = bitmap[i / 8];
        uint8_t mask = 1 << (i % 8);
        if ((byte & mask) == 0)
            fprintf(stdout, "%s,%u\n", "IFREE", i + 1);
    }

    free(bitmap);
}

/*
    INODE
    inode number (decimal)
    file type ('f' for file, 'd' for directory, 's' for symbolic link, '?" for anything else)
    mode (low order 12-bits, octal ... suggested format "0%o")
    owner (decimal)
    group (decimal)
    link count (decimal)
    time of last I-node change (mm/dd/yy hh:mm:ss, GMT)
    modification time (mm/dd/yy hh:mm:ss, GMT)
    time of last access (mm/dd/yy hh:mm:ss, GMT)
    file size (decimal)
    number of blocks (decimal)
    The next fifteen fields are block addresses (decimal, 12 direct, one indirect, one double indirect, one triple indirect).
*/
void summarize_inode(struct ext2_super_block* super_block, struct ext2_group_desc* group_desc){
    // Useful Variables
    __u32 inode_table = group_desc->bg_inode_table;
    __u32 inode_count = super_block->s_inodes_count;
    __u32 block_size = EXT2_MIN_BLOCK_SIZE << super_block->s_log_block_size;


    for (__u32 i = 0; i < inode_count; i++){
        // Malloc and pread
        struct ext2_inode* inode = malloc(sizeof(struct ext2_inode));
        if ((pread(img_fd, inode, sizeof(struct ext2_inode), inode_table * block_size + i * sizeof(struct ext2_inode))) == -1) {
            fprintf(stderr, "Error: pread failed\n");
            exit(2);
        }

        // Ignore if not truly allocated
        if (inode->i_mode == 0 || inode->i_links_count == 0)
            continue;

        // i_mode -> readable char
        char file_type = '?';
        if (inode->i_mode & 0x8000)
            file_type = 'f';
        else if (inode->i_mode & 0x4000)
            file_type = 'd';
        else if (inode->i_mode & 0xA000)
            file_type = 's';

        // i_xtime -> readable time
        time_t i_ctime = inode->i_ctime;
        time_t i_mtime = inode->i_mtime;
        time_t i_atime = inode->i_atime;
        struct tm *gmt_c, *gmt_m, *gmt_a;
        char ctime[18], mtime[18], atime[18];
        gmt_c = gmtime(&i_ctime);
        strftime(ctime, sizeof(ctime), "%m/%d/%y %H:%M:%S", gmt_c);
        gmt_m = gmtime(&i_mtime);
        strftime(mtime, sizeof(mtime), "%m/%d/%y %H:%M:%S", gmt_m);
        gmt_a = gmtime(&i_atime);
        strftime(atime, sizeof(atime), "%m/%d/%y %H:%M:%S", gmt_a);

        // Printout
        fprintf(stdout, "%s,%d,%c,%o,%u,%u,%u,%s,%s,%s,%u,%u", 
            "INODE", i + 1, 
            file_type, inode->i_mode & 0x0FFF,
            inode->i_uid, inode->i_gid,
            inode->i_links_count, ctime,
            mtime, atime,
            inode->i_size, inode->i_blocks);
        for (int b = 0 ; b < EXT2_N_BLOCKS; b++)
            fprintf(stdout, ",%u", inode->i_block[b]);
        fprintf(stdout, "\n");

        free(inode);
    }
}


int main(int argc, char** argv){
    process_args(argc, argv);

    atexit(exit_handler);

    // Load superblock
    struct ext2_super_block* super_block = malloc(sizeof(struct ext2_super_block));
    if (pread(img_fd, super_block, sizeof(struct ext2_super_block), EXT2_MIN_BLOCK_SIZE) == -1){
        fprintf(stderr, "Error: pread failed\n");
        exit(2);
    }

    // Load the only group_desc
    struct ext2_group_desc* group_desc = malloc(sizeof(struct ext2_group_desc));
    if (pread(img_fd, group_desc, sizeof(struct ext2_group_desc), EXT2_MIN_BLOCK_SIZE * 2) == -1){
        fprintf(stderr, "Error: pread failed\n");
        exit(2);
    }
    
    // Summarize
    summarize_superblock(super_block);
    summarize_group(super_block, group_desc);
    summarize_free_block_entries(super_block, group_desc);
    summarize_free_inode_entries(super_block, group_desc);
    summarize_inode(super_block, group_desc);

    free(group_desc);
    free(super_block);

    exit(0);
}