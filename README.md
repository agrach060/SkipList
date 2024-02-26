# Skiplist

Goals: Working with pointers and linked lists

clang-tidy warnings:
    /home/NETID/agrach/2023win342d-p4-agrach060/skiplist.cpp:37:20: error: 2 adjacent parameters of 'SkipList' of similar type ('int') are easily swapped by mistake [bugprone-easily-swappable-parameters,-warnings-as-errors]
    SkipList::SkipList(int levels, int probability)
                    ^~~~~~~~~~~~~~~~~~~~~~~~~~~
    /home/NETID/agrach/2023win342d-p4-agrach060/skiplist.cpp:37:24: note: the first parameter in the range is 'levels'
    SkipList::SkipList(int levels, int probability)
                        ^~~~~~
    /home/NETID/agrach/2023win342d-p4-agrach060/skiplist.cpp:37:36: note: the last parameter in the range is 'probability'
    SkipList::SkipList(int levels, int probability)
These warnings are caused by the original design of the SkipList.

I couldn't fix these memory leaks: 
==2096263==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 64 byte(s) in 2 object(s) allocated from:
    #0 0x7f5d837337b0 in operator new(unsigned long) (/lib64/libasan.so.5+0xf17b0)
    #1 0x40681d in SkipList::SkipList(SkipList const&) /home/NETID/agrach/2023win342d-p4-agrach060/skiplist.cpp:47
    #2 0x40487b in test4() /home/NETID/agrach/2023win342d-p4-agrach060/main.cpp:110
    #3 0x404f27 in main /home/NETID/agrach/2023win342d-p4-agrach060/main.cpp:135
    #4 0x7f5d82987d84 in __libc_start_main (/lib64/libc.so.6+0x3ad84)

Indirect leak of 128 byte(s) in 4 object(s) allocated from:
    #0 0x7f5d837337b0 in operator new(unsigned long) (/lib64/libasan.so.5+0xf17b0)
    #1 0x40681d in SkipList::SkipList(SkipList const&) /home/NETID/agrach/2023win342d-p4-agrach060/skiplist.cpp:47
    #2 0x40487b in test4() /home/NETID/agrach/2023win342d-p4-agrach060/main.cpp:110
    #3 0x404f27 in main /home/NETID/agrach/2023win342d-p4-agrach060/main.cpp:135
    #4 0x7f5d82987d84 in __libc_start_main (/lib64/libc.so.6+0x3ad84)

Indirect leak of 112 byte(s) in 6 object(s) allocated from:
    #0 0x7f5d837337b0 in operator new(unsigned long) (/lib64/libasan.so.5+0xf17b0)
    #1 0x40b572 in __gnu_cxx::new_allocator<SNode*>::allocate(unsigned long, void const*) /usr/include/c++/8/ext/new_allocator.h:111
    #2 0x40b081 in std::allocator_traits<std::allocator<SNode*> >::allocate(std::allocator<SNode*>&, unsigned long) /usr/include/c++/8/bits/alloc_traits.h:436
    #3 0x40a771 in std::_Vector_base<SNode*, std::allocator<SNode*> >::_M_allocate(unsigned long) /usr/include/c++/8/bits/stl_vector.h:296
    #4 0x40a958 in std::_Vector_base<SNode*, std::allocator<SNode*> >::_M_create_storage(unsigned long) /usr/include/c++/8/bits/stl_vector.h:311
    #5 0x4094f2 in std::_Vector_base<SNode*, std::allocator<SNode*> >::_Vector_base(unsigned long, std::allocator<SNode*> const&) /usr/include/c++/8/bits/stl_vector.h:260
    #6 0x4081cb in std::vector<SNode*, std::allocator<SNode*> >::vector(std::vector<SNode*, std::allocator<SNode*> > const&) /usr/include/c++/8/bits/stl_vector.h:460
    #7 0x4062a1 in SNode::SNode(SNode const&) /home/NETID/agrach/2023win342d-p4-agrach060/skiplist.cpp:25
    #8 0x40682f in SkipList::SkipList(SkipList const&) /home/NETID/agrach/2023win342d-p4-agrach060/skiplist.cpp:47
    #9 0x40487b in test4() /home/NETID/agrach/2023win342d-p4-agrach060/main.cpp:110
    #10 0x404f27 in main /home/NETID/agrach/2023win342d-p4-agrach060/main.cpp:135
    #11 0x7f5d82987d84 in __libc_start_main (/lib64/libc.so.6+0x3ad84)

SUMMARY: AddressSanitizer: 304 byte(s) leaked in 12 allocation(s).

I understand that the issue is with my copy constructor for Skiplist and that it is creating too many nodes but I couldn't understand how to fix it.