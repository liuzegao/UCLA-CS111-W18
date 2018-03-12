#!/usr/local/cs/bin/python3

# NAME: Quentin Truong
# EMAIL: quentintruong@gmail.com
# ID: 404782322


import sys

def read_process_csv(csv_content):
    csv_dict = {"SUPERBLOCK": [],
                "GROUP": [],
                "BFREE": [],
                "IFREE": [],
                "DIRENT": [],
                "INODE": [],
                "INDIRECT": []}

    csv_content = [line.strip() for line in csv_content]

    for line in csv_content:
        split_line = line.split(",")
        csv_dict[split_line[0]].append(split_line)

    return csv_dict

def get_data_block_start(csv_file):

    pass

def check_blocks(csv_dict):
    block_first_inode = int(csv_dict["GROUP"][0][8])
    inodes_per_group = int(csv_dict["GROUP"][0][3])
    inode_size = int(csv_dict["SUPERBLOCK"][0][4])
    block_size = int(csv_dict["SUPERBLOCK"][0][3])

    data_block_start_num = int(block_first_inode + inodes_per_group * inode_size / block_size)
    data_block_end_num = int(csv_dict["GROUP"][0][2])

    data_block_referenced = [0] * (data_block_end_num)
    data_block_location = [[] for i in range(data_block_end_num)] 

    for inode in csv_dict["INODE"]:
        for index in range(12, 27):
            block_type = "BLOCK"
            if index == 24:
                block_type = "INDIRECT BLOCK"
            elif index == 25:
                block_type = "DOUBLE INDIRECT BLOCK"
            elif index == 26:
                block_type = "TRIPLE INDIRECT BLOCK"

            offset = index - 12
            if index == 25:
                offset = 12 + 256
            if index == 26:
                offset = 12 + 256 + 256 * 256


            if int(inode[index]) < 0 or int(inode[index]) > data_block_end_num:
                print("INVALID {0} {1} IN INODE {2} AT OFFSET {3}".format(block_type, inode[index], inode[1], offset))
            elif int(inode[index]) < data_block_start_num and int(inode[index]) > 0:
                print("RESERVED {0} {1} IN INODE {2} AT OFFSET {3}".format(block_type, inode[index], inode[1], offset))
            else:
                data_block_referenced[int(inode[index])] += 1
                data_block_location[int(inode[index])].append("DUPLICATE {0} {1} IN INODE {2} AT OFFSET {3}".format(block_type, inode[index], inode[1], offset))

    for indirect in csv_dict["INDIRECT"]:
        data_block_referenced[int(indirect[5])] += 1

    for bfree in csv_dict["BFREE"]:
        if data_block_referenced[int(bfree[1])] != 0:
            print("ALLOCATED BLOCK {0} ON FREELIST".format(bfree[1]))
        data_block_referenced[int(bfree[1])] += 1

    for block_num in range(data_block_start_num, data_block_end_num):
        if data_block_referenced[block_num] == 0:
            print("UNREFERENCED BLOCK {0}".format(block_num))

        if len(data_block_location[block_num]) > 1:
            for line in data_block_location[block_num]:
                print(line)


def check_inodes(csv_dict):
    inode_start_num = int(csv_dict["SUPERBLOCK"][0][7])
    inode_end_num = int(csv_dict["GROUP"][0][3])

    inode_allocated = [0] * (inode_end_num + 1)

    for inode in csv_dict["INODE"]:
        inode_allocated[int(inode[1])] = 1

    for ifree in csv_dict["IFREE"]:
        if inode_allocated[int(ifree[1])] == 1:
            print("ALLOCATED INODE {0} ON FREELIST".format(int(ifree[1])))
        elif inode_allocated[int(ifree[1])] == 0:
            inode_allocated[int(ifree[1])] = 1

    for inode_num in range(inode_start_num, inode_end_num + 1):
        if inode_allocated[inode_num] == 0:
            print("UNALLOCATED INODE {0} NOT ON FREELIST".format(inode_num))

def check_directories(csv_dict):
    inode_start_num = int(csv_dict["SUPERBLOCK"][0][7])
    inode_end_num = int(csv_dict["GROUP"][0][3])

    inode_linkcount = [0] * (inode_end_num + 1)
    inode_alloc = [0] * (inode_end_num + 1)
    inode_parent = [0] * (inode_end_num + 1)

    for inode in csv_dict["INODE"]:
        inode_alloc[int(inode[1])] += 1

    for dirent in csv_dict["DIRENT"]:
        if int(dirent[3]) < 1 or int(dirent[3]) > inode_end_num:
            print("DIRECTORY INODE {0} NAME {1} INVALID INODE {2}".format(dirent[1], dirent[6], dirent[3]))
        elif inode_alloc[int(dirent[3])] == 0:
            print("DIRECTORY INODE {0} NAME {1} UNALLOCATED INODE {2}".format(dirent[1], dirent[6], dirent[3]))
        else:
            inode_linkcount[int(dirent[3])] += 1

            if dirent[6] == "'.'" and dirent[1] != dirent[3]: # may not be strictly correct in all cases
                print("DIRECTORY INODE {0} NAME '.' LINK TO INODE {1} SHOULD BE {0}".format(dirent[1], dirent[3]))

            if dirent[6] != "'.'" and dirent[6] != "'..'":
                inode_parent[int(dirent[3])] = int(dirent[1])


    for inode in csv_dict["INODE"]:
        if inode_linkcount[int(inode[1])] != int(inode[6]):
            print("INODE {0} HAS {1} LINKS BUT LINKCOUNT IS {2}".format(inode[1], inode_linkcount[int(inode[1])], inode[6]))

    for dirent in csv_dict["DIRENT"]:
        if int(dirent[1]) == 2 and dirent[6] == "'..'" and int(dirent[1]) != int(dirent[3]):
            print("DIRECTORY INODE {0} NAME '..' LINK TO INODE {1} SHOULD BE {0}".format(dirent[1], dirent[3]))
        elif int(dirent[1]) != 2 and dirent[6] == "'..'" and int(inode_parent[int(dirent[1])]) != int(dirent[3]):
            print("DIRECTORY INODE {0} NAME '..' LINK TO INODE {1} SHOULD BE {0}".format(dirent[1], dirent[3]))


def main():
    if len(sys.argv) != 2:
        print("Error: invalid command line argument")
        sys.exit(1)
    
    with open(str(sys.argv[1]), "r") as csv_file:
        csv_dict = read_process_csv(csv_file.readlines())

    check_blocks(csv_dict)
    check_inodes(csv_dict)
    check_directories(csv_dict)

    


    #data_block_start = get_data_block_start(csv_file)

if __name__ == "__main__":
    main()



"""



---
legal data block (every block between the end of the I-nodes and the start of the next group) = [start, end]
legal_data_block_start = 
block number of first block of i-nodes in this group (decimal) 
+ i-nodes per group (decimal) * i-node size (in bytes, decimal) / block size (in bytes, decimal)
legal_data_block_end = 
total number of blocks in this group (decimal) - 1
---
inode [start, end]
inode_start = i-nodes per group (decimal)
inode_end = first non-reserved i-node (decimal)
inode 2 exception
---
inode number of the referenced file (decimal)
---

"""

















